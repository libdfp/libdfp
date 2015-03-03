/* Test the internal facility to get_digits_d{32|64|128].

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
#define __STDC_WANT_DEC_FP__ 1
#endif

#include <fenv.h>
#include <float.h>
#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>

#include "scaffold.c"

#include <get_digits.h>

/* We're going to be comparing fields so we need to extract the data.  This is a
 * sneaky way to get around the fact that get_digits_d* isn't exported from
 * libdfp.  */
#ifdef __DECIMAL_BID_FORMAT__
#include "../sysdeps/bid/bid-private.c"
#else
#include "../sysdeps/dpd/dpd-private.c"
#endif


#include "decode.h"

typedef struct
{
  int line;
  _Decimal32 d;
  const char *expect;
  const char *format;
} d32_type;

static const
d32_type d32[] = {
  {__LINE__, 0.00000E-101DF, "+0000000e-101", "%Ha"},
  {__LINE__, 0e10DF, "+0000000e+10", "%Ha"},
  {__LINE__, 0e-6DF, "+0000000e-6", "%Ha"},
  {__LINE__, 0.00000E-95DF, "+0000000e-100", "%Ha"},

  {__LINE__, 12e-12DF, "+0000012e-12", "%Ha"},
  {__LINE__, -659e-9DF, "-0000659e-9", "%Ha"},
  {__LINE__, 10221e12DF, "+0010221e+12", "%Ha"},
  {__LINE__, 16116736E2DF, "+1611674e+3", "%Ha"},
  {__LINE__, 201432679e1DF, "+2014327e+3", "%Ha"},
  {__LINE__, 208055501e1DF, "+2080555e+3", "%Ha"},

  {__LINE__, 0.0004E-4DF, "+0000004e-8", "%s"},
  {__LINE__, 0.0004E-0DF, "+0000004e-4", "%s"},
  {__LINE__, 0.0004E-2DF, "+0000004e-6", "%s"},
  {__LINE__, 1.0E+2DF, "+0000010e+1", "%s"},
  {__LINE__, 0.9999999E-91DF, "+9999999e-98", "%s"},

  {__LINE__, 0.9999999E-92DF, "+9999999e-99", "%s"},
  {__LINE__, 0.9999999E-93DF, "+9999999e-100", "%s"},
  {__LINE__, 0.9999999E-94DF, "+9999999e-101", "%s"},
  {__LINE__, 0.9999999E-95DF, "+1000000e-101", "%s"},
  {__LINE__, 0.0000010E-95DF, "+0000001e-101", "%s"},

  {__LINE__, 0.0000100E-95DF, "+0000010e-101", "%s"},
  {__LINE__, 0.0001000E-95DF, "+0000100e-101", "%s"},
  {__LINE__, 0.0010000E-95DF, "+0001000e-101", "%s"},
  {__LINE__, 0.0100000E-95DF, "+0010000e-101", "%s"},
  {__LINE__, 0.1000000E-95DF, "+0100000e-101", "%s"},

  {__LINE__, 1.0000000E-95DF, "+1000000e-101", "%s"},
  {__LINE__, 10.0000000E-95DF, "+1000000e-100", "%s"},
  {__LINE__, 10.0000000E-96DF, "+1000000e-101", "%s"},
};
static const int d32_s = sizeof (d32) / sizeof (d32[0]);

typedef struct
{
  int line;
  _Decimal64 d;
  const char *expect;
  const char *format;
} d64_type;

d64_type d64[] = {
  {__LINE__, -853894835028358388E+1DD, "-8538948350283584e+3", "%s"},

  {__LINE__, 1.0E+2DD, "+0000000000000010e+1", "%s"},
  {__LINE__, 123456789.123456E+300DD, "+0123456789123456e+294", "%s"},
  {__LINE__, 2.0E-2DD, "+0000000000000020e-3", "%s"},
  {__LINE__, -2.0E-2DD, "-0000000000000020e-3", "%s"},
  {__LINE__, 0.0000000000000001E-365DD, "+0000000000000001e-381", "%s"},
  {__LINE__, 0.0000000000000001E-366DD, "+0000000000000001e-382", "%s"},
  {__LINE__, 0.0000000000000001E-367DD, "+0000000000000001e-383", "%s"},
  {__LINE__, 0.000000000000001E-368DD, "+0000000000000001e-383", "%s"},
  {__LINE__, 0.00000000000001E-369DD, "+0000000000000001e-383", "%s"},
  {__LINE__, 0.1E-382DD, "+0000000000000001e-383", "%s"},
  {__LINE__, 1.E-383DD, "+0000000000000001e-383", "%s"},
};
static const int d64_s = sizeof (d64) / sizeof (d64[0]);


typedef struct
{
  int line;
  _Decimal128 d;
  const char *expect;
  const char *format;
} d128_type;

d128_type d128[] = {
  {__LINE__, 17264850929837472839.48234675928374E+271DL,
   "+1726485092983747283948234675928374e+257", "%s"},
  {__LINE__, 9000000000000000001.83273827374E-212DL,
   "+0000900000000000000000183273827374e-223", "%s"},
  {__LINE__, 12000000000000000005.00000002000001E-157DL,
   "+1200000000000000000500000002000001e-171", "%s"},
  {__LINE__, -1000200300040050607008090.01203456E+300DL,
   "-0100020030004005060700809001203456e+292", "%s"},
  {__LINE__, 2537781477129814100906124727369014E-4DL,
   "+2537781477129814100906124727369014e-4", "%s"},
  {__LINE__, -2456651838715207419210335722224950E-42DL,
   "-2456651838715207419210335722224950e-42", "%s"},
  {__LINE__, 31903802797912746463249044.0008815401650E+13DL,
   "+3190380279791274646324904400088154e+5", "%s"},
  {__LINE__, 0.000000000000000329671851945406791615720896491058158258E-16DL,
   "+3296718519454067916157208964910582e-65", "%s"},

  {__LINE__, 1.0E+2DL, "+0000000000000000000000000000000010e+1", "%s"},
  {__LINE__, 123456789.123456E+300DL,
   "+0000000000000000000123456789123456e+294", "%s"},
  {__LINE__, 2.0E-2DL, "+0000000000000000000000000000000020e-3", "%s"},
  {__LINE__, -2.0E-2DL, "-0000000000000000000000000000000020e-3", "%s"},
  {__LINE__, 0.0000000000000001E-365DL,
   "+0000000000000000000000000000000001e-381", "%s"},
  {__LINE__, 0.0000000000000001E-366DL,
   "+0000000000000000000000000000000001e-382", "%s"},
  {__LINE__, 0.0000000000000001E-367DL,
   "+0000000000000000000000000000000001e-383", "%s"},
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


int
main (void)
{
#define DECIMAL_PRINTF_BUF_SIZE 65	/* ((DECIMAL128_PMAX + 14) * 2) + 1 */
  char digits[DECIMAL_PRINTF_BUF_SIZE];
  int exp;			/* the exponent */
  int is_neg;			/* is negative */
  int is_nan;			/* is not a number */
  int is_inf;			/* is infinite */
  int i;

  for (i=0; i<d32_s; ++i)
    {
      fprintf (stdout, "__get_digits_d32(%HeDF) in: %s:%d\n", d32[i].d,
	       __FILE__, __LINE__ - 1);
      memset (&digits[0], '\0', DECIMAL_PRINTF_BUF_SIZE);
      __get_digits_d32 (d32[i].d, digits + 1, &exp, &is_neg, &is_nan,
			&is_inf);
      out_digits (digits, is_neg, exp);
      _SC_P (__FILE__, d32[i].line, d32[i].expect, &digits[0]);
    }

  for (i=0; i<d64_s; ++i)
    {
      fprintf (stdout, "__get_digits_d64(%DeDD) in: %s:%d\n", d64[i].d,
	       __FILE__, __LINE__ - 1);
      __get_digits_d64 (d64[i].d, digits + 1, &exp, &is_neg, &is_nan,
			&is_inf);
      out_digits (digits, is_neg, exp);
      _SC_P (__FILE__, d64[i].line, d64[i].expect, &digits[0]);
    }

  for (i=0; i<d128_s; ++i)
    {
      fprintf (stdout, "__get_digits_d128(%DDeDL) in: %s:%d\n", d128[i].d,
	       __FILE__, __LINE__ - 1);
      __get_digits_d128 (d128[i].d, digits + 1, &exp, &is_neg, &is_nan,
			 &is_inf);
      out_digits (digits, is_neg, exp);
      _SC_P (__FILE__, d128[i].line, d128[i].expect, &digits[0]);
    }


  _REPORT ();

  /* fail comes from scaffold.c  */
  return fail;
}
