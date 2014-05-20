/* Functions to print the BID declets for use with printf %Z

   Copyright (C) 2014 Free Software Foundation, Inc.

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

   Please see dfp/COPYING.txt for more information.  */

#include <stdio.h>

#include <decode.h>
#include <numdigits.h>
#include <math_private.h>

/* This version is used by all BID implementations.  */

#define BID_SIGN_MASK 0x80000000UL
/* BID_SPEC_MASK is the mask for _Decimal32 to be used with uint32_t and
   since we only work with uint32_t it works for all decoded.  */
#define BID_SPEC_MASK DEC32_INF_MASK

char *
__decoded32 (_Decimal32 a, char *str)
{
  union ieee754r_Decimal32 d = { .sd = a };
  int exp = -DECIMAL32_Bias;
  char sign;
  unsigned int coeff;
  unsigned int decunits[3]  = { 0 };
  int n;

  sign = (d.si & BID_SIGN_MASK ? '-' : '+');
  if ((d.si & BID_SPEC_MASK) != BID_SPEC_MASK)
    {
      if ((d.si & BID_EXPONENT_ENC_MASK) == BID_EXPONENT_ENC_MASK)
	coeff = 0x00800000 | (d.si & 0x001FFFFF);
      else
	coeff = d.si & 0x007FFFFF;

      for (n = 0; (coeff != 0) && (n < 3); ++n)
	{
	  decunits[n] = coeff % 1000;
	  coeff /= 1000;
	}

      exp = getexpd32 (a);
    }

  sprintf (str, "%c%01u,%03u,%03uE%+d",
           sign, decunits[2], decunits[1], decunits[0], exp);

  return str;
}
weak_alias (__decoded32, decoded32)
hidden_def (__decoded32)


char *
__decoded64 (_Decimal64 a, char *str)
{
  union ieee754r_Decimal64 d = { .dd = a };
  int exp = -DECIMAL64_Bias;
  char sign;
  unsigned long long int coeff;
  unsigned int decunits[6]  = { 0 };
  int n;

  sign = (d.di[1] & BID_SIGN_MASK ? '-' : '+');
  if ((d.di[1] & BID_SPEC_MASK) != BID_SPEC_MASK)
    {
      if ((d.di[1] & BID_EXPONENT_ENC_MASK) == BID_EXPONENT_ENC_MASK)
	coeff = 0x00200000 | (d.di[1] & 0x0007FFFF);
      else
	coeff = d.di[1] & 0x001FFFFF;
      coeff = (coeff << 32) | d.di[0];

      for (n = 0; (coeff != 0) && (n < 6); ++n)
	{
	  decunits[n] = coeff % 1000;
	  coeff /= 1000;
	}
      exp = getexpd64 (a);
    }

  sprintf (str, "%c%01u,%03u,%03u,%03u,%03u,%03uE%+d",
           sign, decunits[5], decunits[4], decunits[3],
           decunits[2], decunits[1], decunits[0], exp);

  return str;
}
weak_alias (__decoded64, decoded64)
hidden_def (__decoded64)


char *
__decoded128 (_Decimal128 a, char *str)
{
  union ieee754r_Decimal128 d = { .td = a };
  int exp = -DECIMAL128_Bias;
  char sign;
  u128_t coeff;
  unsigned int decunits[12]  = { 0 };
  unsigned int hi;
  int n;

  sign = (d.ti[3] & BID_SIGN_MASK ? '-' : '+');
  if ((d.ti[3] & BID_SPEC_MASK) != BID_SPEC_MASK)
    {
      if ((d.ti[3] & BID_EXPONENT_ENC_MASK) == BID_EXPONENT_ENC_MASK)
	hi = 0x00020000UL | (d.ti[3] & 0x00007FFFUL);
      else
	hi = d.ti[3] & 0x0001FFFFUL;
      u128_init_from_u32 (coeff, hi, d.ti[2], d.ti[1], d.ti[0]);

      for (n = 0; u128_ne_u32 (coeff, 0) && (n < 12); ++n)
	{
	  u128_t tmp;
	  u128_init (tmp);
	  u128_mod_u32 (coeff, 1000, tmp);
	  u128_to_u32 (tmp, decunits[n]);
	  u128_div_u32 (coeff, 1000, coeff);
        }

      exp = getexpd128 (a);
    }

  sprintf (str, "%c%01u,%03u,%03u,%03u,%03u,%03u,%03u,%03u,%03u,%03u,%03u,%03uE%+d",
           sign, decunits[11], decunits[10], decunits[9],
           decunits[8], decunits[7], decunits[6],
           decunits[5], decunits[4], decunits[3],
           decunits[2], decunits[1], decunits[0], exp);

  return str;
}
weak_alias (__decoded128, decoded128)
hidden_def (__decoded128)
