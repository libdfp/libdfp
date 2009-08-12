/* Returns the _Decimal32 value * 2^ the integral value

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

#define FUNCTION_NAME ldexp

#include <dfpmacro.h>
#include <numdigits.h>
#include <limits.h>

static DEC_TYPE
IEEE_FUNCTION_NAME (DEC_TYPE x, int y)
{
  DEC_TYPE result;
  long newexp;

#if NUMDIGITS_SUPPORT==1
  newexp = FUNC_D (getexp) (x) + y + 1;
  if (newexp > PASTE(DECIMAL,PASTE(_DECIMAL_SIZE,_Emax)))
    {
    result = DFP_HUGE_VAL;
    DFP_EXCEPT (FE_OVERFLOW);
    }
  else if (newexp < PASTE(DECIMAL,PASTE(_DECIMAL_SIZE,_Emin)))
    {
    result = -DFP_HUGE_VAL;
    DFP_EXCEPT (FE_OVERFLOW);
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
   * by y + 1.  */
  newexp = dn_x.exponent + y + 1;
  if(newexp > INT_MAX)
    newexp = INT_MAX;
  if(newexp < -INT_MAX)
    newexp = -INT_MAX;
  dn_x.exponent = newexp;

  ___decContextDefault (&context, DEFAULT_CONTEXT);
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
#ifndef _IEEE_LIBDFP
  if (_LIB_VERSION == _IEEE_) return z;
  if (!FUNC_D(__finite) (z) && FUNC_D(__finite) (x))
    DFP_ERRNO (ERANGE);
#endif
  return z;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
