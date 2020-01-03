/* Returns the next representable neighbor of x in the direction of y

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
   if not, write to the Free Software Foundation, Inc., 59 Temple Place,
   Suite 330, Boston, MA 02111-1307 USA.

   Please see libdfp/COPYING.txt for more information.  */

#ifndef _DECIMAL_SIZE
/* Always include this since we need a _Decimal128 converted to a decNumber */
#  include <decimal128.h>
#  include <decimal32.h>
#  define _DECIMAL_SIZE 32
#endif

#include <decContext.h>
#include <decNumber.h>
#include <math.h>
#include <errno.h>
#include <float.h>
#include <ieee754r_private.h>

#define FUNCTION_NAME nexttoward

#include <dfpmacro.h>

static DEC_TYPE
IEEE_FUNCTION_NAME (DEC_TYPE x, _Decimal128 y)
{
  decContext context;
  decNumber dn_result;
  DEC_TYPE result;
  DEC_TYPE epsilon;
  decNumber dn_x;
  decNumber dn_y;
  decNumber dn_epsilon;
/*  int comparison; */

  FUNC_CONVERT_TO_DN(&x, &dn_x);
  decimal128ToNumber((decimal128*)&y, &dn_y);

  /*  Early exit for nan's */
  if (decNumberIsNaN(&dn_x))
    return x;
  if (decNumberIsNaN(&dn_y))
    return y;

  /*comparison = decCompare(&dn_x, &dn_y); */
  /*  Early exit for equal values */
  /*if (comparison == 0) */
  if (x==y)
    return x;

  epsilon = DFP_EPSILON;
  FUNC_CONVERT_TO_DN(&epsilon, &dn_epsilon);

  dn_epsilon.exponent += dn_x.digits+dn_x.exponent-1;

  decContextDefault(&context, DEFAULT_CONTEXT);
  /*if (comparison > 0)*/
  if (x>y)
    decNumberSubtract(&dn_result,&dn_x,&dn_epsilon,&context);
  else
    decNumberAdd(&dn_result,&dn_x,&dn_epsilon,&context);

  FUNC_CONVERT_FROM_DN(&dn_result, &result, &context);
  if (context.status & DEC_Overflow)
    DFP_EXCEPT (FE_OVERFLOW);

  return result;
}

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x, _Decimal128 y)
{
  DEC_TYPE z = IEEE_FUNCTION_NAME (x, y);
  if (!FUNC_D(__isfinite) (z) && FUNC_D(__isfinite) (x))
    DFP_ERRNO (ERANGE);
  /* TODO: Handle underflow */
  return z;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
