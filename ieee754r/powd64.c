/* Decimal Floating Point 64-bit x^y.

   Copyright (C) 2008-2015 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Steven Munroe <munroesj@us.ibm.com>
              Ryan S. Arnold <rsa@us.ibm.com>

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

#include <math.h>
#include <ieee754r_private.h>

#ifndef _DECIMAL_SIZE
#define _DECIMAL_SIZE 64
#include <decimal64.h>
#endif

#define FUNCTION_NAME pow

#include <dfpmacro.h>

#include <stdint.h>



static DEC_TYPE
intpow (DEC_TYPE val, int N)
{
  DEC_TYPE result = 1;
  DEC_TYPE p = val;
  int mask = 1;

  while (mask <= N)
    {
      if (mask & N)
	result *= p;
      p *= p;
      mask <<= 1;
    }

  return result;
}

/* Only for _Decimal64 and smaller today. */
static bool
isodd (DEC_TYPE y)
{
  DEC_TYPE y_int = FUNC_D (__rint)(y);
  unsigned long int y_abs_int = FUNC_D (__fabs)(y_int);
  return y_int == y && y_abs_int < 9999999999999999ULL && (y_abs_int & 1) == 1;
}

static DEC_TYPE
IEEE_FUNCTION_NAME (DEC_TYPE x, DEC_TYPE y)
{
  DEC_TYPE result = DEC_NAN;
  DEC_TYPE y_int;
  DEC_TYPE y_frac;

  int x_class = FUNC_D (__fpclassify) (x);
  int y_class = FUNC_D (__fpclassify) (y);
  if (!(x_class == FP_NORMAL && y_class == FP_NORMAL))
    {
      if (y_class == FP_SUBNORMAL && y_class == FP_SUBNORMAL)
	{
	  /* Handle normally */
	}
      else if (x == 1.DD)
	{
	  /* For any y, including a NaN. */
	  return x;
	}
      else if (y_class == FP_NAN || x_class == FP_NAN)
	{
	  return y_class == FP_ZERO ? 1.DD : x + y;
	}
      else if (y_class == FP_ZERO)
	{
	  return x_class == FP_NAN ? x : 1.DD;
	}
      else if (x_class == FP_ZERO)
	{
	  if (y_class == FP_INFINITE)
	    return y < 0.DD ? DEC_INFINITY : 0.DD;
	  if (isodd(y))
	    return y < 0.DD ? DEC_INFINITY : FUNC_D (__copysign)(0.DD, x);
	  return y < 0.DD ? DEC_INFINITY : 0.DD;
	}
      else if (y_class == FP_INFINITE)
	{
	  if (x == -1.DD)
	    return 1.DD;
	  else if (x < -1.DD || x > 1.DD)
	    return y > 0.DD ? DEC_INFINITY : 0.DD;
	  else
	    return y > 0.DD ? 0.DD : DEC_INFINITY;
	}
      else if (x_class == FP_INFINITE)
	{
	  if (x > 0.DD)
	    return y < 0.DD ? 0.DD : DEC_INFINITY;
	  /* If x is -INF, the result is determined by the sign of y and whether y is an odd integer. */
	  if (isodd(y))
	    return y < 0.DD ? -0.DD : x;
	  return y < 0.DD ? 0.DD : -x;
	}
      else
	{
	  /* This should be reachable. */
	  return DEC_NAN;
	}
    }

  /* x and y are non-zero, finite, and not one.
     Split y into integer and fraction and use the identity:
     x ^ (m+n) == ( x^m ) * (x^n) */
  y_int = FUNC_D (__rint) (y);
  y_frac = y - y_int;
  result = 1.DD;

  /* Compute x^(int(y)) first */
  if (y_int != 0.DD)
    {				/* y is an integer */
      long int int_y = y_int;	/* convert to int */
      if (int_y > 0)
	{
	  result = intpow (x, int_y);
	}
      else
	{
          /* y is negative so use a^-x == 1/(a^x)
	     Split int_y into two parts to avoid overflowing the divisor (e.x 2^-1279). */
	  long int y1 = (-int_y)/2;
	  DEC_TYPE v1 = intpow (x, y1);
	  DEC_TYPE v2 = v1;
          if ((y1 & 1) == 1)
	    v2 *= x;
	  DEC_TYPE v3 = v2 * v1;
          if ( FUNC_D (__isnormal)(v3))
            result = DFP_CONSTANT(1.0) / v3;
          else
	    result = (DFP_CONSTANT(1.0) / v1) * (DFP_CONSTANT(1.0) / v2);
	}
    }

  /* Compute exp(ln(x)*y_frac) and combine with integral power. */
  if (y_frac != 0.DD)
    {
      /* If x is negative, log_x should propogate a NaN upwards. */
      DEC_TYPE log_x = FUNC_D (__log) (x);
      result = result * FUNC_D (__exp) (y_frac * log_x);
    }

  return result;
}

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x, DEC_TYPE y)
{
  DEC_TYPE z = IEEE_FUNCTION_NAME (x, y);
  /* Pole error: x = 0, y < 0 (non-inf). Set ERANGE in accordance with C99 */
  if (x == DFP_CONSTANT (0.0) && FUNC_D (__isfinite) (y)
      && y < DFP_CONSTANT (0.0))
    DFP_ERRNO (ERANGE);
  if (!FUNC_D (__isfinite) (z) && FUNC_D (__isfinite) (x)
      && FUNC_D (__isfinite) (y))
    {
      if (FUNC_D (__isnan) (z))	/*  Domain error was triggered, x < 0 and y was not an
				   odd int */
	DFP_ERRNO (EDOM);
      else			/*  Overflow */
	DFP_ERRNO (ERANGE);
    }
  return z;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
