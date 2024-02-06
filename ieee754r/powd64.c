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


#if defined(_DECIMAL_SIZE) && _DECIMAL_SIZE != 64
#error "This implementation only works for _Decimal64"
#endif

#define _DECIMAL_SIZE 64

#define FUNCTION_NAME pow

#include <math.h>
#include <ieee754r_private.h>
#include <decimal64.h>
#include <dfpmacro.h>
#include <limits.h>
#include <stdint.h>

/* The actual value is 9223372036854775807, this needs to round down for exact
   conversion.  */
#define D64_INT64_MAX   9223372036854775000.DD

/* Compute integer powers with higher precision to minimize rounding error.  */
static _Decimal128
intpow (_Decimal128 val, uint64_t exp)
{
  _Decimal128 result = 1;
  _Decimal128 p = val;

  for ( ; exp; exp >>= 1, p*=p)
    if (exp&1)
      result *= p;

  return result;
}

/* Only for _Decimal64 and smaller today. */
static bool
isodd (_Decimal64 y)
{
  unsigned long int y_abs_int = __fabsd64 (y);
  return __rintd64 (y) == y && __fabsd64 (y) <= 9999999999999999.DD && (y_abs_int & 1) == 1;
}

static _Decimal64
IEEE_FUNCTION_NAME (_Decimal64 x, _Decimal64 y)
{
  _Decimal128 result = DEC_NAN;
  _Decimal64 y_int;
  _Decimal64 y_frac;

  /* For any y, including a NaN. */
  if (x == 1.DD)
    return x;

  int x_class = __fpclassifyd64 (x);
  int y_class = __fpclassifyd64 (y);
  if (!(x_class == FP_NORMAL && y_class == FP_NORMAL))
    {
      if (y_class == FP_SUBNORMAL && y_class == FP_SUBNORMAL)
	{
	  /* Handle normally */
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
	    return y < 0.DD ? DEC_INFINITY : __copysignd64(0.DD, x);
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
  y_int = __rintd64 (y);
  result = 1.DL;

  /* Compute x^(int(y)) first, if abs(int(y)) fits inside a long int.  */
  if (__fabsd64 (y_int) <= D64_INT64_MAX)
    {
      int64_t int_y = y_int;	/* convert to int */
      if (int_y == 0)
	{
	  /* Do nothing. result is already 1. */
	}
      else if (int_y > 0)
	{
	  result = intpow (x, int_y);
	}
      else
	{
	  result = 1.DL / intpow (x, -int_y);
	}
    }
  else
    {
      /* TODO: the following will likely be inaccurate, but hopefully not blatantly wrong.
               Fixing this requires a better implementation.  */
      y_int = 0.DD;
    }

  y_frac = y - y_int;

  /* Compute exp(ln(x)*y_frac) and combine with integral power with the highest available precision. */
  if (y_frac != 0.DD)
    {
      /* If x is negative, log_x should propogate a NaN upwards. */
      _Decimal128 log_x = __logd128 (x);
      result = result * __expd128 ((_Decimal128)y_frac * log_x);
    }

  return result;
}

_Decimal64
INTERNAL_FUNCTION_NAME (_Decimal64 x, _Decimal64 y)
{
  _Decimal64 z = IEEE_FUNCTION_NAME (x, y);
  /* Pole error: x = 0, y < 0 (non-inf). Set ERANGE in accordance with C99 */
  if (x == 0.DD && __isfinited64 (y) && y < 0.DD)
    DFP_ERRNO (ERANGE);
  if (!__isfinited64 (z) && __isfinited64 (x) && __isfinited64 (y))
    {
      if (__isnand64 (z))	/*  Domain error was triggered, x < 0 and y was not an odd int */
	DFP_ERRNO (EDOM);
      else			/*  Overflow */
	DFP_ERRNO (ERANGE);
    }
  return z;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
