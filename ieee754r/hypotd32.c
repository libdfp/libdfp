/* Returns sqrt(x*x+y*y)

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

#define FUNCTION_NAME hypot

#include <dfpmacro.h>

static DEC_TYPE
IEEE_FUNCTION_NAME (DEC_TYPE x, DEC_TYPE y)
{
  decContext context;
  decNumber dn_result;
  DEC_TYPE result;
  decNumber dn_x;
  decNumber dn_xx;
  decNumber dn_y;
  decNumber dn_yy;
  decNumber dn_sum;

  FUNC_CONVERT_TO_DN (&x, &dn_x);
  FUNC_CONVERT_TO_DN (&y, &dn_y);
  if (___decNumberIsInfinite (&dn_x))
    return x;
  if (___decNumberIsInfinite (&dn_y))
    return y;
  if (___decNumberIsNaN (&dn_x) || ___decNumberIsNaN (&dn_y))
    return x+y;

  ___decContextDefault (&context, DEFAULT_CONTEXT);
  ___decNumberMultiply (&dn_xx, &dn_x, &dn_x, &context);
  ___decNumberMultiply (&dn_yy, &dn_y, &dn_y, &context);
  ___decNumberAdd (&dn_sum, &dn_xx, &dn_yy, &context);
  ___decNumberSquareRoot (&dn_result, &dn_sum, &context);

  FUNC_CONVERT_FROM_DN (&dn_result, &result, &context);

  if (context.status & DEC_Overflow)
    DFP_EXCEPT (FE_OVERFLOW);

  return result;
}

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x, DEC_TYPE y)
{
  DEC_TYPE z = IEEE_FUNCTION_NAME (x, y);
#ifndef _IEEE_LIBDFP
  if(_LIB_VERSION == _IEEE_) return z;
  if (!FUNC_D(__finite) (z) && FUNC_D(__finite) (x) && FUNC_D(__finite) (y))
    DFP_ERRNO (ERANGE);
#endif
  return z;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
