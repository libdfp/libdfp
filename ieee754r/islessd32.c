/* Implements < for _Decimal32

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
   /* needed to pick up DECNUMDIGITS before including decNumber.h  */
#  include <decimal32.h>
#endif

#include <decContext.h>
#include <decNumber.h>
#include <math.h>

#define FUNCTION_NAME isless

#include <dfpmacro.h>
#include <ieee754r_private.h>

int
INTERNAL_FUNCTION_NAME (DEC_TYPE x, DEC_TYPE y)
{
  decNumber dn_x;
  decNumber dn_y;
  decNumber dn_result;
  decContext context;

  FUNC_CONVERT_TO_DN(&x, &dn_x);
  FUNC_CONVERT_TO_DN(&y, &dn_y);

  if (decNumberIsNaN(&dn_x) || decNumberIsNaN(&dn_y))
    return 0;

  decContextDefault (&context, DEFAULT_CONTEXT);
  decNumberCompare (&dn_result, &dn_x, &dn_y, &context);
  return decNumberIsNegative(&dn_result);
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
