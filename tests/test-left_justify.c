/* Unit test the internal left_justifyd[32|64|128]() functions.

   Copyright (C) 2010-2015 Free Software Foundation, Inc.

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
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <get_digits.h>

#define _DECIMAL_SIZE 32
#define DEC_TYPE _Decimal32
#include <numdigits.h>
#undef _DECIMAL_SIZE
#undef DEC_TYPE

#define _DECIMAL_SIZE 64
#define DEC_TYPE _Decimal64
#include <numdigits.h>
#undef _DECIMAL_SIZE
#undef DEC_TYPE

#define _DECIMAL_SIZE 128
#define DEC_TYPE _Decimal128
#include <numdigits.h>
#undef _DECIMAL_SIZE
#undef DEC_TYPE

#include "scaffold.c" /* Pick up the _SC_P(x,y,fmt) macro.  */

/* We're going to be comparing fields so we need to extract the data.  This is a
 * sneaky way to get around the fact that get_digits_d* isn't exported from
 * libdfp.  */
#ifdef __DECIMAL_BID_FORMAT__
# include "sysdeps/bid/bid-private.c"
#else
# include "sysdeps/dpd/dpd-private.c"
#endif

typedef struct
{
  int line;
  _Decimal32 d;
  const char *expect;
  const char *format;
} d32_type;

static const
d32_type d32[] =
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

  {__LINE__, 1E-95DF, "+1000000e-101", "%s"},
  {__LINE__, 0.000001E-95DF, "+0000001e-101", "%s"},
  {__LINE__, 0.000010E-96DF, "+0000001e-101", "%s"},
  {__LINE__, 0.001000E-98DF, "+0000001e-101", "%s"},
  {__LINE__, 0.010000E-99DF, "+0000001e-101", "%s"},
  {__LINE__, 0.100000E-100DF, "+0000001e-101", "%s"},

  {__LINE__, 1.0000000E-95DF, "+1000000e-101", "%s"},
  {__LINE__, 10.0000000E-95DF, "+1000000e-100", "%s"},
  /* Notice 1000000e-101 is the answer.  */
  {__LINE__, 10.0000000E-96DF,"+1000000e-101", "%s"},

  /* __get_digits_d64 returns 0 for a NaN data.  */
  {__LINE__, NAN, "+0000000e-101", "%s"},
};
static const int d32_s = sizeof (d32) / sizeof (d32[0]);

/* Inspired by GLIBC stdio-common/tfformat.c  */
typedef struct{
  int line;
  _Decimal64 d;
  const char *expect;
  const char *format;
} d64_type;

static const
d64_type d64[] =
{
  {__LINE__, 1.0E+2DD, "+1000000000000000e-13", "%s"},
  {__LINE__, 123456789.123456E+300DD, "+1234567891234560e+293", "%s"},
  {__LINE__, 2.0E-2DD, "+2000000000000000e-17", "%s"},
  {__LINE__, -2.0E-2DD, "-2000000000000000e-17", "%s"},
  {__LINE__, 0.0000000000000001E-365DD, "+1000000000000000e-396", "%s"},
  {__LINE__, 0.0000000000000001E-366DD, "+1000000000000000e-397", "%s"},

  /* Left justify is limited by the minimum exponent, this is why 1 is not the
     first mantissa number.  */
  {__LINE__, 0.0000100E-383DD, "+0000010000000000e-398", "%s"},
  {__LINE__, 0.0001000E-383DD, "+0000100000000000e-398", "%s"},
  {__LINE__, 0.0010000E-383DD, "+0001000000000000e-398", "%s"},
  {__LINE__, 0.0100000E-383DD, "+0010000000000000e-398", "%s"},

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

  /* Subnormal min test.  */
  {__LINE__, 0.000000000000001E-383DD, "+0000000000000001e-398", "%s"},

  /* __get_digits_d64 returns 0 for a NaN data.  */
  {__LINE__, NAN, "+0000000000000000e-398", "%s"},
};
static const int d64_s = sizeof (d64) / sizeof (d64[0]);


typedef struct{
  int line;
  _Decimal128 d;
  const char *expect;
  const char *format;
} d128_type;

static const
d128_type d128[] =
{
  {__LINE__, 1.0E+2DL, "+1000000000000000000000000000000000e-31", "%s"},
  {__LINE__, 123456789.123456E+300DL, "+1234567891234560000000000000000000e+275", "%s"},
  {__LINE__, 2.0E-2DL, "+2000000000000000000000000000000000e-35", "%s"},
  {__LINE__, -2.0E-2DL, "-2000000000000000000000000000000000e-35", "%s"},
  {__LINE__, 0.0000000000000001E-365DL, "+1000000000000000000000000000000000e-414", "%s"},
  {__LINE__, 0.0000000000000001E-366DL, "+1000000000000000000000000000000000e-415", "%s"},
  {__LINE__, 0.0000000000000001E-367DL, "+1000000000000000000000000000000000e-416", "%s"},

  {__LINE__, 1E-6143DL, "+1000000000000000000000000000000000e-6176", "%s"},
  {__LINE__, 0.0000000000000001E-6127DL, "+1000000000000000000000000000000000e-6176", "%s"},

  {__LINE__, 0.0000000000000001E-6140DL, "+0000000000000100000000000000000000e-6176", "%s"},
  {__LINE__, 0.0000000000000100E-6143DL, "+0000000000000010000000000000000000e-6176", "%s"},
  {__LINE__, 0.0000000000001000E-6143DL, "+0000000000000100000000000000000000e-6176", "%s"},
  {__LINE__, 0.0000000000000001E-6143DL, "+0000000000000000100000000000000000e-6176", "%s"},
  {__LINE__, 0.0000000000000001E-6160DL, "+0000000000000000000000000000000001e-6176", "%s"},

  /* Subnormal min test.  */
  {__LINE__, 0.000000000000000000000000000000001E-6143DL, "+0000000000000000000000000000000001e-6176", "%s"},

  /* __get_digits_d128 returns 0 for a NaN data.  */
  {__LINE__, NAN, "+0000000000000000000000000000000000e-6176", "%s"},

};
static const int d128_s = sizeof (d128) / sizeof (d128[0]);


static inline void
out_digits (char *digits, int is_neg, int exp)
{
  int i;

  if (is_neg)
    digits[0] = '-';
  else
    digits[0] = '+';
  i = 1;
  while (digits[i] != '\0')
    i++;
  digits[i++] = 'e';
  if (exp < 0)
    digits[i++] = '-';
  else
    digits[i++] = '+';

  if (abs (exp) >= 1000)
    digits[i++] = '0' + ((abs (exp)/1000) % 10);
  if (abs(exp) >= 100)
    digits[i++] = '0' + ((abs (exp)/100) % 10);
  if (abs(exp) >= 10)
    digits[i++] = '0' + ((abs (exp)/10) % 10);
  digits[i++] = '0' + (abs (exp) % 10);
  digits[i] = '\0';
}


int main (void)
{
#define DECIMAL_PRINTF_BUF_SIZE 65 /* ((DECIMAL128_PMAX + 14) * 2) + 1 */
  char digits[DECIMAL_PRINTF_BUF_SIZE];
  int exp;    /* the exponent */
  int is_neg; /* is negative */
  int is_nan; /* is not a number */
  int is_inf; /* is infinite */
  int i;

  for (i=0; i<d32_s; ++i)
    {
      _Decimal32 ret = left_justifyd32 (d32[i].d);
      __get_digits_d32 (ret, digits+1, &exp, &is_neg, &is_nan, &is_inf);
      fprintf (stdout, "left_justifyd32 (%HeDF) in: %s:%d\n", d32[i].d,
		       __FILE__, __LINE__-1);
      out_digits (digits, is_neg, exp);
      _SC_P (__FILE__, d32[i].line, d32[i].expect, &digits[0]);
    }

  for (i=0; i<d64_s; ++i)
    {
      _Decimal64 ret = left_justifyd64 (d64[i].d);
      __get_digits_d64 (ret, digits+1, &exp, &is_neg, &is_nan, &is_inf);
      fprintf(stdout, "left_justifyd64 (%DeDD) in: %s:%d\n", d64[i].d,
		       __FILE__, __LINE__ -1);
      out_digits (digits, is_neg, exp);
      _SC_P(__FILE__, d64[i].line, d64[i].expect, &digits[0]);
    }

  for (i=0; i<d128_s; ++i)
    {
      _Decimal128 ret = left_justifyd128(d128[i].d);
      __get_digits_d128 (ret, digits+1, &exp, &is_neg, &is_nan, &is_inf);
      fprintf(stdout, "left_justifyd128 (%DDeDL) in: %s:%d\n", d128[i].d,
		      __FILE__, __LINE__-1);
      out_digits (digits, is_neg, exp);
      _SC_P(__FILE__, d128[i].line, d128[i].expect,&digits[0]);
    }

  _REPORT();

  /* fail comes from scaffold.c  */
  return fail;
}
