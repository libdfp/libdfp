/* Calculates the arc that has the given tangent for a _Decimal32 type

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2007, 2009 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Pete Eberlein <eberlein@us.ibm.com>
              Joseph Kerian <jkerian@us.ibm.com>

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

#define FUNCTION_NAME atan2

#include <dfpmacro.h>


DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE y, DEC_TYPE x)
{
/*  This needs to be kept in the range of _Decimal32, and only added
 * to fairly large numbers (sizeable fractions of pi) */
  const DEC_TYPE generate_inexact = DFP_CONSTANT(1e-94);

  decContext context;
  decNumber dn_result, dn_temp;
  DEC_TYPE result;
  decNumber dn_x, dn_y;

  FUNC_CONVERT_TO_DN (&x, &dn_x);
  FUNC_CONVERT_TO_DN (&y, &dn_y);

  /*  If either argument is NaN, return NaN */
  if (___decNumberIsNaN (&dn_x) || ___decNumberIsNaN (&dn_y))
    return x+y;

  /* If  x,y are both non-inf, non-NaN, non-zero, calculate as normal */
  if (!___decNumberIsInfinite (&dn_x) && !___decNumberIsZero (&dn_y)
      && !___decNumberIsInfinite (&dn_y) && !___decNumberIsZero (&dn_x))
    {
      ___decContextDefault (&context, DEFAULT_CONTEXT);
      ___decNumberDivide (&dn_temp, &dn_y, &dn_x, &context);
      ___decNumberAtan (&dn_result, &dn_temp, &context);
      FUNC_CONVERT_FROM_DN (&dn_result, &result, &context);
      /* decNumberAtan doesn't quite return the values in the ranges we
       * want for x < 0. So we need to do some correction */
      if (___decNumberIsNegative (&dn_x))
	{
	  if (___decNumberIsNegative (&dn_y))
	    return result - M_PIdl;
	  else
	    return result + M_PIdl;
	}
      else
	return result;
    }

  /* If x and y are both inf, the result depends on the sign of x */
  if (___decNumberIsInfinite (&dn_y) && ___decNumberIsInfinite (&dn_x))
    {
      if (___decNumberIsNegative (&dn_x) )
	result = (DEC_TYPE)(3.0DL * M_PI_4dl) + generate_inexact;
      else
	result = (DEC_TYPE) M_PI_4dl + generate_inexact;
    }
  /*  If y is non-zero and x is non-inf, the result is +-pi/2 */
  else if (!___decNumberIsZero (&dn_y) && !___decNumberIsInfinite (&dn_x) )
    result = (DEC_TYPE)M_PI_2dl;
  else /*  Otherwise it is +0 if x is positive, +pi if x is neg */
    {
      if (___decNumberIsNegative (&dn_x))
	result = ((DEC_TYPE) M_PIdl) - generate_inexact;
      else
	result = DFP_CONSTANT (0.0);
    }

  /*  Atan2 will be negative if y<0 */
  if (___decNumberIsNegative (&dn_y))
    return -result;
  else
    return result;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
