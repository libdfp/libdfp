/* Returns the remainder of a floating point division

   This implementation is based off that presented in
   "The Mathematical-Function Computation Handbook" by
   Nelson H.F. Beebe.

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2007-2015 Free Software Foundation, Inc.

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
   if not, write to the Free Software Foundation, Inc., 51 Franklin
   Street, Fifth Floor, Boston, MA 02110-1301 USA.

   Please see libdfp/COPYING.txt for more information.  */

#ifndef _DECIMAL_SIZE
#include <decimal32.h>
#define _DECIMAL_SIZE 32
#endif

#include <math.h>
#include <errno.h>
#include <ieee754r_private.h>

#define FUNCTION_NAME fmod

#include <dfpmacro.h>

#if _DECIMAL_SIZE == 128
typedef struct
{
  _Decimal128 hi, lo;
} dp128_pair_t;

/* TODO: Could this be simplified as y.lo is always 0, and x is always positive? */
static dp128_pair_t
PSUM2 (_Decimal128 x, _Decimal128 y)
{
  _Decimal128 sum, err;
  dp128_pair_t ret;

  /* The simplified version where x > y, always. */
  sum = x + y;
  err = x - sum;
  err += y;

  ret.hi = sum;
  ret.lo = err;
  return ret;
}

static dp128_pair_t
PSUB (dp128_pair_t x, _Decimal128 y)
{
  _Decimal128 r, s;

  r = x.hi - y;

  /* The simplified version where x > y, always. */
  s = x.hi - r;
  s -= y;
  s += x.lo;

  return PSUM2 (r, s);
}
#endif /* _DECIMAL_SIZE == 128 */

/* Computing the remainder requires a larger type for holding the remainder.  */
#if _DECIMAL_SIZE >= 64
typedef _Decimal128 DEC_TYPE_REMAINDER;
#define FREXP_RTYPE __frexpd128
#define LDEXP_RTYPE __ldexpd128
#else
typedef _Decimal64 DEC_TYPE_REMAINDER;
#define FREXP_RTYPE __frexpd64
#define LDEXP_RTYPE __ldexpd64
#endif /* _DECIMAL_SIZE >= 64 */

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x, DEC_TYPE y)
{
  int xc = FUNC_D (__fpclassify) (x);
  int yc = FUNC_D (__fpclassify) (y);

  if (xc != FP_NORMAL || yc != FP_NORMAL)
    {
      if (xc == FP_INFINITE || yc == FP_INFINITE || xc == FP_NAN
	  || yc == FP_NAN || yc == FP_ZERO)
	{
	  DFP_ERRNO (EDOM);
	  return NAN;
	}
    }

  /* x and y are both finite values. */
  DEC_TYPE xabs = FUNC_D (__fabs) (x);
  DEC_TYPE yabs = FUNC_D (__fabs) (y);
  DEC_TYPE result;

  if (xabs < yabs)
    result = x;
  else if (xabs == yabs)
    result = FUNC_D (__copysign) (0.DD, x);
  else
    {
      DEC_TYPE_REMAINDER r, t, yabs128;	/* This algo requires n+1 digits of precision to be correct in decimal. */
#if _DECIMAL_SIZE == 128
      DEC_TYPE_REMAINDER r_rem;
#endif
      int nr, ny;

      r = xabs;
      yabs128 = yabs;
      FREXP_RTYPE (yabs128, &ny);

      while (r >= yabs128)
	{
	  DEC_TYPE_REMAINDER yabs_scaled;
	  int n;

	  FREXP_RTYPE (r, &nr);
	  n = nr - ny;
	  yabs_scaled = LDEXP_RTYPE (yabs128, n);

	  if (yabs_scaled > r)
	    yabs_scaled = LDEXP_RTYPE (yabs128, n - 1);

	  t = r - yabs_scaled;

#if _DECIMAL_SIZE == 128
	  /* For _Decimal128, we need an extra digit if the subtraction is not exact, and there is no larger format, use pair precision. */
	  r_rem = (r - t) - yabs_scaled;
	  if (r_rem != 0 && t >= yabs128)
	    {
	      /* Subtract using pair precision until exact, (1-9) operations. */
	      dp128_pair_t xp = { t, r_rem };
	      int n = 0;
	      do
		{
		  xp = PSUB (xp, yabs_scaled);
		  n++;
		}
	      while (n < 10 && xp.lo != 0.DD);
	      t = xp.hi;
	    }
#endif
	  r = t;
	}

      result = (x < 0.DD) ? -r : r;
    }
  return result;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
