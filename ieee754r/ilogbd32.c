/* Returns the unbiased exponent of the passed _Decimal32 value as an int

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
   if not, write to the Free Software Foundation, Inc., 51 Franklin
   Street, Fifth Floor, Boston, MA 02110-1301 USA.

   Please see libdfp/COPYING.txt for more information.  */

#ifndef _DECIMAL_SIZE
#  include <decimal32.h>
#  define _DECIMAL_SIZE 32
#endif

#ifndef _RETURN_TYPE
#  define _RETURN_TYPE int
#endif

#ifndef _MAX_VALUE
#  define _MAX_VALUE   INT_MAX
#endif
#ifndef _MIN_VALUE
#  define _MIN_VALUE   INT_MIN
#endif

#ifndef _FBLOG0
#  define _FBLOG0      FP_ILOGB0
#endif
#ifndef _FBLOGNAN
#  define _FBLOGNAN    FP_ILOGBNAN
#endif

#ifndef FUNCTION_NAME
#  define FUNCTION_NAME ilogb
#endif

#include <errno.h>
#include <limits.h>
#include <math.h>
#include <dfpmacro.h>
#include <ieee754r_private.h>

_RETURN_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x)
{
  /* Assume an int can hold any decimal type exponent. */
  int c = FUNC_D (__fpclassify) (x);
  int exp;

  if (c != FP_NORMAL)
    {
      if (x != x)
	{
	  DFP_ERRNO (EDOM);
	  return _FBLOGNAN;
	}
      else if (c == FP_ZERO)
	{
	  DFP_ERRNO (EDOM);
	  return _FBLOG0;
	}
      else if (c == FP_INFINITE)
	{
	  DFP_ERRNO (EDOM);
	  return FUNC_D (__signbit) (x) ? _MIN_VALUE : _MAX_VALUE;
	}
      /* And handle subnormals like any other finite value. */
    }
  /* A simpler approach would be to truncate the value to 1 digit in the least
     significant digit and return the exponent, frexp essentially does that.  */
  FUNC_D (__frexp) (x, &exp);
  return exp - 1;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
