/* Returns the unbiased exponent of the passed _Decimal32 value

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
#  include <decimal32.h>
#  define _DECIMAL_SIZE 32
#endif

#include <decContext.h>
#include <decNumber.h>
#include <math.h>
#include <errno.h>

#include <decNumberMath.h>

#define FUNCTION_NAME logb

#include <dfpmacro.h>

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x)
{
  decContext context;
  decNumber dn_result;
  DEC_TYPE result;
  decNumber dn_x;
  decNumber dn_tmp;
  decNumber dn_log10;
  decNumber dn_one;
  decNumber dn_cmp;
  enum rounding round;

  FUNC_CONVERT_TO_DN (&x, &dn_x);
  if (decNumberIsNaN (&dn_x))
    return x+x;
  if (decNumberIsInfinite (&dn_x))	/* +-Inf: Inf  */
    return DEC_INFINITY;
  if (decNumberIsZero (&dn_x))	/*  Pole Error if x==0 */
    {
      DFP_ERRNO (ERANGE);
      DFP_EXCEPT (FE_DIVBYZERO);
      return -DFP_HUGE_VAL;
    }
  if (decNumberIsInfinite (&dn_x) && decNumberIsNegative (&dn_x))
    return -x;

  decContextDefault (&context, DEFAULT_CONTEXT);
  decNumberAbs (&dn_tmp, &dn_x, &context);
  /*  For DFP, we use radix 10 instead of whatever FLT_RADIX
      happens to be */
  decNumberLog10 (&dn_log10, &dn_tmp, &context);

  /* Capture the case where truncation will return the wrong result,
     by rounding up if -1.0 < x < 1.0  */
  round = DEC_ROUND_DOWN;
  decNumberFromInt32 (&dn_one, 1);
  decNumberCompare (&dn_cmp, &dn_x, &dn_one, &context);
  if (-decNumberIsNegative(&dn_cmp))
    {
      decNumberFromInt32 (&dn_one, -1);
      decNumberCompare (&dn_cmp, &dn_x, &dn_one, &context);
      if (!decNumberIsNegative(&dn_cmp) && !decNumberIsZero(&dn_cmp))
	round = DEC_ROUND_UP;
    }
  context.round = round;

  decNumberToIntegralValue (&dn_result, &dn_log10, &context);

  FUNC_CONVERT_FROM_DN (&dn_result, &result, &context);

  return result;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
