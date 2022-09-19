/* Returns the unbiased exponent of the passed _Decimal32 value as an int

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2007-2015 Free Software Foundation, Inc.

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
   if not, write to the Free Software Foundation, Inc., 51 Franklin
   Street, Fifth Floor, Boston, MA 02110-1301 USA.

   Please see libdfp/COPYING.txt for more information.  */

#ifndef _DECIMAL_SIZE
#  include <decimal32.h>
#  define _DECIMAL_SIZE 32
#endif

#ifndef _RETURN_TYPE
#  define _RETURN_TYPE int
#endif

#ifndef _MAX_VALUE
#  define _MAX_VALUE   INT_MAX
#endif
#ifndef _MIN_VALUE
#  define _MIN_VALUE   INT_MIN
#endif

#ifndef _FBLOG0
#  define _FBLOG0      FP_ILOGB0
#endif
#ifndef _FBLOGNAN
#  define _FBLOGNAN    FP_ILOGBNAN
#endif

#include <decContext.h>
#include <decNumber.h>
#include <math.h>
#include <limits.h>
#include <errno.h>

#include <decNumberMath.h>

#ifndef FUNCTION_NAME
#  define FUNCTION_NAME ilogb
#endif

#include <dfpmacro.h>

_RETURN_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x)
{
  DEC_TYPE result;
  decContext context;
  decNumber dn_result;
  decNumber dn_x;
  decNumber dn_absx;
  decNumber dn_logx;
  decNumber dn_one;
  decNumber dn_cmp;
  enum rounding round;

  FUNC_CONVERT_TO_DN (&x, &dn_x);
  if (decNumberIsZero (&dn_x))
    {
      DFP_EXCEPT (FE_INVALID);
      DFP_ERRNO (EDOM);
      return _FBLOG0;
    }
  if (decNumberIsInfinite (&dn_x))
    {
      DFP_EXCEPT (FE_INVALID);
      DFP_ERRNO (EDOM);
      return decNumberIsNegative (&dn_x) ? _MIN_VALUE : _MAX_VALUE;
    }
  if (decNumberIsNaN (&dn_x))
    {
      DFP_EXCEPT (FE_INVALID);
      DFP_ERRNO (EDOM);
      return _FBLOGNAN;
    }

  decContextDefault (&context, DEFAULT_CONTEXT);

  decNumberAbs (&dn_absx, &dn_x, &context);

  /* For DFP, we use radix 10 instead of whatever FLT_RADIX happens to be */
  decNumberLog10 (&dn_logx, &dn_absx, &context);

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

  decNumberToIntegralValue (&dn_result, &dn_logx, &context);

  FUNC_CONVERT_FROM_DN (&dn_result, &result, &context);
  /* Use _Decimal* to int casting.  */
  return (_RETURN_TYPE) result;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
