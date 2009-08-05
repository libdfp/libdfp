/* Calculates the arcsin(x)

   Copyright (C) 2001 Stephen L. Moshier <moshier@na-net.ornl.gov>
   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2001, 2007, 2009 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s) Joseph Kerian <jkerian@us.ibm.com>

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
#include <errno.h>

/* Portions of this code are:
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 *
 * This was adapted for glibc in 2001.
 * This was adapted for Libdfp in 2006, and those changes donated to the FSF in
 * 2007.
 *
 * __ieee754_asin(x)
 * Method :
 *	Since  asin(x) = x + x^3/6 + x^5*3/40 + x^7*15/336 + ...
 *	we approximate asin(x) on [0,0.5] by
 *		asin(x) = x + x*x^2*R(x^2)
 *      Between .5 and .625 the approximation is
 *              asin(0.5625 + x) = asin(0.5625) + x rS(x) / sS(x)
 *	For x in [0.625,1]
 *		asin(x) = pi/2-2*asin(sqrt((1-x)/2))
 *	Let y = (1-x), z = y/2, s := sqrt(z), and pio2_hi+pio2_lo=pi/2;
 *	then for x>0.98
 *		asin(x) = pi/2 - 2*(s+s*z*R(z))
 *			= pio2_hi - (2*(s+s*z*R(z)) - pio2_lo)
 *	For x<=0.98, let pio4_hi = pio2_hi/2, then
 *		f = hi part of s;
 *		c = sqrt(z) - f = (z-f*f)/(s+f) 	...f+c=sqrt(z)
 *	and
 *		asin(x) = pi/2 - 2*(s+s*z*R(z))
 *			= pio4_hi+(pio4-2s)-(2s*z*R(z)-pio2_lo)
 *			= pio4_hi+(pio4-2f)-(2s*z*R(z)-(pio2_lo+2c))
 *
 * Special cases:
 *	if x is NaN, return x itself;
 *	if |x|>1, return NaN with invalid signal.
 *
 */



#ifdef __STDC__
static const _Decimal128
#else
static _Decimal128
#endif
  one = 1.0DL,
  huge = 1.0e+300DL,
  pio2_hi = 1.5707963267948966192313216916397514420986DL,
  pio2_lo = 4.3359050650618905123985220130216759843812E-35DL,
  pio4_hi = 7.8539816339744830961566084581987569936977E-1DL,

	/* coefficient for R(x^2) */

  /* asin(x) = x + x^3 pS(x^2) / qS(x^2)
     0 <= x <= 0.5
     peak relative error 1.9e-35  */
  pS0 = -8.358099012470680544198472400254596543711E2DL,
  pS1 =  3.674973957689619490312782828051860366493E3DL,
  pS2 = -6.730729094812979665807581609853656623219E3DL,
  pS3 =  6.643843795209060298375552684423454077633E3DL,
  pS4 = -3.817341990928606692235481812252049415993E3DL,
  pS5 =  1.284635388402653715636722822195716476156E3DL,
  pS6 = -2.410736125231549204856567737329112037867E2DL,
  pS7 =  2.219191969382402856557594215833622156220E1DL,
  pS8 = -7.249056260830627156600112195061001036533E-1DL,
  pS9 =  1.055923570937755300061509030361395604448E-3DL,

  qS0 = -5.014859407482408326519083440151745519205E3DL,
  qS1 =  2.430653047950480068881028451580393430537E4DL,
  qS2 = -4.997904737193653607449250593976069726962E4DL,
  qS3 =  5.675712336110456923807959930107347511086E4DL,
  qS4 = -3.881523118339661268482937768522572588022E4DL,
  qS5 =  1.634202194895541569749717032234510811216E4DL,
  qS6 = -4.151452662440709301601820849901296953752E3DL,
  qS7 =  5.956050864057192019085175976175695342168E2DL,
  qS8 = -4.175375777334867025769346564600396877176E1DL,
  /* 1.000000000000000000000000000000000000000E0 */

  /* asin(0.5625 + x) = asin(0.5625) + x rS(x) / sS(x)
     -0.0625 <= x <= 0.0625
     peak relative error 3.3e-35  */
  rS0 = -5.619049346208901520945464704848780243887E0DL,
  rS1 =  4.460504162777731472539175700169871920352E1DL,
  rS2 = -1.317669505315409261479577040530751477488E2DL,
  rS3 =  1.626532582423661989632442410808596009227E2DL,
  rS4 = -3.144806644195158614904369445440583873264E1DL,
  rS5 = -9.806674443470740708765165604769099559553E1DL,
  rS6 =  5.708468492052010816555762842394927806920E1DL,
  rS7 =  1.396540499232262112248553357962639431922E1DL,
  rS8 = -1.126243289311910363001762058295832610344E1DL,
  rS9 = -4.956179821329901954211277873774472383512E-1DL,
  rS10 =  3.313227657082367169241333738391762525780E-1DL,

  sS0 = -4.645814742084009935700221277307007679325E0DL,
  sS1 =  3.879074822457694323970438316317961918430E1DL,
  sS2 = -1.221986588013474694623973554726201001066E2DL,
  sS3 =  1.658821150347718105012079876756201905822E2DL,
  sS4 = -4.804379630977558197953176474426239748977E1DL,
  sS5 = -1.004296417397316948114344573811562952793E2DL,
  sS6 =  7.530281592861320234941101403870010111138E1DL,
  sS7 =  1.270735595411673647119592092304357226607E1DL,
  sS8 = -1.815144839646376500705105967064792930282E1DL,
  sS9 = -7.821597334910963922204235247786840828217E-2DL,
  /*  1.000000000000000000000000000000000000000E0 */

 asinr5625 =  5.9740641664535021430381036628424864397707E-1DL;

#include <math.h>

#define FUNCTION_NAME asin

#include <dfpmacro.h>


//#include "math_private.h"
//long double sqrtl (long double);


static DEC_TYPE
IEEE_FUNCTION_NAME (DEC_TYPE x)
{
  _Decimal128 t, w, p, q, c, r, s, ix;
  int32_t sign, flag;

  if(isnan(x))
    return x+x;
  flag = 0;
  sign = (x < 0.0DL)?1:0;

  ix = FUNC_D(fabs) (x);

  if (ix >= 1.0DL)	/* |x|>= 1 */
    {
      /* asin(1)=+-pi/2 with inexact */
      if (ix == 1.0DL)
	return (DEC_TYPE)(x * pio2_hi + x * pio2_lo);
      /* asin(|x|>1) is NaN */
      DFP_EXCEPT (FE_INVALID);
      return DFP_NAN;
    }
  else if (ix < 0.5DL) /* |x| < 0.5 */
    {
      if (ix < 0.000000000000000000000000000000000000000000000000000000002DL) /* |x| < 2**-57 */
	{
	  if (huge + x > one)
	    return x;		/* return x with inexact if x!=0 */
	  t = 0.0DL;
	}
      else
	{
	  t = x * x;
	  /* Mark to use pS, qS later on.  */
	  flag = 1;
	}
    }
  else if (ix < 0.625DL) /* 0.625 */
    {
      t = ix - 0.5625DL;
      p = ((((((((((rS10 * t
		    + rS9) * t
		   + rS8) * t
		  + rS7) * t
		 + rS6) * t
		+ rS5) * t
	       + rS4) * t
	      + rS3) * t
	     + rS2) * t
	    + rS1) * t
	   + rS0) * t;

      q = ((((((((( t
		    + sS9) * t
		  + sS8) * t
		 + sS7) * t
		+ sS6) * t
	       + sS5) * t
	      + sS4) * t
	     + sS3) * t
	    + sS2) * t
	   + sS1) * t
	+ sS0;
      t = asinr5625 + p / q;
      if (sign == 0)
	return (DEC_TYPE)t;
      else
	return (DEC_TYPE)(-t);
    }
  else
    {
      /* 1 > |x| >= 0.625 */
      w = one - ix;
      t = w * 0.5DL;
    }

  p = (((((((((pS9 * t
	       + pS8) * t
	      + pS7) * t
	     + pS6) * t
	    + pS5) * t
	   + pS4) * t
	  + pS3) * t
	 + pS2) * t
	+ pS1) * t
       + pS0) * t;

  q = (((((((( t
	      + qS8) * t
	     + qS7) * t
	    + qS6) * t
	   + qS5) * t
	  + qS4) * t
	 + qS3) * t
	+ qS2) * t
       + qS1) * t
    + qS0;

  if (flag) /* 2^-57 < |x| < 0.5 */
    {
      w = p / q;
      return (DEC_TYPE)(x + x * w);
    }

  s = __sqrtd128 (t);
  if (ix >= 0.975DL) /* |x| > 0.975 */
    {
      w = p / q;
      t = pio2_hi - (2.0DL * (s + s * w) - pio2_lo);
    }
  else
    {
      w = s;
/*    Look into the reason this code was here
      u.value = s;
      u.parts32.w3 = 0;
      u.parts32.w2 = 0;
      w = u.value;
      */
      c = (t - w * w) / (s + w);
      r = p / q;
      p = 2.0DL * s * r - (pio2_lo - 2.0DL * c);
      q = pio4_hi - 2.0DL * w;
      t = pio4_hi - (p - q);
    }

  if (sign == 0)
    return (DEC_TYPE)t;
  else
    return (DEC_TYPE)(-t);
}

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x)
{
  DEC_TYPE z = IEEE_FUNCTION_NAME (x);
#ifndef _IEEE_LIBDFP
  if(_LIB_VERSION == _IEEE_) return z;
  if (x > DFP_CONSTANT(1.0) || x < DFP_CONSTANT(-1.0))
    DFP_ERRNO (EDOM);
#endif
  return z;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
