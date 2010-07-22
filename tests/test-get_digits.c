/* Test the internal facility to get_digits_d{32|64|128].

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
#define __STDC_WANT_DEC_FP__ 1
#endif

#include <fenv.h>
#include <float.h>
#include <dfp.h>
#include <stdio.h>
#include <wchar.h> /* This should pick up the libdfp wchar in dfp/wchar.h.  */

#include "scaffold.c"

#include <get_digits.h>

/* We're going to be comparing fields so we need to extract the data.  This is a
 *  * sneaky way to get around the fact that get_digits_d* isn't exported from
 *   * libdfp.  */
#include "../sysdeps/dpd/dpd-private.c"

#define DECIMAL_PRINTF_BUF_SIZE 65 /* ((DECIMAL128_PMAX + 14) * 2) + 1 */

/* This may be useful for debugging.  */
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

#include "decode.h"

/* Inspired by GLIBC stdio-common/tfformat.c  */
typedef struct{
  int line;
  _Decimal32 d;
  const char *expect;
  const char *format;
} d32_type;

d32_type printf_d32s[] =
{

  {__LINE__, 0.00000E-101DF, "+0000000e-101", "%Ha"},
  {__LINE__, 0e10DF, "+0000000e+10", "%Ha"},
  {__LINE__, 0e-6DF, "+0000000e-6", "%Ha"},
  {__LINE__, 0.00000E-95DF, "+0000000e-100", "%Ha"},
  {0,0,0,0 }
};

int main (void)
{
  d32_type *d32ptr;

  char digits[DECIMAL_PRINTF_BUF_SIZE];
  int exp, /* the exponent */
   is_neg, /* is negative */
   is_nan, /* is not a number */
   is_inf; /* is infinite */

  for (d32ptr = printf_d32s; d32ptr->line; d32ptr++)
    {
      fprintf(stdout,"__get_digits_d32(%HeDF) in: %s:%d\n", d32ptr->d,__FILE__,__LINE__-1);
      memset(&digits[0],'\0',DECIMAL_PRINTF_BUF_SIZE);
      __get_digits_d32 (d32ptr->d, digits+1, &exp, &is_neg, &is_nan, &is_inf);
      OUT_DIGITS();
      _SC_P(__FILE__,d32ptr->line, d32ptr->expect,&digits[0]);
    }

  _REPORT();

  /* fail comes from scaffold.c  */
  return fail;
}
