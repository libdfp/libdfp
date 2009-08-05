/* Returns x * FLT_RADIX^y without computing the right side directly

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

#define FUNCTION_NAME scalbn

#include <dfpmacro.h>

#include <stdio.h>

static DEC_TYPE
IEEE_FUNCTION_NAME (DEC_TYPE x, int y)
{
  decContext context;
  DEC_TYPE result;
  decNumber dn_x, dn_y;
/*  long int sum = 0L; */
/*  uint32_t temp2 = 0; */
  /* Otherwise this may pass a bad residue to decFinalize which can result in
   * rounding error.  */
/*  int32_t temp =0; */

  FUNC_CONVERT_TO_DN(&x, &dn_x);
  if (___decNumberIsNaN (&dn_x))
    return x+x;
  if (___decNumberIsInfinite (&dn_x) || ___decNumberIsZero (&dn_x) || y==0L)
    return x;

  ___decContextDefault(&context, DEFAULT_CONTEXT);
//  sum = dn_x.exponent + y;
//  if(sum >= 1000000000L || sum <= -2000000000LL)
//    ; /** definite overflow */
//  else
//    dn_x.exponent = sum;
//
//  ___decFinalize(&dn_x, &context, &temp, &temp2);

  ___decNumberFromInt32(&dn_y, y);
  ___decNumberScaleB(&dn_x, &dn_x, &dn_y, &context);

  FUNC_CONVERT_FROM_DN(&dn_x, &result, &context);
  if (context.status & DEC_Overflow)
    DFP_EXCEPT (FE_OVERFLOW);

  return result;
}

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x, int y)
{
  DEC_TYPE z = IEEE_FUNCTION_NAME (x, y);
#ifndef _IEEE_LIBDFP
  if (_LIB_VERSION == _IEEE_) return z;
  if (!FUNC_D(__finite) (z) && FUNC_D(__finite) (x))
    DFP_ERRNO (ERANGE);
#endif
  return z;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
