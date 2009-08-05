/* Calculates the arc that has the given tangent for a _Decimal32 type

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

#include <decNumberMath.h>

#define FUNCTION_NAME atan

#include <dfpmacro.h>

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x)
{
  decContext context;
  decNumber dn_result;
  DEC_TYPE result;
  decNumber dn_x;

  FUNC_CONVERT_TO_DN (&x, &dn_x);

  ___decContextDefault (&context, DEFAULT_CONTEXT);
  if (___decNumberIsInfinite (&dn_x))
    {
      if (___decNumberIsNegative (&dn_x))
        result = -M_PI_2dl;
      else
        result = M_PI_2dl;
    }
  else
    {
      ___decNumberAtan (&dn_result, &dn_x, &context);
      FUNC_CONVERT_FROM_DN (&dn_result, &result, &context);
    }

  return result;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
