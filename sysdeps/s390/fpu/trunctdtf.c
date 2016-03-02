/* Handle conversion from Decimal128 to binary long double (128bit).

   Copyright (C) 2016 Free Software Foundation, Inc.

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

   Please see libdfp/COPYING.txt for more information.  */

#ifndef DECIMAL_TO_BINARY
# define DECIMAL_TO_BINARY
# define SRC 128
# define DEST 128
# define NAME trunc
#endif

#include "dfpacc.h"
#include "convert.h"
#include "convert_helpers.h"

CONVERT_WRAPPER(
  long double temp;
  _Decimal128 a_norm;
  long long mant;
  int exp;
  int parts, rest;

  a_norm = getmantandexpd128 (a, &exp, 18, 1e18DD);
  mant = a_norm;			/* stores first 18 digits of mantissa.  */

  a_norm -= mant;
  a_norm *= 1E+16DL;
  /* a = mant * 10^(exp - 18) + ((long long) a_norm) * 10^(exp - 34)  */

  /* part: mant * 10^(exp - 18)  */
  exp -= 18;				/* Exponent adjusted for mantissa.  */
  temp = mant;
  parts = exp / 256;			/* split because BINPOWOF10 ends at 10^308.  */
  rest = exp % 256;
  if (exp > 0)
    {
      for (int i = 0; i < parts; i++)
	temp *= BINPOWOF10[256];
      temp *= BINPOWOF10[rest];
    }
  else if (exp < 0)
    {
      for (int i = 0; i > parts; i--)
	temp /= BINPOWOF10[256];
      temp /= BINPOWOF10[-rest];
    }
  result = temp;

  /* part: ((long long) a_norm) * 10^(exp - 34)  */
  mant = a_norm;
  exp -= 16;				/* Exponent adjusted for mantissa.  */
  temp = mant;
  parts = exp / 256;			/* split because BINPOWOF10 ends at 10^308.  */
  rest = exp % 256;
  if (exp > 0)
    {
      for (int i = 0; i < parts; i++)
	temp *= BINPOWOF10[256];
      temp *= BINPOWOF10[rest];
    }
  else if (exp < 0)
    {
      for (int i = 0; i > parts; i--)
	temp /= BINPOWOF10[256];
      temp /= BINPOWOF10[-rest];
    }
  result += temp;
)
hidden_def (PREFIXED_FUNCTION_NAME)
