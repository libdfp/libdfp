/* Calculates the cube root function of _Decimal32 type x

   Copyright (C) 1984, 1991 Stephen L. Moshier.
   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2001, 2007, 2009 Free Software Foundation, Inc.

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
   if not, write to the Free Software Foundation, Inc., 59 Temple Place,
   Suite 330, Boston, MA 02111-1307 USA.

   Please see libdfp/COPYING.txt for more information.  */

#ifndef _DECIMAL_SIZE
#  include <decimal32.h>
#  define _DECIMAL_SIZE 32
#endif

#include <math.h>
//#include "math_private.h"

#define FUNCTION_NAME cbrt

#include <dfpmacro.h>

/*
 *  Provided by the Cephes Math Library Release 2.2: January, 1991
 *  By Stephen L. Moshier
 *  Adapted for glibc October, 2001.
 *  Adapted for libdfp 2006, and donated to the FSF in 2007.
 *
 * DESCRIPTION:
 * Returns the cube root of the argument, which may be negative.
 *
 * Range reduction involves determining the power of 2 of
 * the argument.  A polynomial of degree 2 applied to the
 * mantissa, and multiplication by the cube root of 1, 2, or 4
 * approximates the root to within about 0.1%.  Then Newton's
 * iteration is used three times to converge to an accurate
 * result.
 *
 * ACCURACY:
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE       -8,8       100000      1.3e-34     3.9e-35
 *    IEEE    exp(+-707)    100000      1.3e-34     4.3e-35
 */

/* Replace with:
 * cbrt(10) and cbrt(100)
 * and
 * cbrt(-10) and cbrt(-100)
 *
 * used 'bc' to get these and then trunced by hand.
 *  echo 'scale=44;e(1/3*(l(10)))' | bc -l
 *  echo 'scale=44;e(1/3*(l(100)))' | bc -l
 *  echo 'scale=45;e(1/3*(l(1/10)))' | bc -l
 *  echo 'scale=45;e(1/3*(l(1/100)))' | bc -l
 */
static const _Decimal128
	CBRT10   = 2.154434690031883721759293566519350495259345DL,
	CBRT100  = 4.641588833612778892410076350919446576551349DL,
	CBRT10I  = 0.4641588833612778892410076350919446576551349DL,
	CBRT100I = 0.2154434690031883721759293566519350495259345DL;

//	CBRT2 = 1.259921049894873164767210607278228350570251DL,
//	CBRT4 = 1.587401051968199474751705639272308260391493DL,
//	CBRT2I= 0.7937005259840997373758528196361541301957467DL,
//	CBRT4I= 0.6299605249474365823836053036391141752851257DL;

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x)
{
  int e, rem, sign;
  _Decimal128 z;

  if (! FUNC_D(__finite) (x)) /* cbrt(x:x=inf/nan/-inf) = x+x (for sNaN) */
    return x + x;

  if (x == DFP_CONSTANT(0.0)) /*  cbrt(0) = 0 */
    return (x);

  if (x > DFP_CONSTANT(0.0))
    sign = 1;
  else
    {
      sign = -1;
      x = -x;
    }

  z = x;
 /* extract power of 2, leaving mantissa between 0.5 and 1  */
  x = FUNC_D(frexp) (x, &e);

  /* Approximate cube root of number between .5 and 1,
     peak relative error = 1.2e-6  */
  x = ((((1.3584464340920900529734e-1DL * x
	  - 6.3986917220457538402318e-1DL) * x
	 + 1.2875551670318751538055e0DL) * x
	- 1.4897083391357284957891e0DL) * x
       + 1.3304961236013647092521e0DL) * x + 3.7568280825958912391243e-1DL;

  /* exponent divided by 3 */
  if (e >= 0)
    {
      rem = e;
      e /= 3;
      rem -= 3 * e;
      if (rem == 1)
	//x *= CBRT2;
	x *= CBRT10;
      else if (rem == 2)
	//x *= CBRT4;
	x *= CBRT100;
    }
  else
    {				/* argument less than 1 */
      e = -e;
      rem = e;
      e /= 3;
      rem -= 3 * e;
      if (rem == 1)
	//x *= CBRT2I;
	x *= CBRT10I;
      else if (rem == 2)
	//x *= CBRT4I;
	x *= CBRT100I;
      e = -e;
    }

  /* multiply by power of 2 */
  x = FUNC_D(ldexp) (x, e);

  /* Newton iteration */
  x -= (x - (z / (x * x))) * 0.3333333333333333333333333333333333333333DL;
  x -= (x - (z / (x * x))) * 0.3333333333333333333333333333333333333333DL;
  x -= (x - (z / (x * x))) * 0.3333333333333333333333333333333333333333DL;

  if (sign < 0)
    x = -x;
  return (x);
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
