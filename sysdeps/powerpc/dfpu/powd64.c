/* Decimal Floating Point 64-bit x^y.  POWER6 PowerPC64 version.

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
   if not, write to the Free Software Foundation, Inc., 59 Temple Place,
   Suite 330, Boston, MA 02111-1307 USA.

   Please see libdfp/COPYING.txt for more information.  */

#include <math.h>
#include <ieee754r_private.h>

#ifndef _DECIMAL_SIZE
#  define _DECIMAL_SIZE 64
#  include <decimal64.h>
#endif

#define FUNCTION_NAME pow

#include <dfpmacro.h>

static DEC_TYPE 
intpow (DEC_TYPE val, int N)
{
  DEC_TYPE result = 1;
  DEC_TYPE p = val;
  int mask = 1;

  while (mask <= N)
    {
      if (mask & N) result *= p;
      p *= p;
      mask <<= 1;
    }

  return result;
}


static DEC_TYPE
IEEE_FUNCTION_NAME (DEC_TYPE x, DEC_TYPE y)
{
  DEC_TYPE result = DEC_NAN;
  DEC_TYPE y_int;
  DEC_TYPE y_frac;

  int x_class = FUNC_D(__fpclassify) (x);
  int y_class = FUNC_D(__fpclassify) (y);
  if ((x_class == FP_NAN) || (x_class == FP_INFINITE)
  ||  (y_class == FP_NAN) || (y_class == FP_INFINITE))
    {
      if ((x_class == FP_NAN) || (y_class == FP_NAN))
	return DEC_NAN;
      if (x == 1.DD)
	return 1.DD;
      if (y == 0.DD)
	return 1.DD;
      if (y_class == FP_INFINITE && (x_class != FP_NAN))
	{
	  DEC_TYPE x_abs = FUNC_D(__fabs) (x);
	  if (x == -1.DD)
	    return 1.DD;
	  if (y < 0.DD)
	    { /* y == -inf */
	      if (x_abs < 1.DD)
		return DEC_INFINITY;
	      if (x_abs > 1.DD)
		return 0.DD;
	    }
	  else
	    { /* y == +inf */
	      if (x_abs < 1.DD)
		return 0.DD;
	      if (x_abs > 1.DD)
		return DEC_INFINITY;
	    }
	}
      if (x_class == FP_INFINITE && (y_class != FP_NAN))
	{
	  DEC_TYPE y_int = FUNC_D(__trunc) (y);
	  if (y_int == y)
	    { /* y is an int including 0. */
	      long long int_y = y_int;
	      int odd_y = (int)(int_y & 1LL);
	      if (odd_y)
		{ /* y is an odd integer */
		   if ((y < 0.DD) && (x < 0.DD))
		     return -0.DD;
		   if ((y > 0.DD) && (x < 0.DD))
		     return -DEC_INFINITY;
		}
	      else
		{ /* y not an odd integer */
		  if ((y < 0.DD) && (x < 0.DD))
		    return 0.DD;
		  if ((y > 0.DD) && (x < 0.DD))
		    return DEC_INFINITY;
		}
	    }
	  else
	    {
	      if (x > 0.DD)
		{ /* x == +inf */
		  if (y < 0.DD)
		    return 0.DD;
		  else 
		    return DEC_INFINITY;
		}
	    }
	  return DEC_NAN;
	}
    }
  /* handle the eazy case */
  if (y_class == FP_ZERO)
    return 1.DD;

  /* Split y into integer and fraction and use the identity:
     x ^ (m+n) == ( x^m ) * (x^n) */
  y_int = FUNC_D(__rint) (y);
  y_frac = y - y_int;

#ifdef __DEBUG_PRINT__
  printf ("y_int=%Df,y_frac=%Df)\n",
	y_int, y_frac);
#endif
  if (y_frac == 0.DD)
    { /* y is an integer */
      /* FIXME does not handle case where y_int is out of range for
	 int */
      int int_y = y_int; /* convert to int */
      if (y_int > 0.DD)
	{
	  int odd_y = int_y & 1;
	  if (x == 0.0DD)
	    {
	      if ( odd_y )
		result = x;
	      else
		result = 0.DD;
	    }
	  else
	    {
	      result = intpow(x, int_y);
	    }
	}
      else
	{ /* y is negative so use a^-x == 1/(a^x) */
	  int odd_y = int_y & 1;
#ifdef __DEBUG_PRINT__
  printf ("int_y=%d, odd_y=%d)\n",
	int_y, odd_y);
#endif
	  if (x == 0.DD)
	    { /* pole error */
	      result = HUGE_VAL_D64;
	      if ( odd_y )
		result = FUNC_D(__copysign) (result, x);
	    }
	  else
	    result = 1.DD / intpow(x, -int_y);
	}
      /* else y == 0.0, and handled as special case above. */
    }
  else
    { /* y is not a integer */
      if (x == 1.DD)
	result = 1.DD;
      else if (x == 0.DD)
	{
	  if (y > 0.DD)
	    result = 0.DD;
	  else if (y < 0.DD)
	    result = HUGE_VAL_D64; /* pole error */
	  /* else y == 0.0, and handled as special case above. */
	}
      else
	{ /* FIXME lots of additional special cases here!  */
	  int int_y = y_int; /* convert to int */
	  DEC_TYPE log_x;

	  log_x = FUNC_D(__log) (x);
	  result = intpow(x, int_y) * FUNC_D(__exp) (y_frac * log_x);
	}
    }

  return result;
}

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x, DEC_TYPE y)
{
  DEC_TYPE z = IEEE_FUNCTION_NAME (x, y);
  /* Pole error: x = 0, y < 0 (non-inf). Set ERANGE in accordance with C99 */
  if (x == DFP_CONSTANT(0.0) && FUNC_D(__isfinite)(y) && y < DFP_CONSTANT(0.0))
      DFP_ERRNO (ERANGE);
  if (!FUNC_D(__isfinite) (z) && FUNC_D(__isfinite) (x) && FUNC_D(__isfinite) (y))
    {
      if (FUNC_D(__isnan) (z)) /*  Domain error was triggered, x < 0 and y was not an
				   odd int */
	DFP_ERRNO (EDOM);
      else	/*  Overflow */
	DFP_ERRNO (ERANGE);
    }
  return z;
}

weak_alias(INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
