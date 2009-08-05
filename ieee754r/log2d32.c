/* Returns the base 2 logarithmic function of x

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

#define FUNCTION_NAME log2

#include <dfpmacro.h>

static DEC_TYPE
IEEE_FUNCTION_NAME (DEC_TYPE x)
{
  decContext context;
  decNumber dn_result;
  DEC_TYPE result;
  decNumber dn_x;
  decNumber dn_two;
  decNumber dn_logtwo;
  decNumber dn_logx;
  DEC_TYPE two = DFP_CONSTANT(2.0);

  FUNC_CONVERT_TO_DN (&x, &dn_x);
  FUNC_CONVERT_TO_DN (&two, &dn_two);

  if (___decNumberIsNaN (&dn_x))
    return x+x;
  if (___decNumberIsZero (&dn_x))	/*  If x == 0: Pole Error */
    {
      DFP_EXCEPT (FE_DIVBYZERO);
      return -DFP_HUGE_VAL;
    }
  if (___decNumberIsNegative (&dn_x))	/*  If x < 0,: Domain Error */
    {
      DFP_EXCEPT (FE_INVALID);
      return DFP_NAN;
    }
  if (___decNumberIsInfinite (&dn_x))
    return x;

  ___decContextDefault (&context, DEFAULT_CONTEXT);
  // log_2 (x) = log_10 (x) / log_10 (2)
  ___decNumberLog10 (&dn_logx, &dn_x, &context);
  ___decNumberLog10 (&dn_logtwo, &dn_two, &context);
  ___decNumberDivide (&dn_result, &dn_logx, &dn_logtwo, &context);

  FUNC_CONVERT_FROM_DN (&dn_result, &result, &context);

  return result;
}

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x)
{
  DEC_TYPE z = IEEE_FUNCTION_NAME (x);
#ifndef _IEEE_LIBDFP
  if (_LIB_VERSION == _IEEE_) return z;
  if (x == DFP_CONSTANT(0.0))
    DFP_ERRNO (ERANGE);
  if (x < DFP_CONSTANT(0.0))
    DFP_ERRNO (EDOM);
#endif
  return z;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
