/* The fminmag functions determine the value of their argument
   whose magnitude is the minimum of  the  magnitudes  of  the  arguments:
   the value of y if |x| > |y|, x if |x| < |y|, and fmin(x,y) otherwise.

   Copyright (C) 2019-2020 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Pedro Caldeira <caldeira@linux.ibm.com>

   The Decimal Floating Point C Library is free software; you can
   redistribute it and/or modify it under the terms of the GNU Lesser
   General Public License version 2.1.

   The Decimal Floating Point C Library is distributed in the hope that
   it will be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
   the GNU Lesser General Public License version 2.1 for more details.

   You should have received a copy of the GNU Lesser General Public
   License version 2.1 along with the Decimal Floating Point C Library;
   if not, write to the Free Software Foundation, Inc., 51 Franklin
   Street, Fifth Floor, Boston, MA 02110-1301 USA.

   Please see libdfp/COPYING.txt for more information.  */

#ifndef _DECIMAL_SIZE
#  define _DECIMAL_SIZE 32
#  include <decimal32.h>
#endif

#include <decContext.h>
#include <decNumber.h>
#include <math.h>
#include <ieee754r_private.h>

#define FUNCTION_NAME fminmag

#include <dfpmacro.h>

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x, DEC_TYPE y)
{
  DEC_TYPE ax, ay;
  ax = FUNC_D(__fabs) (x);
  ay = FUNC_D(__fabs) (y);
  if (FUNC_D(__isless) (ax, ay)) return x;
  if (FUNC_D(__isless) (ay, ax)) return y;
  return FUNC_D(__fmin) (x, y);
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
