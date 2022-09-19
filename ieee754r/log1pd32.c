/* Calculate the Natural Log of (a _Decimal32 value + 1)

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
#  define _DECIMAL_SIZE 32
#  include <decimal32.h>
#endif

#include <decContext.h>
#include <decNumber.h>
#include <math.h>
#include <errno.h>

#include <decNumberMath.h>

#define FUNCTION_NAME log1p

#include <dfpmacro.h>

static DEC_TYPE
IEEE_FUNCTION_NAME (DEC_TYPE x)
{
  decContext context;
  decNumber dn_result;
  DEC_TYPE result;
  decNumber dn_x;
  decNumber dn_sum;
  decNumber dn_one;
  DEC_TYPE one = DFP_CONSTANT(1.0);

  FUNC_CONVERT_TO_DN (&x, &dn_x);
  FUNC_CONVERT_TO_DN (&one, &dn_one);

  /*  For NaN, 0, or +Inf, just return x */
  if (decNumberIsNaN (&dn_x) || decNumberIsZero (&dn_x) ||
	(decNumberIsInfinite (&dn_x) && !decNumberIsNegative (&dn_x)))
    return x+x;

  decContextDefault(&context, DEFAULT_CONTEXT);
  decNumberAdd(&dn_sum, &dn_x, &dn_one, &context);
  if (decNumberIsZero(&dn_sum)) /*  Pole Error if x was -1 */
    {
      DFP_EXCEPT (FE_DIVBYZERO);
      return -DFP_HUGE_VAL;
    }
  if (decNumberIsNegative(&dn_sum)) /*  Domain Error if x < -1 */
    {
      DFP_EXCEPT (FE_INVALID);
      return DFP_NAN;
    }

  decNumberLn(&dn_result, &dn_sum, &context);
  FUNC_CONVERT_FROM_DN(&dn_result, &result, &context);


  return result;
}

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x)
{
  DEC_TYPE z = IEEE_FUNCTION_NAME (x);
  if (x == DFP_CONSTANT(-1.0))
    DFP_ERRNO (ERANGE);
  if (x < DFP_CONSTANT(-1.0))
    DFP_ERRNO (EDOM);
  return z;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
