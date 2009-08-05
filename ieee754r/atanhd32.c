/* Calculate the hyperbolic arc tangent (atanh) for _Decimal32 type

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2007, 2009 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Pete Eberlein <eberlein@us.ibm.com>

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

#define FUNCTION_NAME atanh

#include <dfpmacro.h>

static DEC_TYPE
IEEE_FUNCTION_NAME (DEC_TYPE x)
{
  decContext context;
  decNumber dn_result;
  DEC_TYPE result, one, temp;
  decNumber dn_x, dn_temp, dn_one;
/*  int comp;*/

  one=DFP_CONSTANT(1.0);
  FUNC_CONVERT_TO_DN (&one, &dn_one);
  FUNC_CONVERT_TO_DN (&x, &dn_x);

  /*  Handle NaN and early exit for x==0 */
  if (___decNumberIsNaN (&dn_x) || ___decNumberIsZero (&dn_x))
    return x + x;

  ___decContextDefault (&context, DEFAULT_CONTEXT);
  ___decNumberAbs (&dn_temp, &dn_x, &context);

  FUNC_CONVERT_FROM_DN (&dn_temp, &temp, &context);
  if(temp==one) {
	/*  |x| == 1 -> Pole Error */
	DFP_EXCEPT (FE_DIVBYZERO);
	return ___decNumberIsNegative(&dn_x) ? -DFP_HUGE_VAL:DFP_HUGE_VAL;
  } else if (temp>one) {
	/*  |x| > 1 -> Domain Error (this handles +-Inf too) */
	DFP_EXCEPT (FE_INVALID);
	return DFP_NAN;
  }

//  comp = ___decCompare (&dn_temp, &dn_one);
//  switch (comp)
//    {
//      case 0: /*  |x| == 1 -> Pole Error */
//	DFP_EXCEPT (FE_DIVBYZERO);
//	return ___decNumberIsNegative(&dn_x) ? -DFP_HUGE_VAL:DFP_HUGE_VAL;
//      case 1: /*  |x| > 1 -> Domain Error (this handles +-Inf too) */
//	DFP_EXCEPT (FE_INVALID);
//	return DFP_NAN;
//    }

  /* Using trig identity: atanh(x) = 1/2 * log((1+x)/(1-x)) */
  ___decNumberAdd (&dn_result, &dn_one, &dn_x, &context);
  ___decNumberSubtract (&dn_temp, &dn_one, &dn_x, &context);
  ___decNumberDivide (&dn_result, &dn_result, &dn_temp, &context);
  ___decNumberLn (&dn_result, &dn_result, &context);
  ___decNumberAdd (&dn_temp, &dn_one, &dn_one, &context); /* 2 */
  ___decNumberDivide (&dn_result, &dn_result, &dn_temp, &context);

  FUNC_CONVERT_FROM_DN (&dn_result, &result, &context);

  return result;
}

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x)
{
  DEC_TYPE z = IEEE_FUNCTION_NAME (x);
#ifndef _IEEE_LIBDFP
  if(_LIB_VERSION == _IEEE_) return z;
  if (x < DFP_CONSTANT(-1.0) || x > DFP_CONSTANT(1.0))
    DFP_ERRNO (EDOM);
  /* The normal glibc ieee754 k_standard.c file does not follow c99 or POSIX
   * with regard to atanh pole errors. atan(+-1.0) [the binary version] will
   * set errno to EDOM.  Hopefully this will get worked out soon. */
  if (x == DFP_CONSTANT(-1.0) || x == DFP_CONSTANT(1.0))
    DFP_ERRNO (ERANGE);
#endif
  return z;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
