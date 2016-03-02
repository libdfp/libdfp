/* Handle conversion from binary long double (128bit) to Decimal128.

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

#ifndef BINARY_TO_DECIMAL
# define BINARY_TO_DECIMAL
# define SRC 128
# define DEST 128
# define NAME extend
#endif

#include "dfpacc.h"
#include "convert.h"

CONVERT_WRAPPER(
  _Decimal128 temp;
  long double a_norm;
  long long mant;
  int exp;
  int parts, rest;

  a_norm = FREXPTF (a, &exp);
  a_norm *= 0x1p63L;  /* * 2^63 because long long is full
			 but __LDBL_MANT_DIG__ == 113!  */
  mant = a_norm;      /* Stores first 63 bits of mantissa.  */
  a_norm -= mant;     /* Cut first 63 bits of mantissa
			 and leave the rest of mantissa here.  */
  a_norm *= 0x1p50L;  /* * 2^50 for bits 64 to 113.  */
  /* a = mant * 2^(exp - 63) + ((long long) a_norm) * 2^(exp - 113)  */

  /* part: mant * 2^(exp - 63)  */
  exp -= 63;
  temp = mant;
  parts = exp / 1024; /* Split because DECPOWOF2 ends at 2^1077.  */
  rest = exp % 1024;
  if (exp > 0)
    {
      for (int i = 0; i < parts; i++)
	temp *= DECPOWOF2[1024];
      temp *= DECPOWOF2[rest];
    }
  else if (exp < 0)
    {
      for (int i = 0; i > parts; i--)
	temp /= DECPOWOF2[1024];
      temp /= DECPOWOF2[-rest];
    }
  result = temp;

  /* part: ((long long) a_norm) * 2^(exp - 113)  */
  mant = a_norm;
  exp -= 50;
  parts = exp / 1024;
  rest = exp % 1024;
  temp = mant;
  if (exp > 0)
    {
      for (int i = 0; i < parts; i++)
	temp *= DECPOWOF2[1024];
      temp *= DECPOWOF2[rest];
    }
  else if (exp < 0)
    {
      for (int i = 0; i > parts; i--)
	temp /= DECPOWOF2[1024];
      temp /= DECPOWOF2[-rest];
    }
  result += temp;
)
hidden_def (PREFIXED_FUNCTION_NAME)
