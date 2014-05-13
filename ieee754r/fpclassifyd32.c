/* Returns the type of floating point number from a _Decimal32 type

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2007-2014 Free Software Foundation, Inc.

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
   if not, write to the Free Software Foundation, Inc., 59 Temple Place,
   Suite 330, Boston, MA 02111-1307 USA.

   Please see libdfp/COPYING.txt for more information.  */

#include <math.h>
#include <math_private.h>
#include <ieee754r_private.h>

#define _DECIMAL_SIZE 32
#include <decimal32.h>
#include <dfpmacro.h>

int
__fpclassifyd32 (_Decimal32 x)
{
  decNumber dn_x;
  decContext context;

  FUNC_CONVERT_TO_DN (&x, &dn_x);

  if (decNumberIsNaN (&dn_x))
    return FP_NAN;
  else if (decNumberIsInfinite (&dn_x))
    return FP_INFINITE;
  else if (decNumberIsZero (&dn_x))
    return FP_ZERO;

  /* Since DFP value are not normalized, checking the exponent for
     normal/subnormal is not suffice.  For instance, the value 10e-96 will
     result in a expoenent below the minimum, however it is still a FP_NORMAL
     number due implicit normalization.  TO avoid such traps the check relies
     on runtime comparisons.  */
  decContextDefault (&context, DEC_INIT_DECIMAL32);
  if (decNumberIsSubnormal (&dn_x, &context))
    return FP_SUBNORMAL;

  return FP_NORMAL;
}

hidden_def (__fpclassifyd32)
weak_alias (__fpclassifyd32, fpclassifyd32)
