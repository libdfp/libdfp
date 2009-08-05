/* Returns the hyperbolic arc cosine of a _Decimal32 type argument

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

#define FUNCTION_NAME acosh

#include <dfpmacro.h>

#include <decNumberMath.h>

static DEC_TYPE
IEEE_FUNCTION_NAME (DEC_TYPE x)
{
  decContext context;
  decNumber dn_result;
  DEC_TYPE result, one;
  decNumber dn_x, dn_one;

  one=DFP_CONSTANT(1.0);
  FUNC_CONVERT_TO_DN (&one, &dn_one);
  FUNC_CONVERT_TO_DN (&x, &dn_x);

  if (___decNumberIsNaN (&dn_x))
    return x+x;

  if (x<one) {
    /*  Domain error (invalid exception) if x < 1 (incl. -Inf) */
    DFP_EXCEPT (FE_INVALID);
    return DFP_NAN;
  } else if (x==one) {
    /*  If x == +1, result is zero. */
    return DFP_CONSTANT(0.0);
  }

//  switch (___decCompare (&dn_x, &dn_one) )
//    {
      /*  Domain error (invalid exception) if x < 1 (incl. -Inf) */
//      case -1:
//	DFP_EXCEPT (FE_INVALID);
//	return DFP_NAN;
//      /*  If x == +1, result is zero. */
//      case 0:
//	return DFP_CONSTANT(0.0);
//    }

  /*  Early exit for the infinite case (-Inf is handled above) */
  if (___decNumberIsInfinite (&dn_x))
    return x;

  ___decContextDefault (&context, DEFAULT_CONTEXT);

  /* using trig identity: acosh(x) = log(x+sqrt(x*x-1)) */
  ___decNumberMultiply (&dn_result, &dn_x, &dn_x, &context);
  ___decNumberSubtract (&dn_result, &dn_result, &dn_one, &context);
  ___decNumberSquareRoot (&dn_result, &dn_result, &context);
  ___decNumberAdd (&dn_result, &dn_result, &dn_x, &context);
  ___decNumberLn (&dn_result, &dn_result, &context);

  FUNC_CONVERT_FROM_DN (&dn_result, &result, &context);

  return result;
}

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x)
{
  DEC_TYPE z = IEEE_FUNCTION_NAME (x);
#ifndef _IEEE_LIBDFP
  if(_LIB_VERSION == _IEEE_) return z;
  if (x < DFP_CONSTANT(1.0))
    DFP_ERRNO (EDOM);
#endif
  return z;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
