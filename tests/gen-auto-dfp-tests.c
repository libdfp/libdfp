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
   if not, write to the Free Software Foundation, Inc., 59 Temple Place,
   Suite 330, Boston, MA 02111-1307 USA.

   Please see libdfp/COPYING.txt for more information. */

/*
   gcc gen-auto-dfp-tests.c -Wall -O2 -o gen-auto-dfp-tests -lmpfr -lgmp -I..

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

#include <mpfr.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <error.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#define __STDC_WANT_IEC_60559_DFP_EXT__
#define __STDC_WANT_DEC_FP__
#include <dfp/math.h>


/* libbid uses the level of precision converting. Let's start here. */
#define DFP_MPFR_PREC 512
#define DFP_DEFAULT_RND MPFR_RNDN

/*
  TODO: only mpfr now. Assuming libdecnumber is sufficiently correctly for
        for the functions it natively supports (e.g <= 1ULP), we can use
        it to bootstrap/verify the mpfr usage below for functions libdecnumber
        does not natively support.
*/
enum func_type
{
  mpfr_d_d,
};

/* Table 3.6 in IEEE 754-2008 */
struct dec_fmt_param
{
  int p;
  int emax;
  int emin;
  const char *name;
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
  {7, 96, -95 - 6, "decimal32"},
  {16, 384, -383 - 15, "decimal64"},
  {34, 6144, -6143 - 33, "decimal128"},
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
  int excepts;
  result_t input; /* TODO: only one type support now. */
  result_t result; /* TODO: only one type supported now. */
  struct testf *tf;
} test_t;

union func_ptr
{
  int (*mpfr_d_d) (mpfr_t out, mpfr_t const in, mpfr_rnd_t rnd);
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

typedef void (*special_init_func)(const char *v, mpfr_t in[decimal_fmt_cnt]);

typedef struct special
{
  const char *name;
  special_init_func set_val;
} special_t;

void get_nan(const char *v, mpfr_t in[decimal_fmt_cnt])
{
  for(int i = 0; i < decimal_fmt_cnt; i++)
    {
      mpfr_init_set_si (in[i], 0, DFP_DEFAULT_RND);
      mpfr_set_nan (in[i]);
      mpfr_setsign (in[i], in[i], v[0] == '-' ? 1 : 0, DFP_DEFAULT_RND);
    }
}

/* Add a mark to the payload. */
void get_snan(const char *v, mpfr_t in[decimal_fmt_cnt])
{
  for(int i = 0; i < decimal_fmt_cnt; i++)
    {
      mpfr_init_set_si (in[i], 0, DFP_DEFAULT_RND);
      mpfr_set_nan (in[i]);
      mpfr_setsign (in[i], in[i], v[0] == '-' ? 1 : 0, DFP_DEFAULT_RND);
    }
}

void get_min(const char *v, mpfr_t in[decimal_fmt_cnt])
{
  for(int i = 0; i < decimal_fmt_cnt; i++)
    {
      mpfr_init_set (in[i], dec_fmt_param[i].frmin, DFP_DEFAULT_RND);
      mpfr_setsign (in[i], in[i], v[0] == '-' ? 1 : 0, DFP_DEFAULT_RND);
    }
}

void get_max(const char *v, mpfr_t in[decimal_fmt_cnt])
{
  for(int i = 0; i < decimal_fmt_cnt; i++)
    {
      mpfr_init_set (in[i], dec_fmt_param[i].frmax, DFP_DEFAULT_RND);
      mpfr_setsign (in[i], in[i], v[0] == '-' ? 1 : 0, DFP_DEFAULT_RND);
    }
}

void get_inf(const char *v, mpfr_t in[decimal_fmt_cnt])
{
  for(int i = 0; i < decimal_fmt_cnt; i++)
    {
      mpfr_init (in[i]);
      mpfr_set_inf (in[i], v[0] == '-' ? -1 : 0);
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
  { }
};

char *
xstrdup(char *in)
{
  char * out = strdup (in);
  if (!out)
    error (EXIT_FAILURE, errno, "strdup");
  return out;
}

void
alloc_test(testf_t *tf, const char *val, char *test)
{
  test_t *t = NULL;
  mpfr_t frin;
  char *endptr;

  tf->testno++;
  tf->tests = realloc (tf->tests, tf->testno * sizeof (tf->tests[0]));
  t = &tf->tests[tf->testno - 1];

  if (!tf->tests)
    error (EXIT_FAILURE, errno, "realloc");
  memset (t, 0, sizeof (*t));
  t->stest = test;
  t->tf = tf;

  /* Scan for specials, and skip conversion below. Ignore case. */
  for (special_t *sv = &dec_spec_vals[0]; sv->name; sv++)
    if (!strcasecmp(sv->name, val))
      {
        sv->set_val (val, t->frin);
        return;
      }

  /* For now, only accept decimal inputs as radix-2 operands can cause
     false errors if they fall between 2 dfp values.
     TODO: can rounding to the nearest radix-10 value solve these errors?

     e.g, the value taken from glibc libm test suite:

     exp 0x2.c5c85fdf473dep+8
     exp 7097827128933839730962063185870647e-31
  */
  mpfr_init (frin);
  mpfr_strtofr (frin, val, &endptr, 10, DFP_DEFAULT_RND);
  if (endptr && *endptr == 0)
    for (int fmt = 0; fmt < decimal_fmt_cnt; fmt++)
      {
        char str[128];

        /*
          This wins no awards, but round-trip values through
          strings to better approximate the _DecimalN value.
        */
        mpfr_snprintf (str, sizeof(str) - 1, "%.*Re", dec_fmt_param[fmt].p - 1, frin);
        mpfr_init_set_str (t->frin[fmt], str, 0, DFP_DEFAULT_RND);
      }
  mpfr_clear(frin);
  if (!endptr || *endptr != 0)
    error (EXIT_FAILURE, errno, "Failed to convert: %s", val);
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

/* Slightly misleaning... round bfp to dfp result using default rounding mode. */
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
      else if (mpfr_cmpabs(dec_fmt_param[i].frmin, in[i]) >= 0)
        sprintf (out->v[i], "%s0", mpfr_signbit (in[i]) ? "-" : "");
      else
        {
          mpfr_exp_t exp;
          uintptr_t adj = mpfr_signbit (in[i]) ? 1 : 0;

          /* Remove implicit radix point by shifting the exponent per format */
          mpfr_get_str (out->v[i], &exp, 10, dec_fmt_param[i].p, in[i], DFP_DEFAULT_RND);
          exp -= dec_fmt_param[i].p;
          if (exp > dec_fmt_param[i].emax || exp < dec_fmt_param[i].emin)
            error(EXIT_FAILURE, 0, "Conversion failure. Exponent out of range");
          sprintf (out->v[i] + dec_fmt_param[i].p + adj, "e%d", (int) exp);
        }
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

      fprintf (out, "# name %s\n"
                    "# arg1 decimal\n"
                    "# ret  decimal\n", tf->fname);

      for (int i = 0; i < tf->testno; i++)
        {
          test_t *t = &tf->tests[i];

          /* fprintf (out, "%s\n", t->stest); */
          for (int fmt = 0; fmt < decimal_fmt_cnt; fmt++)
            {
              switch (t->tf->ftype)
                {
                case mpfr_d_d:
                  /* fprintf (out, "= %s %s %s : %s\n", t->tf->fname, fmt_str[fmt], t->input.d_.v[fmt], t->result.d_.v[fmt]); */
                  fprintf (out, "%-*s %-*s %-*s\n", DEC_MAX_STR, t->input.d_.v[fmt],
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
