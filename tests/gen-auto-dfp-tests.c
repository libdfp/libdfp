/* Generator for Decimal Floating Point (TS 18661 part 2) functions

   Copyright (C) 2019 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   The Decimal Floating Point C Library is free software; you can
   redistribute it and/or modify it under the terms of the GNU Lesser
   General Public License version 2.1.

   The Decimal Floating Point C Library is distributed in the hope that
   it will be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
   the GNU Lesser General Public License version 2.1 for more details.

   You should have received a copy of the GNU Lesser General Public
   License version 2.1 along with the Decimal Floating Point C Library;
   if not, write to the Free Software Foundation, Inc., 51 Franklin
   Street, Fifth Floor, Boston, MA 02110-1301 USA.

   Please see libdfp/COPYING.txt for more information. */

/*
   gcc gen-auto-dfp-tests.c -Wall -O2 -o gen-auto-dfp-tests -lmpfr -lgmp -ldecnumber

   This framework is similar in spirit to the glibc math benchmarking
   program. As of writing, no readily available, and licensing compatible
   frameworks exist to produce correctly rounded results for the majority
   of decimal IEEE 754 functions.

   For trigonometric and transcendental functions, we defer to mpfr. It
   is generally accepted as the oracle. We choose a sufficiently high
   precision and round. This should give us results correct to 1ULP.
   This is not perfect, but it gives us a starting point.

   For functions which can be correctly computed via libdecnumber, we
   defer.

   Ideally, we could copy and extend the glibc version, but in practice
   most if it is highly tailored to the nuances of binary floating point.

   TODO: Correctly round decimal values from MPFR
   TODO: Correctly rounded values for all 5 rounding modes



   For now, we take as input a file which lists tests in form:

   # Optional comment lines
   func arg1 arg2 ... argN

   For each function, we generate file:

   auto-dfp-test-out-FUNC

   of the form:
   func arg1 arg2 ... argN
   = func format arg1 arg2 ... argN : out1 out2 ... outN

*/

#define _GNU_SOURCE

#include <mpfr.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <error.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

/* libbid uses the level of precision converting. Let's start here.  */
#define DFP_MPFR_PREC 512
#define DFP_DEFAULT_RND MPFR_RNDN

/* Use libdecnumber as the oracle for testing functions where possible.  */
#define DECNUMDIGITS 34
#include <decnumber/decContext.h>
#include <decnumber/decNumber.h>

/*
  TODO: only mpfr now. Assuming libdecnumber is sufficiently correctly for
        for the functions it natively supports (e.g <= 1ULP), we can use
        it to bootstrap/verify the mpfr usage below for functions libdecnumber
        does not natively support.
*/
enum func_type
{
  mpfr_d_d,
  mpfr_dd_d,
  dec_dd_d,
  dec_dD_d,
  dec_d_si,
  dec_d_sl,
  _func_type_cnt
};

/* Table 3.6 in IEEE 754-2008 */
struct dec_fmt_param
{
  int p;
  int emax;
  int emin;
  int emin_normal;
  const char *name;
  int dctxtiv; /* Second argument to decContextDefault */
  mpfr_t frmax;
  mpfr_t frmin;
};

enum dec_type
{
  decimal32,
  decimal64,
  decimal128,
  decimal_fmt_cnt,
};

struct dec_fmt_param dec_fmt_param[] =
{
  {7, 96, -95 - 6, -95, "decimal32", DEC_INIT_DECIMAL32},
  {16, 384, -383 - 15, -383, "decimal64", DEC_INIT_DECIMAL64},
  {34, 6144, -6143 - 33, -6143, "decimal128", DEC_INIT_DECIMAL128},
};
#define DEC_MAX_FMT_STR (10)

/* Keep the results as strings. */
typedef struct decimal_value
{
  /* Enough to hold <sign><digits>e<sign><exp digits> for the largest type */
  char v[3][1 + 34 + 2 + 4 + 1];
} decimal_value_t;
#define DEC_MAX_STR (int)(sizeof(((decimal_value_t*)0)->v[0]) - 1)

typedef union result
  {
  int si;
  long long int di;
  decimal_value_t d_;
  } result_t;

struct testf;

typedef struct test
{
  int line;
  const char *stest; /* allocated */
  mpfr_t frin[decimal_fmt_cnt];
  mpfr_t frin2[decimal_fmt_cnt];
  decNumber din[decimal_fmt_cnt];
  decNumber din2[decimal_fmt_cnt];
  int excepts;
  result_t input;
  result_t input2; /* TODO: only up to two args of same type supported now. */
  result_t result; /* TODO: only one type supported now. */
  struct testf *tf;
} test_t;

union func_ptr
{
  int (*mpfr_d_d) (mpfr_t out, mpfr_t const in, mpfr_rnd_t rnd);
  int (*mpfr_dd_d) (mpfr_t out, mpfr_t const in1, mpfr_t const in2, mpfr_rnd_t rnd);
  decNumber* (*dec_dd_d) (decNumber*, const decNumber*, const decNumber*, decContext*);
  void (*dec_d_si) (char*, const decNumber*, decContext*);
  void (*dec_d_li) (char*, const decNumber*, decContext*);
};

typedef struct testf
{
  enum func_type ftype;
  union func_ptr func;
  const char *fname;
  size_t testno;
  test_t *tests;
  char **compat_tests; /* TODO: holdover tests while transitioning */
  size_t compat_testno;
  size_t compat_chr_skip;
} testf_t;

typedef void (*special_init_func)(const char *v, mpfr_t in[decimal_fmt_cnt], decNumber din[decimal_fmt_cnt]);

typedef struct special
{
  const char *name;
  special_init_func set_val;
} special_t;

/* There is no real oracle for ilog10, so one is made with decnumber. */
void decnumber_ilog10(char *out, const decNumber *in, decContext *ctxt)
{
  if (decNumberIsZero (in))
    strcpy (out, "FP_ILOGB0");
  else if (decNumberIsInfinite (in) && decNumberIsNegative (in))
    strcpy (out, "INT_MIN");
  else if (decNumberIsInfinite (in) && !decNumberIsNegative (in))
    strcpy (out, "INT_MAX");
  else if (decNumberIsNaN (in))
    strcpy (out, "FP_ILOGBNAN");
  else
    sprintf (out,"%d", in->digits + in->exponent - 1);
}

/* Likewise for the effectively identical llog10. */
void decnumber_llog10(char *out, const decNumber *in, decContext *ctxt)
{
  if (decNumberIsZero (in))
    strcpy (out, "FP_LLOGB0");
  else if (decNumberIsInfinite (in) && decNumberIsNegative (in))
    strcpy (out, "LONG_MIN");
  else if (decNumberIsInfinite (in) && !decNumberIsNegative (in))
    strcpy (out, "LONG_MAX");
  else if (decNumberIsNaN (in))
    strcpy (out, "FP_LLOGBNAN");
  else
    sprintf (out,"%d", in->digits + in->exponent - 1);
}

/* This is the algorithm proposed in Nelson H.F. Beebe's The Mathematical-Function Computation Handbook.
   Note, that for decimal floating point, it requires 1 extra digit of precision beyond the input format
   for correctness (unlike binary float).  */
decNumber *
decNumberFmod(decNumber *result, const decNumber *x, const decNumber *y, decContext *ctxt)
{
  /* This requires 35 digits to correctly compute any <=34 digit remainder. */
  if (decNumberIsSpecial (x) || decNumberIsSpecial(y) || decNumberIsZero(y))
    {
      result->bits = DECNAN;
      return result;
    }

  /* The remainder may contain 1 extra digit. Add extra room for at least 1 more. */
  struct {
  	decNumber d;
        decNumberUnit extra;
  } r, yabs, xabs, cmp, yabs_scaled;
  int ny;

  decNumberAbs (&xabs.d, x, ctxt);
  decNumberAbs (&yabs.d, y, ctxt);

  decNumberCompare (result, &xabs.d, &yabs.d, ctxt);

  if (decNumberIsZero (result))
    {
      decNumberCopySign (result, result, x);
      return result;
    }
  else if (result->bits & DECNEG)
    {
      decNumberCopy (result, x);
      return result;
    }

  /* All special cases accounted for, find the remainder.  */
  decNumberCopy (&r.d, &xabs.d);
  decNumberCopy (&yabs_scaled.d, &yabs.d);
  ny = yabs.d.exponent + yabs.d.digits - 1;

  /* Expand the precision to while computing the remainder.  */
  ctxt->digits = DECNUMDIGITS + 1;

  decNumberCompare (&cmp.d, &r.d, &yabs.d, ctxt);
  while (!(cmp.d.bits & DECNEG)) /* r >= yabs */
    {
      int n = (r.d.exponent + r.d.digits - 1) - ny;
      /* Rescale yabs to the nearest power of 10 less than r.  */
      yabs_scaled.d.exponent = yabs.d.exponent + n;
      decNumberCompare (&cmp.d, &yabs_scaled.d, &r.d, ctxt);
      if (!decNumberIsZero (&cmp.d) && !(cmp.d.bits & DECNEG))
        yabs_scaled.d.exponent--;

      decNumberSubtract (&r.d, &r.d, &yabs_scaled.d, ctxt); /* r = r - yabs_scaled */
      decNumberCompare (&cmp.d, &r.d, &yabs.d, ctxt); /* cmp = r ? yabs */
    }

  ctxt->digits = DECNUMDIGITS;
  decNumberNormalize (result, &r.d, ctxt);
  if (x->bits & DECNEG)
    result->bits |= DECNEG;
  return result;
}


void get_nan(const char *v, mpfr_t in[decimal_fmt_cnt], decNumber din[decimal_fmt_cnt])
{
  for(int i = 0; i < decimal_fmt_cnt; i++)
    {
      mpfr_init_set_si (in[i], 0, DFP_DEFAULT_RND);
      mpfr_set_nan (in[i]);
      mpfr_setsign (in[i], in[i], v[0] == '-' ? 1 : 0, DFP_DEFAULT_RND);
      decNumberZero(&din[i]);
      din[i].bits |= DECNAN | (v[0] == '-' ? DECNEG : 0);
    }
}

/* Add a mark to the payload. */
void get_snan(const char *v, mpfr_t in[decimal_fmt_cnt], decNumber din[decimal_fmt_cnt])
{
  for(int i = 0; i < decimal_fmt_cnt; i++)
    {
      mpfr_init_set_si (in[i], 0, DFP_DEFAULT_RND);
      mpfr_set_nan (in[i]);
      mpfr_setsign (in[i], in[i], v[0] == '-' ? 1 : 0, DFP_DEFAULT_RND);
      decNumberZero(&din[i]);
      din[i].bits |= DECNAN | DECSNAN | (v[0] == '-' ? DECNEG : 0);
    }
}

void get_min(const char *v, mpfr_t in[decimal_fmt_cnt], decNumber din[decimal_fmt_cnt])
{
  decNumber dtmp;
  decContext dctxt;

  decNumberZero (&dtmp);

  for(int i = 0; i < decimal_fmt_cnt; i++)
    {
      mpfr_init_set (in[i], dec_fmt_param[i].frmin, DFP_DEFAULT_RND);
      mpfr_setsign (in[i], in[i], v[0] == '-' ? 1 : 0, DFP_DEFAULT_RND);
      decContextDefault (&dctxt, dec_fmt_param[i].dctxtiv);
      decNumberNextPlus (&din[i], &dtmp, &dctxt);
      din[i].bits |= (v[0] == '-' ? DECNEG : 0);
    }
}

void get_max(const char *v, mpfr_t in[decimal_fmt_cnt], decNumber din[decimal_fmt_cnt])
{
  decNumber dtmp;
  decContext dctxt;

  decNumberZero (&dtmp);
  dtmp.bits |= DECINF;

  for(int i = 0; i < decimal_fmt_cnt; i++)
    {
      mpfr_init_set (in[i], dec_fmt_param[i].frmax, DFP_DEFAULT_RND);
      mpfr_setsign (in[i], in[i], v[0] == '-' ? 1 : 0, DFP_DEFAULT_RND);
      decContextDefault (&dctxt, dec_fmt_param[i].dctxtiv);
      decNumberNextMinus (&din[i], &dtmp, &dctxt);
      din[i].bits |= (v[0] == '-' ? DECNEG : 0);
    }
}

void get_inf(const char *v, mpfr_t in[decimal_fmt_cnt], decNumber din[decimal_fmt_cnt])
{
  for(int i = 0; i < decimal_fmt_cnt; i++)
    {
      mpfr_init (in[i]);
      mpfr_set_inf (in[i], v[0] == '-' ? -1 : 0);
      decNumberZero(&din[i]);
      din[i].bits |= DECINF | (v[0] == '-' ? DECNEG : 0);
    }
}

/* TODO: glibc libm punts NaN tests. Should we? sNaN requires some more though */
special_t dec_spec_vals[] =
{
  { "nan", get_nan },
  { "-nan", get_nan },
  { "min", get_min },
  { "-min", get_min },
  { "max", get_max },
  { "-max", get_max },
  { "inf", get_inf },
  { "-inf", get_inf },
  {}
};

#define DECL_TESTF_D_D(func) { mpfr_d_d, { .mpfr_d_d = mpfr_ ## func,  }, \
                               #func, 0, NULL, NULL, 0, sizeof(#func) + 1 }

#define DECL_TESTF_DD_D(func) { mpfr_dd_d, { .mpfr_dd_d = mpfr_ ## func,  }, \
                               #func, 0, NULL, NULL, 0, sizeof(#func) + 1 }

#define DECL_TESTF_DEC_DD_D(func, decname) \
                              { dec_dd_d, { .dec_dd_d =  decNumber ## decname,  }, \
                                #func, 0, NULL, NULL, 0, sizeof(#func) + 1 }

#define DECL_TESTF_DEC_DD128_D(func, decname) \
                              { dec_dD_d, { .dec_dd_d =  decNumber ## decname,  }, \
                                #func, 0, NULL, NULL, 0, sizeof(#func) + 1 }

#define DECL_TESTF_DEC_D_SI(func, decname) \
                              { dec_d_si, { .dec_d_si = decname, }, \
                                #func, 0, NULL, NULL, 0, sizeof(#func) + 1 }

#define DECL_TESTF_DEC_D_SL(func, decname) \
                              { dec_d_sl, { .dec_d_si = decname, }, \
                                #func, 0, NULL, NULL, 0, sizeof(#func) + 1 }

testf_t testlist[] =
{
  DECL_TESTF_D_D(cos),
  DECL_TESTF_D_D(acos),
  DECL_TESTF_D_D(sin),
  DECL_TESTF_D_D(asin),
  DECL_TESTF_D_D(tan),
  DECL_TESTF_D_D(atan),
  DECL_TESTF_D_D(log),
  DECL_TESTF_D_D(log10),
  DECL_TESTF_D_D(log1p),
  DECL_TESTF_D_D(exp),
  DECL_TESTF_D_D(exp2),
  DECL_TESTF_DD_D(pow),
  /* Use a local implementation of fmod using decNumber and extended precision. binary fmod needs a huge amount of
     precision for correctness in decimal, and decNumberRemainder fails for sufficiently far apart values.  */
  DECL_TESTF_DEC_DD_D(fmod, Fmod),
  DECL_TESTF_DEC_DD_D(nextafter, NextToward),
  DECL_TESTF_DEC_DD128_D(nexttoward, NextToward),
  DECL_TESTF_DEC_D_SI(ilogb, decnumber_ilog10),
  DECL_TESTF_DEC_D_SL(llogb, decnumber_llog10),
  { }
};

char *
xstrdup(const char *in)
{
  char * out = strdup (in);
  if (!out)
    error (EXIT_FAILURE, errno, "strdup");
  return out;
}

void
parse_next_val(mpfr_t frin[3], decNumber din[3], const char *val, char **endptr)
{
  mpfr_t frin_tmp;
  char *decstr = xstrdup (val);
  *strchrnul (decstr,' ') = 0;


  // val may contain more than 1 value
  for (special_t *sv = &dec_spec_vals[0]; sv->name; sv++)
    if (!strcasecmp(sv->name, decstr))
      {
        sv->set_val (val, frin, din);
        *endptr = (char*)&val[strlen(sv->name)];
        free (decstr);
        return;
      }

  // Parse the value into libdecnumber. It handles special values.
  for (int fmt = 0; fmt < decimal_fmt_cnt; fmt++)
    {
      decContext dctxt;
      decContextDefault (&dctxt, dec_fmt_param[fmt].dctxtiv);
      decNumberFromString (&din[fmt], decstr, &dctxt);
    }
  free(decstr);

  /* For now, only accept decimal inputs as radix-2 operands can cause
     false errors if they fall between 2 dfp values.
     TODO: can rounding to the nearest radix-10 value solve these errors?

     e.g, the value taken from glibc libm test suite:

     exp 0x2.c5c85fdf473dep+8
     exp 7097827128933839730962063185870647e-31
  */
  mpfr_init (frin_tmp);
  mpfr_strtofr (frin_tmp, val, endptr, 10, DFP_DEFAULT_RND);
  if (**endptr == 0 || **endptr == ' ')
    for (int fmt = 0; fmt < decimal_fmt_cnt; fmt++)
      {
        char str[128];

        /*
          If the input is subnormal, round to the reduced number of digits when converting.
        */
        mpfr_exp_t exp;
        mpfr_get_str(str, &exp, 10, dec_fmt_param[fmt].p - 1, frin_tmp, DFP_DEFAULT_RND);
        if (exp < dec_fmt_param[fmt].emin_normal && exp >= dec_fmt_param[fmt].emin)
          {
            mpfr_snprintf (str, sizeof(str) - 1, "%.*Re", exp - dec_fmt_param[fmt].emin - 1, frin_tmp);
            mpfr_init_set_str (frin[fmt], str, 0, DFP_DEFAULT_RND);
          }
        else if (exp < dec_fmt_param[fmt].emin)
          {
            /* Too small for the format.  Round to zero and preserve sign.  */
            mpfr_set_zero(frin[fmt], mpfr_signbit(frin_tmp) ? -1 : 0);
          }
        else
          {
            /*
              This wins no awards, but round-trip values through
              strings to better approximate the _DecimalN value.
            */
            mpfr_snprintf (str, sizeof(str) - 1, "%.*Re", dec_fmt_param[fmt].p - 1, frin_tmp);
            mpfr_init_set_str (frin[fmt], str, 0, DFP_DEFAULT_RND);
          }
      }
  else
    error (EXIT_FAILURE, errno, "Failed to convert: %s", val);
  mpfr_clear(frin_tmp);
}

void
alloc_test(testf_t *tf, const char *val, char *test)
{
  test_t *t = NULL;
  char *endptr = "";

  tf->testno++;
  tf->tests = realloc (tf->tests, tf->testno * sizeof (tf->tests[0]));
  t = &tf->tests[tf->testno - 1];

  if (!tf->tests)
    error (EXIT_FAILURE, errno, "realloc");
  memset (t, 0, sizeof (*t));
  t->stest = test;
  t->tf = tf;

  parse_next_val(t->frin, t->din, val, &endptr);
  if (*endptr == ' ')
    parse_next_val(t->frin2, t->din2, endptr+1, &endptr);
}

/*
  For now, only parse lines of the form:
  func value

  Add more complexity as required for testing.
*/
void
parse_line(char *line, const char *filename, size_t lineno)
{
  char *test = xstrdup (line);
  char *val = strchr (line, ' ');
  char *func = NULL;
  testf_t *t = NULL;
  bool passthru = false;

  if (!val)
    goto failure;

  func = line;
  *val = '\0';
  val++;

  /* Don't process these, just pass these lines through. e.g for sNAN */
  passthru = func[0] == '\'';
  if (passthru)
      func++;

  for (t = &testlist[0]; t->fname; t++)
    if (!strcmp (t->fname, func))
      break;

  if (!t->fname)
    goto failure;

  if (!passthru)
    alloc_test (t, val, test);
  else
    {
      t->compat_testno++;
      t->compat_tests = realloc (t->compat_tests, sizeof(*t->compat_tests) * t->compat_testno);
      if (!t->compat_tests)
        error (EXIT_FAILURE, 0, "realloc failure");
      t->compat_tests[t->compat_testno - 1] = test;
    }

  return;

failure:
  error_at_line (EXIT_FAILURE, 0, filename, lineno, "failed to parse '%s'", test);
  if (test)
    free (test);
};

/* Slightly misleading... round bfp to dfp result using default rounding mode. */
void
round_result(mpfr_t in[decimal_fmt_cnt], decimal_value_t *out)
{
  for (int i = 0; i < decimal_fmt_cnt; i++)
    {
      /* Handle 0 and inf cases. */
      if (mpfr_nan_p (in[i]))
        sprintf (out->v[i], "%sNaN", mpfr_signbit (in[i]) ? "-" : "");
      else if (mpfr_cmpabs (in[i], dec_fmt_param[i].frmax) > 0)
        sprintf (out->v[i], "%sInf", mpfr_signbit (in[i]) ? "-" : "");
      else if (mpfr_cmpabs(dec_fmt_param[i].frmin, in[i]) > 0)
        sprintf (out->v[i], "%s0.0", mpfr_signbit (in[i]) ? "-" : "");
      else
        {
          mpfr_exp_t exp;
          uintptr_t adj = mpfr_signbit (in[i]) ? 1 : 0;

          /* Remove implicit radix point by shifting the exponent per format */
          mpfr_get_str (out->v[i], &exp, 10, dec_fmt_param[i].p, in[i], DFP_DEFAULT_RND);
          exp -= dec_fmt_param[i].p;

          /* Sanity and subnormal check. */
          if (exp < dec_fmt_param[i].emin)
            {
              /* This should be a subnormal number. Determine how many decimals to clip and rewrite the string. */
              if (exp > (dec_fmt_param[i].emin - dec_fmt_param[i].p))
                {
                  int dig = dec_fmt_param[i].p - (dec_fmt_param[i].emin - exp);
                  mpfr_get_str (out->v[i], &exp, 10, dig, in[i], DFP_DEFAULT_RND);
                  sprintf (out->v[i] + dig + adj, "e%d", (int)(exp - dig));
                }
              else
                error(EXIT_FAILURE, 0, "Conversion failure. Subnormal value rounds to 0. %se%d", out->v[i], (int)exp);
            }
          else if (exp > dec_fmt_param[i].emax)
            error(EXIT_FAILURE, 0, "Conversion failure. Exponent out of range %d > %d", (int)exp, dec_fmt_param[i].emax);
          else
            sprintf (out->v[i] + dec_fmt_param[i].p + adj, "e%d", (int) exp);
        }
    }
}

void
decnum_to_str(const decNumber *d, char *out)
{
  if (decNumberIsNegative(d))
    *out++ = '-';

  if (decNumberIsSNaN(d))
     strcpy (out, "sNaN");
  else if (decNumberIsQNaN(d))
     strcpy (out, "NaN");
  else if (decNumberIsInfinite(d))
     strcpy (out, "Inf");
  else
    {
      decNumber e;

      decNumberCopyAbs(&e, d);
      e.exponent = 0;
      decNumberToString(&e, out);
      sprintf(&out[e.digits],"e%d",d->exponent);
    }
}

void
compute(test_t *t)
{
  switch (t->tf->ftype)
    {
      case mpfr_d_d:
        {
          mpfr_t tmp[decimal_fmt_cnt];

          for (int fmt=0; fmt < decimal_fmt_cnt; fmt++)
            {
              mpfr_init (tmp[fmt]);
              t->tf->func.mpfr_d_d (tmp[fmt], t->frin[fmt], DFP_DEFAULT_RND);
            }

          round_result (t->frin, &t->input.d_);
          round_result (tmp, &t->result.d_);

          for (int fmt=0; fmt < decimal_fmt_cnt; fmt++)
            mpfr_clear (tmp[fmt]);
        }
        break;

      case mpfr_dd_d:
        {
          mpfr_t tmp[decimal_fmt_cnt];

          for (int fmt=0; fmt < decimal_fmt_cnt; fmt++)
            {
              mpfr_init (tmp[fmt]);
              t->tf->func.mpfr_dd_d (tmp[fmt], t->frin[fmt], t->frin2[fmt], DFP_DEFAULT_RND);
            }

          round_result (t->frin, &t->input.d_);
          round_result (t->frin2, &t->input2.d_);
          round_result (tmp, &t->result.d_);

          for (int fmt=0; fmt < decimal_fmt_cnt; fmt++)
            mpfr_clear (tmp[fmt]);
        }
        break;

      case dec_dD_d:
        {
          /* A special case where the second argument is always _Decimal128.  e.x nexttoward.  */
          for (int fmt=0; fmt < decimal_fmt_cnt; fmt++)
            {
              decContext c;
              decNumber r;

              decContextDefault(&c, dec_fmt_param[fmt].dctxtiv);
              t->tf->func.dec_dd_d (&r, &t->din[fmt], &t->din2[decimal128], &c);

              decnum_to_str(&t->din[fmt], t->input.d_.v[fmt]);
              decnum_to_str(&t->din2[decimal128], t->input2.d_.v[fmt]);
              decnum_to_str(&r, t->result.d_.v[fmt]);
            }
        }
        break;

      case dec_dd_d:
        {
          for (int fmt=0; fmt < decimal_fmt_cnt; fmt++)
            {
              decContext c;
              decNumber r;

              decContextDefault(&c, dec_fmt_param[fmt].dctxtiv);
              t->tf->func.dec_dd_d (&r, &t->din[fmt], &t->din2[fmt], &c);

              decnum_to_str(&t->din[fmt], t->input.d_.v[fmt]);
              decnum_to_str(&t->din2[fmt], t->input2.d_.v[fmt]);
              decnum_to_str(&r, t->result.d_.v[fmt]);
            }
        }
        break;

      case dec_d_sl:
        /* This has the same calling conventions as dec_d_si, fallthrough. */
      case dec_d_si:
        {
          for (int fmt=0; fmt < decimal_fmt_cnt; fmt++)
            {
              decContext c;

              decContextDefault(&c, dec_fmt_param[fmt].dctxtiv);
              t->tf->func.dec_d_si (t->result.d_.v[fmt], &t->din[fmt], &c);
              decnum_to_str(&t->din[fmt], t->input.d_.v[fmt]);
            }
        }
        break;

      default:
        error (EXIT_FAILURE, 0, "Unknown function type %d", (int)t->tf->ftype);
        break;
    }

}

void
init_fmt(void)
{
  decimal_value_t d;

  /* get approximate maximum finite value in radix-2 */
  for (int i = 0; i < decimal_fmt_cnt; i++)
    {
      int p = dec_fmt_param[i].p;

      /* TODO: add a tiny value e.g 1eMIN_FMT_EXP */
      memset(d.v[i], '9', dec_fmt_param[i].p);
      sprintf(d.v[i] + p,"e%d",dec_fmt_param[i].emax - p + 1);
      mpfr_init_set_str(dec_fmt_param[i].frmax, d.v[i], 10, MPFR_RNDZ);
      sprintf(d.v[i],"1e%d",dec_fmt_param[i].emin);
      mpfr_init_set_str(dec_fmt_param[i].frmin, d.v[i], 10, MPFR_RNDZ);
      /* mpfr_printf("%.*Re\n", dec_fmt_param[i].p - 1, dec_fmt_param[i].frmin); */
      /* mpfr_printf("%.*Re\n", dec_fmt_param[i].p - 1, dec_fmt_param[i].frmax); */
    }
}

/*
  For the moment, recycle the existing *.input format until more
  meaningful refactoring can occur.

  Likewise, generate a makefile stub ensure these tests
  are built.
*/
void
gen_output(const char *fprefix)
{
  char fname[128];
  FILE *out = stdout;
  bool debug = !strcmp (fprefix, "-");
  FILE *automf = NULL;

  if (!debug)
    {
      automf = fopen ("Makefile.autotest", "w");
      if (!automf)
        error (EXIT_FAILURE, 0, "Failed to open makefile file");
      fprintf (automf, "# The is a generated file from gen-auto-dfp-tests\n"
                       "libdfp-auto-tests =");
    }

  for (testf_t *tf = &testlist[0]; tf->fname; tf++)
    {
      if (!tf->testno)
        continue;

      if (!debug)
        {
          snprintf (fname, sizeof (fname) - 1, "%s%s.input", fprefix, tf->fname);
          out = fopen (fname, "w");
          fprintf (automf, " %s", tf->fname);
        }

      if (!out)
        error(EXIT_FAILURE, errno, "failed to open for writing %s", fname);

      char *argstr[_func_type_cnt] =
        {
          "# arg1 decimal\n# ret  decimal",
          "# arg1 decimal\n# arg2 decimal\n# ret  decimal",
          "# arg1 decimal\n# arg2 decimal\n# ret  decimal",
          "# arg1 decimal\n# arg2 decimal128\n# ret  decimal",
          "# arg1 decimal\n# ret  int",
          "# arg1 decimal\n# ret  longint",
        };

      fprintf (out, "# name %s\n%s\n", tf->fname, argstr[tf->ftype]);

      for (int i = 0; i < tf->testno; i++)
        {
          test_t *t = &tf->tests[i];

          /* fprintf (out, "%s\n", t->stest); */
          for (int fmt = 0; fmt < decimal_fmt_cnt; fmt++)
            {
              switch (t->tf->ftype)
                {
                case dec_d_si:
                case dec_d_sl:
                case mpfr_d_d:
                  /* fprintf (out, "= %s %s %s : %s\n", t->tf->fname, fmt_str[fmt], t->input.d_.v[fmt], t->result.d_.v[fmt]); */
                  fprintf (out, "%-*s %-*s %-*s\n", DEC_MAX_STR, t->input.d_.v[fmt],
                                                    DEC_MAX_STR, t->result.d_.v[fmt],
                                                    DEC_MAX_FMT_STR, dec_fmt_param[fmt].name);
                  break;

                case dec_dd_d:
                case dec_dD_d:
                case mpfr_dd_d:
                  /* fprintf (out, "= %s %s %s %s : %s\n", t->tf->fname, fmt_str[fmt], t->input.d_.v[fmt], t->input.d_.v[fmt], t->result.d_.v[fmt]); */
                  fprintf (out, "%-*s %-*s %-*s %-*s\n", DEC_MAX_STR, t->input.d_.v[fmt],
                                                         DEC_MAX_STR, t->input2.d_.v[fmt],
                                                         DEC_MAX_STR, t->result.d_.v[fmt],
                                                         DEC_MAX_FMT_STR, dec_fmt_param[fmt].name);
                  break;

                default:
                  error (EXIT_FAILURE, 0, "Unhandled type %d", t->tf->ftype);
                  break;
                }
            }
        }

      /* TODO: passthrough tests to transition to glibc style testing. */
      for (int i = 0; i < tf->compat_testno; i++)
        fprintf (out, "%s\n", tf->compat_tests[i] + tf->compat_chr_skip);

      if (!debug)
        fclose(out);
    }

  if (automf)
    {
      fprintf (automf, "\n");
      fclose (automf);
    }
}

int
main(int args, char **argv)
{
  FILE *in = NULL;
  char *line = NULL;
  size_t linelen = 0;
  size_t lineno = 0;
  ssize_t ilinelen;

  mpfr_set_default_prec( DFP_MPFR_PREC );

  if (args < 3)
    error(EXIT_FAILURE, 0, "Usage %s: <auto-dfp-test-in> <output-prefix>", argv[0]);

  in = fopen (argv[1], "r");
  if (!in)
    error (EXIT_FAILURE, errno, "Cannot open %s", argv[1]);

  init_fmt();

  /* Generate tests */
  while ( ((ilinelen = getline (&line, &linelen, in))) != -1)
  {
    lineno++;
    /* Ignore empty or comment lines */
    if (!linelen || *line == '#' || *line == '\n')
      continue;

    /* chomp trailing whitespace/delimiter if any */
    while (isspace(line[ilinelen - 1]))
      {
        line[ilinelen - 1] = '\0';
        ilinelen--;
      }

    parse_line (line, argv[1], lineno);
  }

  /* Compute expected values. TODO: +/- 1ULP for now. */
  for (testf_t *tf = &testlist[0]; tf->fname; tf++)
    for (int i = 0; i < tf->testno; i++)
      compute (&tf->tests[i]);

  /* Generate output */
  gen_output(argv[2]);

  /* cleanup */
  fclose (in);
  if (line)
    free (line);

  return EXIT_SUCCESS;
}
