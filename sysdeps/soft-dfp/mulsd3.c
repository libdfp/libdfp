/* _Decimal32 multiplication for soft-dfp

   Copyright (C) 2007, 2008 IBM Corporation.
   Copyright (C) 2008, 2009 Free Software Foundation, Inc.

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
#  include <decimal32.h>
#  define _DECIMAL_SIZE 32
#  define ACTUAL_FUNCTION_NAME mulsd3
#endif

#include <decContext.h>
#include <decNumber.h>
#include <math.h>

#define FUNCTION_NAME mul

#include "dfpacc.h"
#include <dfpmacro.h>
DEC_TYPE
PREFIXED_FUNCTION_NAME (DEC_TYPE x, DEC_TYPE y)
{
  DEC_TYPE result;
  decNumber dn_x, dn_y, dn_result;
  decContext context;
  ___decContextDefault(&context, DEFAULT_CONTEXT);

  FUNC_CONVERT_TO_DN(&x, &dn_x);
  FUNC_CONVERT_TO_DN(&y, &dn_y);

  ___decNumberMultiply(&dn_result, &dn_x, &dn_y, &context);

  if (context.status != 0)
    {
      int ieee_flags = 0;
      if (context.status & DEC_IEEE_854_Division_by_zero)
        ieee_flags |= FE_DIVBYZERO;
      if (context.status & DEC_IEEE_854_Inexact)
        ieee_flags |= FE_INEXACT;
      if (context.status & DEC_IEEE_854_Invalid_operation)
        ieee_flags |= FE_INVALID;
      if (context.status & DEC_IEEE_854_Overflow)
        ieee_flags |= FE_OVERFLOW;
      if (context.status & DEC_IEEE_854_Underflow)
        ieee_flags |= FE_UNDERFLOW;
      if (ieee_flags != 0)
        feraiseexcept (ieee_flags);
    }

  FUNC_CONVERT_FROM_DN (&dn_result, &result, &context);
  return result;
}
