/* Compute the quantum exponend of a finite argument.

   Copyright (C) 2014-2015 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

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

#include <decimal32.h>

#include <decContext.h>
#include <decNumber.h>
#include <math.h>

#define _DECIMAL_SIZE 32
#include <dfpmacro.h>

_Decimal32
__quantumd32 (_Decimal32 x)
{
  decNumber dn_x;
  decNumber dn_result;
  decContext context;
  _Decimal32 result;

  FUNC_CONVERT_TO_DN (&x, &dn_x);
  if (decNumberIsNaN (&dn_x) || decNumberIsZero (&dn_x))
    return x;
  if (decNumberIsInfinite (&dn_x))
    return DEC_INFINITY;

  /* The quantum of a finite number is defined as 1 x 10^exponent, so
     first get input absolute value and then sets its coefficient to 1.  */
  decContextDefault (&context, DEFAULT_CONTEXT);
  decNumberAbs (&dn_result, &dn_x, &context);
  dn_result.digits = 1;
  dn_result.lsu[0] = 1;

  FUNC_CONVERT_FROM_DN (&dn_result, &result, &context);

  return result;
}
weak_alias (__quantumd32, quantumd32)
