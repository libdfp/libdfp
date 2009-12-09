/* Break floating point number into normalized fraction and integral power of two

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2007, 2009 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Joseph Kerian <jkerian@us.ibm.com>

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
#  define _DECIMAL_SIZE 32
#  include <decimal32.h>
#endif

#include <decContext.h>
#include <decNumber.h>
#include <math.h>

#include <decNumberMath.h>

#define FUNCTION_NAME frexp

#include <dfpmacro.h>
#include <numdigits.h>

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x, int *y)
{
  /* GCC isn't smart enough to realize that the else block sets this in the
   * FUNC_CONVERT_FROM_DN macro so simply initialize it to NaN to silence the
   * warnings.  */
  DEC_TYPE result = DEC_NAN;

#if NUMDIGITS_SUPPORT==1
  int digits, exponent;

  if (isinf(x) || isnan(x))
    return x+x;

  // Given 3.1e0 is encoded as 31e-1 and we want .31e1
  // We have 2 'digits'.

  // Given 30.0e0 is encoded as 300e-1 and we want .300e2
  // We have 3 'digits'.

  // Given .003e0 is encoded as 3e3 and we want .3e-2
  // We have 1 'digits'.

  digits = FUNC_D (numdigits) (x);

  // We have -1 exponent.

  // We have -1 exponent.

  // We have -3 exponent.

  exponent = FUNC_D (getexp) (x);
  *y = digits + exponent;

  result = FUNC_D(setexp) (x, -digits);
  //2 + (-1) = 1   0

  //3 + (-1) = 2   0

  //1 + (-3) = -2

#else
  decNumber dn_x;
  decContext context;

  *y = 0;
  FUNC_CONVERT_TO_DN(&x, &dn_x);
  if (___decNumberIsNaN (&dn_x) || ___decNumberIsInfinite (&dn_x)
	|| ___decNumberIsZero (&dn_x))
    return x+x;

  *y = dn_x.digits + dn_x.exponent;
  dn_x.exponent = -dn_x.digits;

  ___decContextDefault(&context, DEFAULT_CONTEXT);
  FUNC_CONVERT_FROM_DN (&dn_x, &result, &context);
#endif 

  return result;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
