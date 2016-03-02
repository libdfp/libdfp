/* Handle conversion from binary long double (128) to Decimal64.

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
# define DEST 64
# define NAME trunc
#endif


#include "dfpacc.h"
#include "convert.h"

CONVERT_WRAPPER(
  // trunctfdd, trunctfsd
  _Decimal128 temp; /* Needs to be big enough so that temp = mant doesn't round.  */
  long double a_norm;
  long long mant;
  int exp;
  int parts, rest;

  a_norm = FREXPTF (a, &exp);
  a_norm *= 0x1p54L;  /* 2^54: Decimal64|32 needs not more precision.  */
  mant = a_norm;
  exp -= 54;

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
  result = (DEST_TYPE)temp; /* Cast to the resultant type.  */
)
hidden_def (PREFIXED_FUNCTION_NAME)
