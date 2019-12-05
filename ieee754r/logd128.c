/* Calculate the Natural Log of a given a _Decimal128 value

   This is fairly straightforward transformation of the glibc
   float128 logl as noted below.

   Copyright (C) 2019 Free Software Foundation, Inc.
   Copyright 2001 by Stephen L. Moshier <moshier@na-net.ornl.gov>

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
#include <ieee754r_private.h>

#define _DECIMAL_SIZE 128
#include <dfpmacro.h>

#include "dfp_inline.h"

#include "t_logd128.h"

/* log(1+x) = x - .5 x^2 + x^3 l(x)
   -.0078125 <= x <= +.0078125
   peak relative error 1.2e-37 */
static const _Decimal128
  l3 = 3.333333333333333333333333333333336096926E-1DL,
  l4 = -2.499999999999999999999999999486853077002E-1DL,
  l5 = 1.999999999999999999999999998515277861905E-1DL,
  l6 = -1.666666666666666666666798448356171665678E-1DL,
  l7 = 1.428571428571428571428808945895490721564E-1DL,
  l8 = -1.249999999999999987884655626377588149000E-1DL,
  l9 = 1.111111111111111093947834982832456459186E-1DL,
  l10 = -1.000000000000532974938900317952530453248E-1DL,
  l11 = 9.090909090915566247008015301349979892689E-2DL,
  l12 = -8.333333211818065121250921925397567745734E-2DL,
  l13 = 7.692307559897661630807048686258659316091E-2DL,
  l14 = -7.144242754190814657241902218399056829264E-2DL,
  l15 = 6.668057591071739754844678883223432347481E-2DL;

/* 4 digits of precision (max exp has 4 digit) */
static _Decimal128 ln10a = 2.302DL;
static _Decimal128 ln10b = 0.585092994045684017991454684364207601e-3DL;

_Decimal128
__logd128 (_Decimal128 x)
{
  _Decimal128 u, y, t, z, w;
  int e, m;

  if (__isnand128 (x))
    return x + x;
  else if (x <= 0.0DL)
    {
      if (x == 0.0DL)
	{
	  DFP_ERRNO (EDOM);
	  return -DEC_INFINITY;
	}
      else
	{
	  DFP_ERRNO (ERANGE);
	  return DFP_NAN;
	}
    }
  else if (__isinfd128 (x))
    return DEC_INFINITY;

  u = __frexpd128 (x, &e);

  /* u is .1 <= f < 1 */

  /* Round to the nearest 3 digit number. RTN gives the closest. */
  m = __quantized128 (u * 1000.0DL, 2.0e33DL - 0.01DL) - LNTVALS_START;

  /* Recenter the normalized value around .20 <= x < 1.20.
     This reduces the x s.t |x| < .0078125 to ensure the polynomial approx
     holds within sufficient error. It is about .005 for non-RTN, .0025 RTN
     against t = 0.20 */
  if (m < LNTVALS_START)
    {
      e -= 1;
      u *= 10.0DL;		/* TODO: incrementing the exponent could be faster. */
    }

  /* Note, values can only be 100-999 after frexp */
  t = lntvals[m].t;

  /* Avoid cancellation error when values close to 1. */
  if (x >= 0.999DL && x <= 1.001DL)
    {
      if (x == 1.0DL)
	return 0.0DL;
      /* Avoid the table */
      z = x - 1.0DL;
      m = 0;
      t = lntvals[m].t;
      printf ("no table\n");
    }
  else
    {
      /* log(u) = log(u*t/u) = log(t) + log(u/t) where
         log(u/t) = log(1+z) where z = u/t - 1 = (u-t)/t. */
      z = (u - t) / t;
    }
  w = z * z;
  y =
    ((((((((((((l15 * z + l14) * z + l13) * z + l12) * z + l11) * z +
	    l10) * z + l9) * z + l8) * z + l7) * z + l6) * z + l5) * z +
      l4) * z + l3) * z * w;
  y -= 0.5DL * w;
  y += e * ln10b;
  y += z;
  y += lntvals[m].lnt;

  /* |e * ln10a| bounded to about 2.3 and 14000. Subtract from this to avoid error on small values e.g x < ln(10) */
  if (e != 0)
    {
      y += t;
      y += e * ln10a - 1.0DL;
    }
  else
    {
      y += t - 1.0DL;
    }
  return y;
}

weak_alias (__logd128, logd128)
