/* Unit test the internal left_justifyd[32|64|128]() functions.

   Copyright (C) 2010 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Ryan S. Arnold <rsa@us.ibm.com>

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

#ifndef __STDC_WANT_DEC_FP__
#define __STDC_WANT_DEC_FP__
#endif

#include <float.h> /* DEC_NAN definition.  */
#include <dfp.h>
#include <stdio.h>
#include <math.h>

#include <get_digits.h>
#define _DECIMAL_SIZE 32
#define DEC_TYPE _Decimal32
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
#define _DECIMAL_SIZE 128
#define DEC_TYPE _Decimal128
#include <numdigits.h>

#include "scaffold.c" /* Pick up the _SC_P(x,y,fmt) macro.  */

/* We're going to be comparing fields so we need to extract the data.  This is a
 * sneaky way to get around the fact that get_digits_d* isn't exported from
 * libdfp.  */
#include "../sysdeps/dpd/dpd-private.c"

/* Inspired by GLIBC stdio-common/tfformat.c  */
typedef struct{
  int line;
  _Decimal32 d;
  const char *expect;
  const char *format;
} d32_type;

d32_type printf_d32s[] =
{
  {__LINE__, 0.0004E-4DF, "+4000000e-14", "%s"},
  {__LINE__, 0.0004E-0DF, "+4000000e-10", "%s"},
  {__LINE__, 0.0004E-2DF, "+4000000e-12", "%s"},
  {__LINE__, 1.0E+2DF, "+1000000e-4", "%s"},
  {__LINE__, 0.9999999E-91DF, "+9999999e-98", "%s"},

  {__LINE__, 0.9999999E-92DF, "+9999999e-99", "%s"},
  {__LINE__, 0.9999999E-93DF, "+9999999e-100", "%s"},
  {__LINE__, 0.9999999E-94DF, "+9999999e-101", "%s"},
  /* Notice 1000000e-101 is the answer.  */
  {__LINE__, 0.9999999E-95DF, "+1000000e-101", "%s"},
  {__LINE__, 0.0000010E-95DF, "+0000001e-101", "%s"},

  {__LINE__, 0.0000100E-95DF, "+0000010e-101", "%s"},
  {__LINE__, 0.0001000E-95DF, "+0000100e-101", "%s"},
  {__LINE__, 0.0010000E-95DF, "+0001000e-101", "%s"},
  {__LINE__, 0.0100000E-95DF, "+0010000e-101", "%s"},
  {__LINE__, 0.1000000E-95DF, "+0100000e-101", "%s"},

  {__LINE__, 1.0000000E-95DF, "+1000000e-101", "%s"},
  {__LINE__, 10.0000000E-95DF, "+1000000e-100", "%s"},
  /* Notice 1000000e-101 is the answer.  */
  {__LINE__, 10.0000000E-96DF,"+1000000e-101", "%s"},
  {0,0,0,0 }
};

/* Inspired by GLIBC stdio-common/tfformat.c  */
typedef struct{
  int line;
  _Decimal64 d;
  const char *expect;
  const char *format;
} d64_type;

d64_type printf_d64s[] =
{
  {__LINE__, 1.0E+2DD, "+1000000000000000e-13", "%s"},
  {__LINE__, 123456789.123456E+300DD, "+1234567891234560e+293", "%s"},
  {__LINE__, 2.0E-2DD, "+2000000000000000e-17", "%s"},
  {__LINE__, -2.0E-2DD, "-2000000000000000e-17", "%s"},
  {__LINE__, 0.0000000000000001E-365DD, "+1000000000000000e-396", "%s"},
  {__LINE__, 0.0000000000000001E-366DD, "+1000000000000000e-397", "%s"},
  /* The minimum exponent we can left-justify if the mantissa is full.  */
  {__LINE__, 0.0000000000000001E-367DD, "+1000000000000000e-398", "%s"},
  /* Notice the 15 digit mantissa.  */
  {__LINE__, 0.000000000000001E-368DD, "+1000000000000000e-398", "%s"},
  /* Notice the 14 digit mantissa.  */
  {__LINE__, 0.00000000000001E-369DD, "+1000000000000000e-398", "%s"},
  /* Notice the 1 digit mantissa.  */
  {__LINE__, 0.1E-382DD, "+1000000000000000e-398", "%s"},
  /* Notice the 1 digit mantissa.  */
  {__LINE__, 1.E-383DD, "+1000000000000000e-398", "%s"},
  {0,0,0,0 }
};

/* Inspired by GLIBC stdio-common/tfformat.c  */
typedef struct{
  int line;
  _Decimal128 d;
  const char *expect;
  const char *format;
} d128_type;

d128_type printf_d128s[] =
{
  {__LINE__, 1.0E+2DL, "+1000000000000000000000000000000000e-31", "%s"},
  {__LINE__, 123456789.123456E+300DL, "+1234567891234560000000000000000000e+275", "%s"},
  {__LINE__, 2.0E-2DL, "+2000000000000000000000000000000000e-35", "%s"},
  {__LINE__, -2.0E-2DL, "-2000000000000000000000000000000000e-35", "%s"},
  {__LINE__, 0.0000000000000001E-365DL, "+1000000000000000000000000000000000e-414", "%s"},
  {__LINE__, 0.0000000000000001E-366DL, "+1000000000000000000000000000000000e-415", "%s"},
  {__LINE__, 0.0000000000000001E-367DL, "+1000000000000000000000000000000000e-416", "%s"},
  {0,0,0,0 }
};


#define DECIMAL_PRINTF_BUF_SIZE 65 /* ((DECIMAL128_PMAX + 14) * 2) + 1 */

#define OUT_DIGITS() \
  do \
    { \
      int i; \
      if(is_neg) \
	digits[0] = '-'; \
      else \
	digits[0] = '+'; \
      i = 1; \
      while (digits[i] != '\0') \
        i++; \
      digits[i++] = 'e'; \
      if (exp < 0) \
	  digits[i++] = '-'; \
      else \
	digits[i++] = '+'; \
      if (__builtin_abs(exp) >= 1000) \
	      digits[i++] = '0'+((__builtin_abs(exp)/1000)%10); \
      if (__builtin_abs(exp) >= 100) \
	      digits[i++] = '0'+((__builtin_abs(exp)/100)%10); \
      if (__builtin_abs(exp) >= 10) \
	      digits[i++] = '0'+((__builtin_abs(exp)/10)%10); \
      digits[i++] = '0'+(__builtin_abs(exp)%10); \
      digits[i] = '\0'; \
    } while(0);


int main (void)
{
  d32_type *d32ptr;
  d64_type *d64ptr;
  d128_type *d128ptr;

  char digits[DECIMAL_PRINTF_BUF_SIZE];
  int exp, /* the exponent */
   is_neg, /* is negative */
   is_nan, /* is not a number */
   is_inf; /* is infinite */

  for (d32ptr = printf_d32s; d32ptr->line; d32ptr++)
    {
      _Decimal32 d32 = left_justifyd32(d32ptr->d);
      __get_digits_d32 (d32, digits+1, &exp, &is_neg, &is_nan, &is_inf);
      OUT_DIGITS();
      fprintf(stdout,"left_justifyd32(%HeDF) in: %s:%d\n", d32ptr->d,__FILE__,__LINE__-1);
      _SC_P(__FILE__,d32ptr->line, d32ptr->expect,&digits[0]);
    }

  for (d64ptr = printf_d64s; d64ptr->line; d64ptr++)
    {
      _Decimal64 d64 = left_justifyd64(d64ptr->d);
      __get_digits_d64 (d64, digits+1, &exp, &is_neg, &is_nan, &is_inf);
      OUT_DIGITS();
      fprintf(stdout,"left_justifyd64(%DeDD) in: %s:%d\n", d64ptr->d,__FILE__,__LINE__-1);
      _SC_P(__FILE__,d64ptr->line, d64ptr->expect,&digits[0]);
    }

  for (d128ptr = printf_d128s; d128ptr->line; d128ptr++)
    {
      _Decimal128 d128 = left_justifyd128(d128ptr->d);
      __get_digits_d128 (d128, digits+1, &exp, &is_neg, &is_nan, &is_inf);
      OUT_DIGITS();
      fprintf(stdout,"left_justifyd128(%DDeDL) in: %s:%d\n", d128ptr->d,__FILE__,__LINE__-1);
      _SC_P(__FILE__,d128ptr->line, d128ptr->expect,&digits[0]);
    }

  _REPORT();

  /* fail comes from scaffold.c  */
  return fail;
}


