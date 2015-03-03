/* Compute the quantum exponend of a finite argument as long long int.

   Copyright (C) 2014-2015 Free Software Foundation, Inc.

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

#ifndef _DECIMAL_SIZE
# define _DECIMAL_SIZE 32
# include <decimal32.h>
#endif

#include <decContext.h>
#include <decNumber.h>
#include <math.h>
#include <limits.h>

#include <decNumberMath.h>

#define FUNCTION_NAME llquantexp

#include <dfpmacro.h>
#include <numdigits.h>

long long int
INTERNAL_FUNCTION_NAME (DEC_TYPE x)
{
  decNumber dn_x;

  FUNC_CONVERT_TO_DN(&x, &dn_x);
  if (decNumberIsNaN (&dn_x) || decNumberIsInfinite (&dn_x))
    {
      DFP_ERRNO (EDOM);
      return LLONG_MIN;
    }
  
  return (long long int)dn_x.exponent;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
