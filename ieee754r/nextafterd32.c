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
   if not, write to the Free Software Foundation, Inc., 51 Franklin
   Street, Fifth Floor, Boston, MA 02110-1301 USA.

   Please see libdfp/COPYING.txt for more information.  */

#ifndef _DECIMAL_SIZE
# include <decimal32.h>
# define _DECIMAL_SIZE 32
# define SUBNORMAL_MIN DEC32_SUBNORMAL_MIN
# define DEC_MAX DEC32_MAX
#endif

#include <math.h>
#include <float.h>
#include <errno.h>
#include <ieee754r_private.h>
#include <dfpfenv_private.h>

#define FUNCTION_NAME nextafter

#include <dfpmacro.h>
#include <numdigits.h>
static DEC_TYPE
IEEE_FUNCTION_NAME (DEC_TYPE x, DEC_TYPE y)
{
  DEC_TYPE epsilon = SUBNORMAL_MIN;
  int nrm = FE_DEC_UPWARD;
  int orm;

  if (x == y)
    {
      /* Special case: (0,-0) = -0, (-0,0) = 0.  */
      if (x == 0)
        {
          int negx = FUNC_D(__signbit)(x);
          int negy = FUNC_D(__signbit)(y);

          if (!negx && negy)
            return -0.D;
          if (negx && !negy)
            return 0.D;
        }
       return x;
    }
  else if (x != x)
    return x;
  else if (y != y)
    return y;
  else if (x > y)
    {
      if (x == SUBNORMAL_MIN) /* Special case: this value always return 0.  */
        return 0.D;
      if (x == DEC_INFINITY)
	return DEC_MAX;
      epsilon = -epsilon;
      nrm = FE_DEC_DOWNWARD;
    }
  else
    {
      if (x == -SUBNORMAL_MIN) /* Special case: this value always return -0.  */
        return -0.D;
      if (x == -DEC_INFINITY)
	return -DEC_MAX;
    }

  /* Add or subtract the minimal amount, and round in direction of epsilon.  */
  orm = __fe_dec_getround ();
  __fe_dec_setround (nrm);
  epsilon += x;
  __fe_dec_setround (orm);

  return epsilon;
}

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x, DEC_TYPE y)
{
  DEC_TYPE z = IEEE_FUNCTION_NAME (x, y);
  if (!FUNC_D (__isfinite) (z) && FUNC_D (__isfinite) (x))
    DFP_ERRNO (ERANGE);
  /* TODO: Handle underflow here */
  return z;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
