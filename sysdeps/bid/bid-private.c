/* Data types for BID format output.

   Copyright (C) 2014-2015 Free Software Foundation, Inc.

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

   Please see dfp/COPYING.txt for more information.  */

#include <stdio.h>
#include <string.h>

#include "bid-private.h"
#include "get_digits.h"

const struct ieee754r_c_field c_decoder[32] = {
  {0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 0, 2}, {0, 0, 0, 3},  /* 00000-00011 */
  {0, 0, 0, 4}, {0, 0, 0, 5}, {0, 0, 0, 6}, {0, 0, 0, 7},  /* 00100-00111 */
  {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 1, 2}, {0, 0, 1, 3},  /* 01000-01011 */
  {0, 0, 1, 4}, {0, 0, 1, 5}, {0, 0, 1, 6}, {0, 0, 1, 7},  /* 01100-01111 */
  {0, 0, 2, 0}, {0, 0, 2, 1}, {0, 0, 2, 2}, {0, 0, 2, 3},  /* 10000-10011 */
  {0, 0, 2, 4}, {0, 0, 2, 5}, {0, 0, 2, 5}, {0, 0, 2, 7},  /* 10100-10111 */
  {0, 0, 0, 8}, {0, 0, 0, 9}, {0, 0, 1, 8}, {0, 0, 1, 9},  /* 11000-11011 */
  {0, 0, 2, 8}, {0, 0, 2, 9}, {0, 1, 0, 0}, {1, 0, 0, 0}   /* 11100-11111 */
};

/* Return the sign, exponent and coefficient from a _Decimal32 encoded in
   BID format.  The encoding is defined as

   (−1)^s × 10^(E−Bp) × c
   sign_p    exp_p     str  */
void
__get_digits_d32 (_Decimal32 x, char *str, int *exp_p, int *sign_p,
		  int *nan_p, int *inf_p)
{
  unsigned int result;
  int i, size, exp = 0;

  struct ieee754r_c_field c_f;
  union ieee754r_Decimal32 d;

  d.sd = x;
  c_f = c_decoder[d.nan.nan];

  if (c_f.is_nan)
    {
      exp = 0;
      result = 0;
    }
  else if (d.divided.c != 3)
    {
      exp = (d.si >> 23) & 0xFF;    // 8 bits.
      result = d.si & 0x7FFFFFUl;   // 23 bits.
    }
  else
    {
      exp = (d.si >> 21) & 0xFF;
      result  = 4 << 21;
      result |= d.si & 0x1FFFFFUL;  // 21 bits.
    }

  if (result >= DECIMAL32_TH)
    result = 0;

  exp -= DECIMAL32_Bias;

  sprintf (str, "%u", result);
  size = (7 - strlen (str));
  for (i = 0; i < size; i++)
    str[i] = '0';
  sprintf (str + i, "%u", result);
  str[7] = '\0';

  if (sign_p)
    *sign_p = d.divided.negative;
  if (exp_p)
    *exp_p = exp;
  if (nan_p)
    *nan_p = c_f.is_nan;
  if (inf_p)
    *inf_p = c_f.is_inf;
}

/* Return the sign, exponent and coefficient from a _Decimal64 encoded in
   BID format.  The encoding is defined as

   (−1)^s × 10^(E−Bp) × c
   sign_p    exp_p     str  */
void
__get_digits_d64 (_Decimal64 x, char *str, int *exp_p, int *sign_p,
		  int *nan_p, int *inf_p)
{
  unsigned long long int result;
  int i, size, exp = 0;

  struct ieee754r_c_field c_f;
  union ieee754r_Decimal64 d;

  d.dd = x;
  c_f = c_decoder[d.nan.nan];

  if (c_f.is_nan)
    {
      exp = 0;
      result = 0;
    }
  else if (d.divided.c != 3)
    {
      exp = (d.di[1] >> 21) & 0x3FF;	// 10 bits.
      result  = d.di[1] & 0x1FFFFF;	// 21 bits.
      result  = result << 32;
      result |= d.di[0] & 0xFFFFFFFFUL;	// 32 bits.
    }
  else
    {
      exp = (d.di[1] >> 19) & 1023;
      result  = 4 << 19;
      result |= d.di[1] & 0x7FFFF;	// 19 bits.
      result  = result << 32;
      result |= d.di[0] & 0xFFFFFFFFUL;
    }

  if (result >= DECIMAL64_TH)
    result = 0;

  exp -= DECIMAL64_Bias;

  sprintf (str, "%llu", result);
  size = (16 - strlen (str));
  for (i = 0; i < size; i++)
    str[i] = '0';
  sprintf (str + i, "%llu", result);
  str[16] = '\0';

  if (sign_p)
    *sign_p = d.divided.negative;
  if (exp_p)
    *exp_p = exp;
  if (nan_p)
    *nan_p = c_f.is_nan;
  if (inf_p)
    *inf_p = c_f.is_inf;
}

void
__get_digits_d128 (_Decimal128 x, char *str, int *exp_p, int *sign_p,
		   int *nan_p, int *inf_p)
{
  int i, j, exp = 0;
  unsigned int digits[NDIGITS_128] = { 0 };

  struct ieee754r_c_field c_f;
  union ieee754r_Decimal128 d;
  unsigned long long int result[2];

  d.td = x;
  c_f = c_decoder[d.nan.nan];

  if (c_f.is_nan)
    {
      exp = 0;
      result[1] = 0;
      result[0] = 0;
    }
  else if (d.divided.c != 3)
    {
      exp = (d.ti[3] >> 17) & 0x3FFF;	// 14 bits.
      result[1]  = d.ti[3] & 0x1FFFF;	// 17 bits.
      result[1]  = result[1] << 32;
      result[1] |= d.ti[2] & 0xFFFFFFFFUL;	// 32 bits.
      result[0]  = d.ti[1] & 0xFFFFFFFFUL;
      result[0]  = result[0] << 32;
      result[0] |= d.ti[0] & 0xFFFFFFFFUL;
    }
  else
    {
      exp = (d.ti[3] >> 15) & 0x3FFF;
      result[1]  = 4 << 15;
      result[1] |= d.ti[3] & 0x7FFF;	// 15 bits.
      result[1]  = result[1] << 32;
      result[1] |= d.ti[2] & 0xFFFFFFFFUL;
      result[0]  = d.ti[1] & 0xFFFFFFFFUL;
      result[0]  = result[0] << 32;
      result[0] |= d.ti[0] & 0xFFFFFFFFUL;
    }

  if (result[1] >= DECIMAL128_TH)
    {
      result[0] = 0;
      result[1] = 0;
    }

  exp -= DECIMAL128_Bias;

  // High
  for (i = 63; i >= 0; i--)
    {
      digits[0] += (result[1] >> i) & 1;	// 1 bit.
      for (j = 0; j < 35; j++)
	digits[j] *= 2;		// Double the digits.
      for (j = 0; j < 34; j++)
	{			// Handle the carry.
	  digits[j + 1] += digits[j] / 10;
	  digits[j] %= 10;
	}
    }
  // Low
  for (i = 63; i >= 0; i--)
    {
      digits[0] += (result[0] >> i) & 1;
      if (i > 0)
	{
	  for (j = 0; j < 35; j++)
	    digits[j] *= 2;
	}
      for (j = 0; j < 34; j++)
	{
	  digits[j + 1] += digits[j] / 10;
	  digits[j] %= 10;
	}
    }

  j = 0;
  for (i = 33; i >= 0; i--)
    {
      sprintf (str + j, "%u", digits[i]);
      j++;
    }
  str[34] = '\0';

  if (sign_p)
    *sign_p = d.divided.negative;
  if (exp_p)
    *exp_p = exp;
  if (nan_p)
    *nan_p = c_f.is_nan;
  if (inf_p)
    *inf_p = c_f.is_inf;
}
