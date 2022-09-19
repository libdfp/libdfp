/* Returns the result of x - ((int-round_even)(x/y)) * y

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

#include <decContext.h>
#include <decNumber.h>
#include <math.h>
#include <errno.h>
#include <ieee754r_private.h>

#define FUNCTION_NAME remainder

#include <dfpmacro.h>

static DEC_TYPE
IEEE_FUNCTION_NAME (DEC_TYPE x, DEC_TYPE y)
{
  decContext context;
  decNumber dn_result;
  DEC_TYPE result;
  decNumber dn_x;
  decNumber dn_y;
  decNumber dn_mult;
  decNumber dn_rounded;

  FUNC_CONVERT_TO_DN (&x, &dn_x);
  FUNC_CONVERT_TO_DN (&y, &dn_y);
  if (decNumberIsNaN (&dn_x) || decNumberIsNaN (&dn_y))
    return x+y;

  /*  Domain Error: x = +-Inf, or y = +-0 and x is non-NaN */
  if (decNumberIsInfinite (&dn_x) || decNumberIsZero (&dn_y))
    {
      DFP_EXCEPT (FE_INVALID);
      return (x - x) / (x - x);
    }

  decContextDefault (&context, DEFAULT_CONTEXT);
  decNumberDivide (&dn_mult, &dn_x, &dn_y, &context);
  context.round = DEC_ROUND_HALF_EVEN;
  decNumberToIntegralValue (&dn_rounded, &dn_mult, &context);
  decNumberMultiply (&dn_mult, &dn_rounded, &dn_y, &context);
  decNumberSubtract (&dn_result, &dn_x, &dn_mult, &context);

  FUNC_CONVERT_FROM_DN (&dn_result, &result, &context);

  return result;
}

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x, DEC_TYPE y)
{
  DEC_TYPE z = IEEE_FUNCTION_NAME (x, y);
  if (!FUNC_D(__isinf) (x) || (!FUNC_D(__isnan) (x) && y == DFP_CONSTANT(0.0)))
    DFP_ERRNO (EDOM);
  return z;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
