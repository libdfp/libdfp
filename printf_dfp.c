/* Function definition to convert DFP values to strings

   Copyright (C) 2006, 2009, 2010, 2011 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Ryan S. Arnold <rsa@us.ibm.com>
              Pete Eberlein <eberlein@us.ibm.com>

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

   Please see libdfp/COPYING.txt for more information.  */

/* Based on GLIBC stdio-common/printf_fp.c by:
   Ulrich Drepper <drepper@gnu.ai.mit.edu>  */


#include "printf_dfp.h"
#include <unistd.h>
/* wchar.h has to be included BEFORE stdio.h or it loses function
 * definitions when dfp/wchar.h uses #include_next <wchar.h>.  */
#include <wchar.h>
#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#include <fenv.h>

#include <langinfo.h>
#include <wctype.h>
#include <limits.h> /* For CHAR_MAX  */
#include <stdio.h>

#include <get_digits.h>
#define _DECIMAL_SIZE 128
#define DEC_TYPE _Decimal128
#include <numdigits.h>
#undef _DECIMAL_SIZE
#undef DEC_TYPE
#undef ADJUST
#undef Q
#undef DECIMAL_BIAS
#define _DECIMAL_SIZE 64
#define DEC_TYPE _Decimal64
#include <numdigits.h>
#undef _DECIMAL_SIZE
#undef DEC_TYPE
#undef ADJUST
#undef Q
#undef DECIMAL_BIAS
#define _DECIMAL_SIZE 32
#define DEC_TYPE _Decimal32
#include <numdigits.h>

static int
wpadn (FILE *fp, wint_t pad, int count)
{
  int i;
  int written = 0;

  for (i=0;i<count;i++)
      if(putwc(pad,fp) != WEOF)
	written++;

  return written;
}

static int
padn (FILE *fp, int pad, int count)
{
  int i;
  int written = 0;

  for (i=0;i<count;i++)
      if(putc(pad,fp) != EOF)
	written++;

  return written;
}

#ifdef COMPILE_WIDE
# ifndef L
#  define L(c) L##c
# endif
# ifndef CHAR_T
#  define CHAR_T wchar_t
# endif
# ifndef PUT
#  define PUT(x,y) putwc(x,y)
# endif
#else
# ifndef L
#  define L(c) c
# endif
# ifndef CHAR_T
#  define CHAR_T char
# endif
# ifndef PUT
#  define PUT(x,y) putc(x,y)
# endif
#endif

#define PAD(f, c, n) (wide ? wpadn (f, c, n) : padn (f, c, n))

#define size_t     _IO_size_t
#define FILE	     _IO_FILE

/* Macros for doing the actual output.  */

#define outchar(ch)							      \
  do									      \
    {									      \
      register const int outc = (ch);					      \
      if (PUT(outc, fp) == EOF)						      \
	{								      \
	  return -1;							      \
	}								      \
      ++done;								      \
    } while (0)

#define PADN(ch, len)							      \
  do									      \
    {									      \
      if ((PAD (fp, ch, len)) != len)					      \
	{								      \
	  return -1;							      \
	}								      \
      done += len;							      \
    }									      \
  while (0)

#define _OUT(pr) \
  do \
    { \
	int t_pr = 0; \
    if (pr < 0) \
      outchar('-');\
    if (__builtin_abs(pr) > 999)\
      {\
        outchar('0' + __builtin_abs(pr) / 1000);\
	t_pr = (__builtin_abs(pr) % 1000);\
        outchar('0' + t_pr/100);\
	t_pr = (__builtin_abs(t_pr) % 100);\
        outchar('0' + t_pr/10);\
	t_pr = (__builtin_abs(t_pr) % 10);\
        outchar('0' + t_pr);\
      }\
    else if (__builtin_abs(pr) > 99)\
      {\
        outchar('0' + __builtin_abs(pr)/100);\
	t_pr = (__builtin_abs(pr) % 100);\
        outchar('0' + t_pr/10);\
	t_pr = (__builtin_abs(t_pr) % 10);\
        outchar('0' + t_pr);\
      }\
    else if (__builtin_abs(pr) > 9)\
      {\
        outchar('0' + __builtin_abs(pr)/10);\
	t_pr = (__builtin_abs(pr) % 10);\
        outchar('0' + t_pr);\
      }\
    else\
      {\
        outchar('0' + __builtin_abs(pr));\
      }\
   } while (0)

#define OUT_DIGITS(xp) \
  do \
   { \
      int ia; \
      outchar('['); \
      outchar('d'); \
      outchar('i'); \
      outchar('g'); \
      outchar('i'); \
      outchar('t'); \
      outchar('s'); \
      outchar(':'); \
      for (ia = 0; digits[ia] != '\0'; ia++) \
	outchar(digits[ia]); \
      outchar('e'); \
      _OUT(xp); \
      outchar(']');\
   } while (0)


#define OUT(pr,val) \
  do \
   { \
      int slen = strlen(pr); \
      int ia; \
      outchar('['); \
      for (ia = 0; ia < slen; ia++) \
	   outchar(pr[ia]); \
      outchar(':'); \
      _OUT(val); \
      outchar(']');\
   } while (0)

#define OUT_PREC(pr) \
  do \
    { \
      OUT("prec",pr); \
    } while (0)

#define OUT_INDEX(idx) \
  do \
    { \
      OUT("index",idx); \
    } while (0)

#define OUT_DEFAULT_PREC(pr) \
  do \
    { \
      OUT("default_prec",pr); \
    } while (0)

#define OUT_LEN(ln) \
  do \
    { \
      OUT("len",ln); \
    } while (0)

#define OUT_SIG(sg) \
  do \
    { \
      OUT("sig",sg); \
   } while (0)


#define OUT_N(na) \
  do \
    { \
      OUT("n",na); \
   } while (0)

#define OUT_WIDTH(wd) \
  do \
    { \
      OUT("width",wd); \
   } while (0)

#define OUT_DECPT(dpt) \
  do \
    { \
      OUT("decpt",dpt); \
   } while (0)

static int pa_d128;
static int pa_d64;
static int pa_d32;

static int mod_H;
static int mod_D;
static int mod_DD;

void
__d128_va (void *mem, va_list *ap)
{
  _Decimal128 d = va_arg (*ap, _Decimal128);
  memcpy (mem, &d, sizeof (d));
}
strong_alias(__d128_va, d128_va)
hidden_def(__d128_va)

void
__d64_va (void *mem, va_list *ap)
{
  _Decimal64 d = va_arg (*ap, _Decimal64);
  memcpy (mem, &d, sizeof (d));
}
strong_alias(__d64_va, d64_va)
hidden_def(__d64_va)

void
__d32_va (void *mem, va_list *ap)
{
  _Decimal32 d = va_arg (*ap, _Decimal32);
  memcpy (mem, &d, sizeof (d));
}
strong_alias(__d32_va, d32_va)
hidden_def(__d32_va)

int
__dfp_ais (const struct printf_info *info, size_t n __attribute__ ((unused)), int *argtype, int *size)
{
  if ((info->user & mod_D) == mod_D)
    {
      argtype[0] = pa_d64;
      size[0] = sizeof (_Decimal64);
      return 1;
    }
  else if ((info->user & mod_DD) == mod_DD)
    {
      argtype[0] = pa_d128;
      size[0] = sizeof (_Decimal128);
      return 1;
    }
  else if ((info->user & mod_H) == mod_H)
    {
      argtype[0] = pa_d32;
      size[0] = sizeof (_Decimal32);
      return 1;
    }

  return -1;
}
strong_alias(__dfp_ais, dfp_ais)
hidden_def(__dfp_ais)

#define EXP_BIAS_D128 -6109
#define EXP_BIAS_D64  -368
#define EXP_BIAS_D32  -87

/* this includes the max digits in a _Decimal128, plus a bunch of formatting
 * characters.  */
#define DECIMAL_PRINTF_BUF_SIZE 65 /* ((DECIMAL128_PMAX + 14) * 2) + 1  */

/* fe_decround.c will initialize this function pointer to fe_decgetround */
int (*__printf_dfp_getround_callback)(void) = NULL;

int
__printf_dfp (FILE *fp,
	      const struct printf_info *info,
	      const void *const *args)
{
  int wide = info->wide;
  /* Counter for number of written characters.  */
  int done = 0;

  /* Locale-dependent representation of decimal point.  */
  const char *decimal;

  union { const char *mb; unsigned int wc; } decimalwc;

  char spec = tolower(info->spec);

  /* Locale-dependent thousands separator and grouping specification.  */
  const char *thousands_sep = NULL;
  wchar_t thousands_sepwc = 0;
  const char * thousands_sepmb;

  const char *grouping;

#ifdef OPTION_EGLIBC_LOCALE_CODE
  if (info->extra == 0)
    {
      decimal = nl_langinfo (__DECIMAL_POINT);
      decimalwc.mb = nl_langinfo (_NL_NUMERIC_DECIMAL_POINT_WC);
    }
  else
    {
      decimal = nl_langinfo (__MON_DECIMAL_POINT);
      if (*decimal == '\0')
	decimal = nl_langinfo (__DECIMAL_POINT);

      decimalwc.mb = nl_langinfo (_NL_MONETARY_DECIMAL_POINT_WC);
      if (decimalwc.wc == L'\0')
	decimalwc.mb = nl_langinfo (_NL_NUMERIC_DECIMAL_POINT_WC);
    }
  /* The decimal point character must not be zero.  */
  assert (*decimal != '\0');
  assert (decimalwc.wc != L'\0');
#else
  /* Hard-code values from 'C' locale.  */
  decimal = ".";
  decimalwc.wc = L'.';
#endif

#ifdef OPTION_EGLIBC_LOCALE_CODE
  if (info->group)
    {
      if (info->extra == 0)
	grouping = nl_langinfo (__GROUPING);
      else
	grouping = nl_langinfo (__MON_GROUPING);

      if (*grouping <= 0 || *grouping == CHAR_MAX)
	grouping = NULL;
      else
	{
	  /* Figure out the thousands separator character.  */
	  if (wide)
	    {
	      if (info->extra == 0)
		{
		  thousands_sepmb = nl_langinfo (_NL_NUMERIC_THOUSANDS_SEP_WC);
		  mbrtowc(&thousands_sepwc,thousands_sepmb, CHAR_MAX, NULL);
		}
	      else
		{
		  thousands_sepmb = nl_langinfo (_NL_MONETARY_THOUSANDS_SEP_WC);
		  mbrtowc(&thousands_sepwc,thousands_sepmb, CHAR_MAX, NULL);
		}
	    }
	  else
	    {
	      if (info->extra == 0)
		thousands_sep = nl_langinfo (__THOUSANDS_SEP);
	      else
		thousands_sep = nl_langinfo (__MON_THOUSANDS_SEP);
	    }

	  if ((wide && thousands_sepwc == L'\0')
	      || (! wide && *thousands_sep == '\0'))
	    grouping = NULL;
	  else if (thousands_sepwc == L'\0')
	    /* If we are printing multibyte characters and there is a
	       multibyte representation for the thousands separator,
	       we must ensure the wide character thousands separator
	       is available, even if it is fake.  */
	    thousands_sepwc = 0xfffffffe;
	}
    }
  else
    grouping = NULL;
#else
  grouping = NULL;
#endif

  /* Seriously, only touch this code if you MUST.  */

{
  char digits[DECIMAL_PRINTF_BUF_SIZE];
  int exp,       /* The exponent. */
   is_neg,       /* Is negative?  */
   is_nan,       /* Is not a number?  */
   is_inf,       /* Is infinite? */
   decpt = 2,    /* decimal point offset into digits[] */
   prec,         /* number of digits that follow the decimal point, or number of significant digits for %g */
   default_prec = 6, /* Default precision, per the C Spec.  */
   input_prec = 0,   /* Precision of the _Decimal* value.  */
   mw,           /* Mantissa Width  */
   n,            /* Current digit offset into digits[] */
   nd,           /* num_digits before the get_digits call. */
   width,        /* Width of the field */
   is_zero = 0;  /* Used in some of the output tests.  */

  digits[0] = '0'; /* need an extra digit for rounding up */

  if (info->user & mod_D)
    {
      _Decimal64 d64 = **(_Decimal64**)args[0];
      if (d64 == 0) is_zero = 1;
      nd = numdigitsd64(d64);
      __get_digits_d64 (d64, digits+1, &exp, &is_neg, &is_nan, &is_inf);
      mw = __DEC64_MANT_DIG__ + 1;
    }
  else if (info->user & mod_DD)
    {
      _Decimal128 d128 = **(_Decimal128**)args[0];
      if (d128 == 0) is_zero = 1;
      nd = numdigitsd128(d128);
      __get_digits_d128 (d128, digits+1, &exp, &is_neg, &is_nan, &is_inf);
      mw = __DEC128_MANT_DIG__ + 1;
    }
  else if (info->user & mod_H)
    {
       _Decimal32 d32 = **(_Decimal32**)args[0];
       if (d32 == 0) is_zero = 1;
       nd = numdigitsd32(d32);
       __get_digits_d32 (d32, digits+1, &exp, &is_neg, &is_nan, &is_inf);
       mw = __DEC32_MANT_DIG__ + 1;
    }
  else /* We shouldn't get here, but it is possible.  */
    return -2;

  /* The first digit is always a zero to allow rounding.  */
  n = 0;

  /* 'n' = position of first non-zero digit in the right-justified mantissa.  */
  n = mw - nd;

  /* Width and precision can not both be set or the results are undefined per
   * the C Spec.  */
  width = info->width;

  /* The user specified precision overrides the input's inherent precision.
   * This gets complicated quickly.  */
  prec = info->prec;

  if (is_nan || is_inf)
    {
      width -= 3;
      /*if (is_nan) is_neg = 0;*/
      if (is_neg || info->showsign || info->space) width--;

      if (!info->left && width > 0)
	PADN (' ', width);

      if (is_neg)
	outchar ('-');
      else if (info->showsign)
	outchar ('+');
      else if (info->space)
	outchar (' ');

      if (is_nan)
	{
	  if (isupper(info->spec))
	    { outchar ('N'); outchar ('A'); outchar ('N'); }
	  else
	    { outchar ('n'); outchar ('a'); outchar ('n'); }
	}
      else
	{
	  if (isupper(info->spec))
	    { outchar ('I'); outchar ('N'); outchar ('F'); }
	  else
	    { outchar ('i'); outchar ('n'); outchar ('f'); }
	}
      if (info->left && width > 0)
	PADN (' ', width);

	return 0;
    }

  /* The term "precision" refers to the number of significant digits right of
   * the decimal place.  Determine the implicit precision of the input value.
   * There are special rules for each of the supported flags.*/
  switch (spec)
    {
      case 'a':
	  {
	    /* The DFP spec addition for %a refers to all of the significant
	     * digits in the precision.  */
	    if (exp < 0)
	      {
		  input_prec = nd;
	      }
	    else
	      {
	        input_prec = 1;
	      }

	    /* This same check is done in two different places but it'll only
	     * effect a single pass through once.  If prec is not set it'll hit
	     * this instance.  If prec is set it'll hit the next instance.  This
	     * is because the DFP spec requires this to be run after rounding
	     * when prec < input_prec.  */
	    if (prec < 0 || prec >= input_prec)
	    {
	      /* Per the DFP specification (s,c,q), c == digits, q = exp, s ==
	       * is_neg.  */
	      if (exp >= -(nd+5) && exp <= 0)
	        {
	          prec = -exp;
	          spec = 'f';
	        }
	      else
	        {
	          prec = nd - 1;
	          spec = 'e';
	          input_prec = nd - 1;
	        }
	      }
	    break;
	  }
	case 'g':
	  {
	    int P = prec;

	    /* When the C specification refers to X as the exponent it means the
	     * exponent when the input value encoding is normalized to the form
	     * d.dddd.  This means we have to do that before we can do the goof
	     * check.
	     *
	     * e.g., 123.456E-5
	     * right-justified -> 00123456E-9
	     * normalized -> 1.23456E-4
	     *
	     * Normalize X to d.ddd... form by taking (exp) + (nd - 1)
	     *
	     * X == -4  */
	    int X = exp + (nd -1);

	    /* The C Specification also indicates how to compute P. */
	    if (prec < 0)
	      P = 6;
	    else if (prec == 0)
	      P = 1;

	    /* Straight from the specification which assumes X is exponent normalized to
	     * d.ddd... form.  */
	    if (X >= -4 && P > X)
	      {
		prec = (P - (X + 1));
		spec = 'f';
	      }
	    else
	      {
		prec = P - 1;
		spec = 'e';
		input_prec = nd - 1;
	      }
	  break;
	  }
	case 'e':
	  input_prec = nd - 1;
	  break;
	case 'f':
	  if(exp < 0 && (-exp) > default_prec)
	    /*  00123456E-7 has an input_prec of 7. */
	    input_prec = (-exp);
	  else
	    /*  01234567E-6 has an input_prec of 6. */
	    /*  00000190E6 has an input_prec of 6. */
	    /*  00000123E1 has an input_prec of 6.  */
	    /*  00000123E0 has an input_prec of 6.  */
	    input_prec = default_prec;
	  break;
    }

  /* The specs 'g' and 'a' may have already modified prec so this won't happen for
   * those cases.  */
  if (prec < 0)
    prec = default_prec;

  /* Do rounding if precision is less than the decimal type.  On hardware DFP
   * this could probably easily be done with quantize but on soft-dfp the
   * existing method would be faster.  */
  if (prec < input_prec)
    {
      int index, roundmode = 0;
      char rounddigit = '4';

      if (spec == 'f')
	/* This may force index to negative, in which case we ignore it at a
	 * later time.  */
	index = n + nd + exp + prec;
      /* Goofy special case where we round significant digits which aren't
       * right of the decimal place.  */
      else if (tolower(info->spec) == 'a')
	index = n + prec;
      else
	index = n + prec + 1;

      /* FIXME: we should check rounding mode for %a */
      if (__printf_dfp_getround_callback)
        {
          roundmode = (*__printf_dfp_getround_callback)();

	  switch (roundmode)
	    {
	      case FE_DEC_TONEAREST: rounddigit = '4'; break;
	      case FE_DEC_TOWARDZERO: rounddigit = '9'; break;
	      case FE_DEC_UPWARD: rounddigit = (is_neg ? '9' : '0'-1); break;
	      case FE_DEC_DOWNWARD: rounddigit = (is_neg ? '0'-1 : '9'); break;
	      case FE_DEC_TONEARESTFROMZERO: rounddigit = '4'; break;
	      case 5: rounddigit = '4'; break; /* nearest, ties toward zero */
	      case 6: rounddigit = '0'-1; break; /* away from zero */
	      case 7: rounddigit = '4'; break; /* round for shorter precision */
	      default: rounddigit = '4'; break;
	    }
	}

      /* If this is true then the requested precision is smaller than the
      * default and rounding is required.  If 'exp' was sufficiently negative
      * 'index' may be negative, in which case we don't need to round.  */
      if (index > 0 && index < mw && digits[index] > rounddigit) 
	do {
	  int trailzero = index+1;
	  if (digits[index] == rounddigit+1)
	    {
	      while (trailzero < mw)
		{
		  if (digits[trailzero] != '0')
		    {
		      trailzero = 0;
		      break;
		    }
		  ++trailzero;
		}
	      if (roundmode == FE_DEC_TONEAREST && trailzero &&
	        (digits[index-1] & 1) == 0) break;
	      if (roundmode == FE_DEC_UPWARD && !trailzero) break;
	      if (roundmode == FE_DEC_DOWNWARD && !trailzero) break;
	      if (roundmode == 5 && trailzero) break;
	      if (roundmode == 6 && trailzero) break;
	  }

	while (digits[--index] == '9')
	  digits[index] = '0';
	digits[index]++;
	if (index < n)
	  {
	    n--;
	    nd++;
	  }
      } while (0);
    } /* Done rounding.  */

  /* If spec == 'a' at this point it means that prec was set by the user
   * and rounding had to be considered.  The spec now requires that the
   * 'a' format presentation algorithm be calculated again.  If prec
   * wasn't set by the user then this was handled earlier and spec has already
   * been set to either 'e' or 'f'.  */
  if (spec == 'a')
    {
      int old_exp = exp;

      /* The goofy DFP specification requires that we now assume that after
       * rounding the digits are right justified and truncated and the
       * algorithm recomputed using the new values for nd and exp, e.g.,
       *
       * 00654300E-2 with %.1Hf -> 00000007E3.  */

       exp = nd + exp - prec;
       nd = prec;

      /* Per the DFP specification (s,c,q), c == digits, q = exp, s ==
      * is_neg.  */
      if (exp >= -(nd+5) && exp <= 0)
	{
	  prec = -exp;
	  spec = 'f';
	}
      else
	{
	  prec = nd - 1;
	  if (prec < 0) prec = 0;
	  spec = 'e';
	  input_prec = nd - 1;
	  /* Return exp to the original value because the 'e' case below will
	   * recompute it.  */
	  exp = old_exp;
	}
	/* spec will have been changed to 'e' or 'f' at this point, so determine
	* the decimal point now.  */
    }

  /* Calculate decimal point, adjust prec and exp if necessary.
   * By this point everything should be represented as either %e or %f.  */
  if (spec == 'f')
    {
      if (exp < 0)
	decpt = exp + nd + n;
      else if (is_zero)
	decpt = n + 1;
      else
	decpt = n + nd + exp;
    }
  else if (spec == 'e')
    {
      decpt = n + 1;
      exp = mw + exp - decpt;
    }

  /* Remove trailing zeroes for %g */
  if (tolower(info->spec) == 'g' && !info->alt)
    {
      while (prec > 0 && decpt+prec > mw) prec--;
      while (prec > 0 && digits[decpt+prec-1] == '0') prec--;
    }

  /* Remove trailing zeroes for %a, but only if they are not significant.  */
  if (tolower(info->spec) == 'a')
    {
      while (prec > 0 && decpt+prec > mw) prec--;
      while (prec > 0 && decpt+prec > n+nd && digits[decpt+prec-1] == '0') prec--;
    }

  /* Digits to the left of the decimal pt. */
  if (n < decpt)
    {
      width -= decpt - n;
      if (grouping) width -= (decpt-n)/3;
    }
  else width--;  /* none to the left of the decimal point */

  /* Digits to the right of the decimal pt. */
  if (prec > 0) width -= 1 + prec;
  else if (info->alt) width -= 1;

  if (spec != 'f')
    {
      width -= 3;
      if (0!=(exp/10) || spec!='a') --width;
      if (0!=(exp/100)) --width;
      if (0!=(exp/1000)) --width;
    }

  if (is_neg || info->showsign || info->space) width--;

  if (!info->left && info->pad != '0' && width > 0)
    PADN (info->pad, width);

  if (is_neg)
    outchar ('-');
  else if (info->showsign)
    outchar ('+');
  else if (info->space)
    outchar (' ');

  if (!info->left && info->pad == '0' && width > 0)
    PADN ('0', width);

  /* Print zero, decimal point and leading zeroes if needed */
  if (decpt <= n)
    {
      n = decpt;
      outchar ('0');
      if (n < 0)
	{
	  outchar (wide ? decimalwc.wc : *decimal);
	  while (n < 0 && n < decpt + prec)
	    {
	      outchar ('0');
	      n++;
	    }
	}
    }

  /* Print the digits.  If decpt exceeds mw then we know that
   * they're simply trailing zeros and we don't need to display them.  */
  while (n < mw && n < decpt + prec)
    {
      if (n == decpt)
      {
	outchar (wide ? decimalwc.wc : *decimal);
      }
      else if (grouping && n < decpt && (decpt-n)%3 == 0)
	outchar (wide ? thousands_sepwc : *thousands_sep);
      outchar (digits[n]);
      n++;
    }

  /* print trailing zeroes */
  while (n < decpt + prec)
    {
      if (n == decpt)
	outchar (wide ? decimalwc.wc : *decimal);
      else if (grouping && n < decpt && (decpt-n)%3 == 0)
	outchar (wide ? thousands_sepwc : *thousands_sep);
      outchar ('0');
      n++;
    }

  /* print decimal point, if needed */
  if (n == decpt && info->alt) outchar (wide ? decimalwc.wc : *decimal);

  /* The C spec says that for %e, if the value is zero the exponent is zero.
   * This isn't true for the DFP spec for %a so make sure to check info->spec
   * and not spec since it could have promoted 'a' to 'e'.  */
  if(spec == 'e' && (tolower(info->spec) != 'a' && is_zero))
    exp = 0;

  /* Don't display the exponent part for 'f' because it is never used and don't
   * do it for 'g' if the value is zero.  */
  if (spec != 'f' && !((tolower(info->spec) == 'g') && is_zero))
    {
      outchar (isupper(info->spec) ? 'E' : 'e');
      if (exp < 0)
	{ outchar ('-'); n = -exp; }
      else
	{ outchar ('+'); n = exp; }
      if (n >= 1000) outchar ('0'+((n/1000)%10));
      if (n >= 100) outchar ('0'+((n/100)%10));
      if (n >= 10 || (tolower(info->spec) != 'a')) outchar ('0'+((n/10)%10));
      outchar ('0'+(n%10));
    }

  if (info->left && width > 0)
    PADN (info->pad, width);
} /* Done output block.  */

   return 0;
}
strong_alias (__printf_dfp, printf_dfp)
hidden_def (__printf_dfp)

int __register_printf_dfp (void)
{
  pa_d128 = register_printf_type (d128_va);
  pa_d32 = register_printf_type (d32_va);
  pa_d64 = register_printf_type (d64_va);

  mod_DD = register_printf_modifier (L"DD");
  mod_H = register_printf_modifier (L"H");
  mod_D = register_printf_modifier (L"D");

  register_printf_specifier ('f', printf_dfp, dfp_ais);
  register_printf_specifier ('F', printf_dfp, dfp_ais);
  register_printf_specifier ('e', printf_dfp, dfp_ais);
  register_printf_specifier ('E', printf_dfp, dfp_ais);
  register_printf_specifier ('g', printf_dfp, dfp_ais);
  register_printf_specifier ('G', printf_dfp, dfp_ais);
  register_printf_specifier ('a', printf_dfp, dfp_ais);
  register_printf_specifier ('A', printf_dfp, dfp_ais);

  return 0;
}
strong_alias (__register_printf_dfp, register_printf_dfp)
hidden_def (__register_printf_dfp)
