/* Floating point multiply and add (x * y + z)

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

#define FUNCTION_NAME fma

#include <dfpmacro.h>

static DEC_TYPE
IEEE_FUNCTION_NAME (DEC_TYPE x, DEC_TYPE y, DEC_TYPE z)
{
  decContext context;
  decNumber dn_result;
  DEC_TYPE result;
  decNumber dn_x;
  decNumber dn_y;
  decNumber dn_product;
  decNumber dn_z;

  FUNC_CONVERT_TO_DN (&x, &dn_x);
  FUNC_CONVERT_TO_DN (&y, &dn_y);
  FUNC_CONVERT_TO_DN (&z, &dn_z);

  /*  If x or y is NaN, return NaN */
  if (___decNumberIsNaN (&dn_x) || ___decNumberIsNaN (&dn_y))
    return x+y;

  /*  Domain error if x or y is Inf, the other is 0 */
  if (  (___decNumberIsInfinite (&dn_x) && ___decNumberIsZero (&dn_y)) ||
	(___decNumberIsInfinite (&dn_y) && ___decNumberIsZero (&dn_x))  )
    {
      DFP_EXCEPT (FE_INVALID);
      return DFP_NAN;
    }
  /* If x and y are not 0,Inf or Inf,0, and z is NaN, return NaN */
  if (___decNumberIsNaN (&dn_z))
    return z+z;

  ___decContextDefault (&context, DEFAULT_CONTEXT);
  ___decNumberMultiply (&dn_product, &dn_x, &dn_y, &context);

  /* Domain error if x*y = Inf and z=Inf (with opposite signs) */
  if (___decNumberIsInfinite (&dn_product) && ___decNumberIsInfinite (&dn_z) &&
	(___decNumberIsNegative (&dn_product) != ___decNumberIsNegative (&dn_z)))
    {
      DFP_EXCEPT (FE_INVALID);
      return DFP_NAN;
    }
  ___decNumberAdd (&dn_result, &dn_product, &dn_z, &context);

  FUNC_CONVERT_FROM_DN (&dn_result, &result, &context);

  return result;
}

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x, DEC_TYPE y, DEC_TYPE z)
{
  DEC_TYPE r = IEEE_FUNCTION_NAME (x, y, z);
#ifndef _IEEE_LIBDFP
  if(_LIB_VERSION == _IEEE_) return r;
  if ( (FUNC_D(__isinf) (x) && y == DFP_CONSTANT(0.0)) ||
	(FUNC_D(__isinf) (y) && x == DFP_CONSTANT(0.0)) )
      DFP_ERRNO(EDOM);
  else if (FUNC_D (__isinf) (z))
    {
      int isneg = FUNC_D(__signbit) (x) ^ FUNC_D(__signbit) (y);
      int inf = FUNC_D(__isinf) (x) | FUNC_D(__isinf) (y);
      if ( inf && FUNC_D (__signbit) (z) != isneg)
	DFP_ERRNO (EDOM);
    }
#endif
  return r;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
