/* Calculate x^y, where x and y are _Decimal32 types

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
#include <errno.h>

#include <decNumberMath.h>

#define FUNCTION_NAME pow

#include <dfpmacro.h>

static DEC_TYPE
IEEE_FUNCTION_NAME (DEC_TYPE x, DEC_TYPE y)
{
  decContext context;
  decNumber dn_result;
  DEC_TYPE result;
  DEC_TYPE absx;
  decNumber dn_x;
  decNumber dn_absx;
  decNumber dn_y;
  decNumber dn_one;
  decNumber dn_two;
  decNumber dn_temp;
  decNumber dn_temp2;
  decNumber dn_temp3;
  int y_is_int;
  int y_is_oddint=0;
  int abs_x_vs_1;
  DEC_TYPE one = DFP_CONSTANT(1.0);
  DEC_TYPE two = DFP_CONSTANT(2.0);

  FUNC_CONVERT_TO_DN (&x, &dn_x);
  FUNC_CONVERT_TO_DN (&y, &dn_y);
  FUNC_CONVERT_TO_DN (&one, &dn_one);

  ___decContextDefault (&context, DEFAULT_CONTEXT);
  if (___decNumberIsZero (&dn_y))
    return one;
  if (___decNumberIsNaN (&dn_x))
    return x+x;

  ___decNumberAbs (&dn_absx, &dn_x, &context);

  FUNC_CONVERT_FROM_DN (&dn_absx, &absx, &context);
  if(absx<one)
    abs_x_vs_1 = -1;
  else if (absx==one)
    abs_x_vs_1 = 0;
  else
    abs_x_vs_1 = 1;

/*  abs_x_vs_1 = ___decCompare(&dn_absx, &dn_one); */
  if(abs_x_vs_1 == 0 && !___decNumberIsNegative (&dn_x)) /*  If x == +1 */
    return one;
  if (___decNumberIsNaN (&dn_y))
    return y+y;

  /*  Detect if y is odd/an integer */
  ___decNumberToIntegralValue (&dn_temp, &dn_y, &context);
  ___decNumberSubtract (&dn_temp2, &dn_temp, &dn_y, &context);
  y_is_int = ___decNumberIsZero (&dn_temp2);
  if (y_is_int)
    {
      FUNC_CONVERT_TO_DN (&two, &dn_two);
      ___decNumberDivide (&dn_temp, &dn_y, &dn_two, &context);
      ___decNumberToIntegralValue (&dn_temp2, &dn_temp, &context);
      ___decNumberSubtract (&dn_temp3, &dn_temp2, &dn_temp, &context);
      y_is_oddint = !___decNumberIsZero (&dn_temp3);
    }

  /*  Handle all special cases for which x = +-0 */
  if (___decNumberIsZero (&dn_x))
    {
      if(___decNumberIsNegative (&dn_y))
	{
	  if (___decNumberIsInfinite (&dn_y))	/*  +-0^-Inf = +Inf */
	    return -y;
	  /*  Pole Error for x = +-0, y < 0 */
	  DFP_EXCEPT (FE_DIVBYZERO);
	  return ___decNumberIsNegative(&dn_x) && y_is_oddint ?
		-DFP_HUGE_VAL : DFP_HUGE_VAL;
	}
      return ___decNumberIsNegative(&dn_x) && y_is_oddint ?
		-DFP_CONSTANT(0.0) : DFP_CONSTANT(0.0);
    }

  /* Handle remaining special cases for x = +-Inf or y = +-Inf */
  if (___decNumberIsInfinite (&dn_x) || ___decNumberIsInfinite (&dn_y))
    {
      if (abs_x_vs_1 == 0)	/*  If (-1)^(+-Inf) */
	return one;
      if (abs_x_vs_1 < 0)	/*  x^(+-Inf), where 0<x<1 */
	return ___decNumberIsNegative (&dn_y) ? DFP_HUGE_VAL
		: DFP_CONSTANT(0.0);
      if (___decNumberIsNegative (&dn_y))
	result = DFP_CONSTANT(0.0);
      else
	result = (DEC_TYPE)DEC_INFINITY;
      if (y_is_oddint && ___decNumberIsNegative(&dn_x))
	result = -result;
      return result;
    }

  /* Domain Error: x < 0 && y is a finite non-int */
  if (___decNumberIsNegative (&dn_x) && !y_is_int)
    {
      DFP_EXCEPT (FE_INVALID);
      return DFP_NAN;
    }

  ___decNumberPower (&dn_result, &dn_x, &dn_y, &context);
  FUNC_CONVERT_FROM_DN (&dn_result, &result, &context);

  if (context.status & DEC_Overflow)
    DFP_EXCEPT (FE_OVERFLOW);
  if (context.status & DEC_Underflow)
    DFP_EXCEPT (FE_UNDERFLOW);

  return result;
}

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x, DEC_TYPE y)
{
  DEC_TYPE z = IEEE_FUNCTION_NAME (x, y);
#ifndef _IEEE_LIBDFP
  if (_LIB_VERSION == _IEEE_)  return z;
  /* Pole error: x = 0, y < 0 (non-inf). Set ERANGE in accordance with C99 */
  if (x == DFP_CONSTANT(0.0) && FUNC_D(__finite)(y) && y < DFP_CONSTANT(0.0))
      DFP_ERRNO (ERANGE);
  if (!FUNC_D(__finite) (z) && FUNC_D(__finite) (x) && FUNC_D(__finite) (y))
    {
      if (FUNC_D(__isnan) (z)) /*  Domain error was triggered, x < 0 and y was not an
			odd int */
	DFP_ERRNO (EDOM);
      else	/*  Overflow */
	DFP_ERRNO (ERANGE);
    }
#endif
  return z;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
