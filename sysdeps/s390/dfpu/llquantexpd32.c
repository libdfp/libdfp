/* Compute the quantum exponend of a finite argument as long long int.

   Copyright (C) 2018 Free Software Foundation, Inc.

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

#ifndef _DECIMAL_SIZE
# define _DECIMAL_SIZE 32
#endif

#include <math.h>
#include <limits.h>

#define FUNCTION_NAME llquantexp
#include <dfpmacro.h>

#include <numdigits.h>

long long int
INTERNAL_FUNCTION_NAME (DEC_TYPE x)
{
  int exp = FUNC_D(getexp)(x);
  int bias_exp = exp + DECIMAL_BIAS;
  if (bias_exp < 0 && bias_exp >= -3)
    {
      /* -1 = Infinity
	 -2 = NaN
	 -3 = SNaN  */
      DFP_ERRNO (EDOM);
      return LLONG_MIN;
    }
  return exp;
}
weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
