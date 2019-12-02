/* Returns the _Decimal32 value * 2^ the integral value

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
#  include <decimal32.h>
#  define _DECIMAL_SIZE 32
#endif

#include <decContext.h>
#include <decNumber.h>
#include <math.h>
#include <errno.h>
#include <ieee754r_private.h>

#define FUNCTION_NAME ldexp

#include <dfpmacro.h>
#include <numdigits.h>
#include <limits.h>

static DEC_TYPE
IEEE_FUNCTION_NAME (DEC_TYPE x, int y)
{
  DEC_TYPE result, tiny;
  long newexp;
  int minexp = PASTE(DECIMAL,PASTE(_DECIMAL_SIZE,_Emin));
  int p = PASTE(DECIMAL,PASTE(_DECIMAL_SIZE,_Pmax));

#if NUMDIGITS_SUPPORT==1
  newexp = FUNC_D (getexp) (x) + y;
  if (newexp > PASTE(DECIMAL,PASTE(_DECIMAL_SIZE,_Emax)))
    {
    result = FUNC_D(__copysign)(DFP_HUGE_VAL, x);
    DFP_EXCEPT (FE_OVERFLOW);
    }
  else if (newexp < minexp)
    {
    /* Check if the result might be subnormal. */
    if( newexp > (minexp - p))
      {
        result = FUNC_D(setexp) (x, minexp);
        /* Use a multiply so we correctly round whatever may shifts out. */
        tiny = FUNC_D(setexp) (1.DL, newexp - minexp);
        return result * tiny;
      }
    result = DFP_CONSTANT(0.0);
    DFP_EXCEPT (FE_UNDERFLOW);
    return result;
    }
  else
    result = FUNC_D(setexp) (x, newexp);

#else
  decContext context;
  decNumber dn_x;

  FUNC_CONVERT_TO_DN (&x, &dn_x);
  if (___decNumberIsNaN (&dn_x) || ___decNumberIsZero (&dn_x) ||
	___decNumberIsInfinite (&dn_x))
    return x+x;
  if (y == 0)
    return x;

  /* ldexp(x,y) is just x*10**y, which is equivalent to increasing the exponent
   * by y.  */
  newexp = dn_x.exponent + y;
  if(newexp > INT_MAX)
    newexp = INT_MAX;
  if(newexp < -INT_MAX)
    newexp = -INT_MAX;
  dn_x.exponent = newexp;

  decContextDefault (&context, DEFAULT_CONTEXT);
  FUNC_CONVERT_FROM_DN (&dn_x, &result, &context);

  if (context.status & DEC_Overflow)
    DFP_EXCEPT (FE_OVERFLOW);
#endif

  return result;
}

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x, int y)
{
  DEC_TYPE z = IEEE_FUNCTION_NAME (x, y);
  if (!FUNC_D(__isfinite) (z) && FUNC_D(__isfinite) (x))
    DFP_ERRNO (ERANGE);
  return z;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
