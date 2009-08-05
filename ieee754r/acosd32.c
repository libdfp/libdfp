/* Calculates the arccos(x)

   Copyright (C) 2001 Stephen L. Moshier <moshier@na-net.ornl.gov>
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
 * __ieee754_acosl(x)
 * Method :
 *      acos(x)  = pi/2 - asin(x)
 *      acos(-x) = pi/2 + asin(x)
 * For |x| <= 0.375
 *      acos(x) = pi/2 - asin(x)
 * Between .375 and .5 the approximation is
 *      acos(0.4375 + x) = acos(0.4375) + x P(x) / Q(x)
 * Between .5 and .625 the approximation is
 *      acos(0.5625 + x) = acos(0.5625) + x rS(x) / sS(x)
 * For x > 0.625,
 *      acos(x) = 2 asin(sqrt((1-x)/2))
 *      computed with an extended precision square root in the leading term.
 * For x < -0.625
 *      acos(x) = pi - 2 asin(sqrt((1-|x|)/2))
 *
 * Special cases:
 *      if x is NaN, return x itself;
 *      if |x|>1, return NaN with invalid signal.
 *
 * Functions needed: __ieee754_sqrtl.
 */


#ifdef __STDC__
static const _Decimal128
#else
static _Decimal128
#endif
  one = 1.0DL,
  pio2_hi = 1.5707963267948966192313216916397514420986DL,
  pio2_lo = 4.3359050650618905123985220130216759843812E-35DL,

  /* acos(0.5625 + x) = acos(0.5625) + x rS(x) / sS(x)
     -0.0625 <= x <= 0.0625
     peak relative error 3.3e-35  */

  rS0 =  5.619049346208901520945464704848780243887E0DL,
  rS1 = -4.460504162777731472539175700169871920352E1DL,
  rS2 =  1.317669505315409261479577040530751477488E2DL,
  rS3 = -1.626532582423661989632442410808596009227E2DL,
  rS4 =  3.144806644195158614904369445440583873264E1DL,
  rS5 =  9.806674443470740708765165604769099559553E1DL,
  rS6 = -5.708468492052010816555762842394927806920E1DL,
  rS7 = -1.396540499232262112248553357962639431922E1DL,
  rS8 =  1.126243289311910363001762058295832610344E1DL,
  rS9 =  4.956179821329901954211277873774472383512E-1DL,
  rS10 = -3.313227657082367169241333738391762525780E-1DL,

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
  /* 1.000000000000000000000000000000000000000E0 */

  acosr5625 = 9.7338991014954640492751132535550279812151E-1DL,
  pimacosr5625 = 2.1682027434402468335351320579240000860757E0DL,

  /* acos(0.4375 + x) = acos(0.4375) + x rS(x) / sS(x)
     -0.0625 <= x <= 0.0625
     peak relative error 2.1e-35  */

  P0 =  2.177690192235413635229046633751390484892E0DL,
  P1 = -2.848698225706605746657192566166142909573E1DL,
  P2 =  1.040076477655245590871244795403659880304E2DL,
  P3 = -1.400087608918906358323551402881238180553E2DL,
  P4 =  2.221047917671449176051896400503615543757E1DL,
  P5 =  9.643714856395587663736110523917499638702E1DL,
  P6 = -5.158406639829833829027457284942389079196E1DL,
  P7 = -1.578651828337585944715290382181219741813E1DL,
  P8 =  1.093632715903802870546857764647931045906E1DL,
  P9 =  5.448925479898460003048760932274085300103E-1DL,
  P10 = -3.315886001095605268470690485170092986337E-1DL,
  Q0 = -1.958219113487162405143608843774587557016E0DL,
  Q1 =  2.614577866876185080678907676023269360520E1DL,
  Q2 = -9.990858606464150981009763389881793660938E1DL,
  Q3 =  1.443958741356995763628660823395334281596E2DL,
  Q4 = -3.206441012484232867657763518369723873129E1DL,
  Q5 = -1.048560885341833443564920145642588991492E2DL,
  Q6 =  6.745883931909770880159915641984874746358E1DL,
  Q7 =  1.806809656342804436118449982647641392951E1DL,
  Q8 = -1.770150690652438294290020775359580915464E1DL,
  Q9 = -5.659156469628629327045433069052560211164E-1DL,
  /* 1.000000000000000000000000000000000000000E0 */

  acosr4375 = 1.1179797320499710475919903296900511518755E0DL,
  pimacosr4375 = 2.0236129215398221908706530535894517323217E0DL,

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
  qS8 = -4.175375777334867025769346564600396877176E1DL;
  /* 1.000000000000000000000000000000000000000E0 */

#include <math.h>

#define FUNCTION_NAME acos

#include <dfpmacro.h>

static DEC_TYPE
IEEE_FUNCTION_NAME (DEC_TYPE x)
{
  _Decimal128 z, r, w, p, q, s, t, f2, ix;
  int32_t sign;

  if(isnan(x))
    return x+x;

  sign = (x > 0.0DL)?0:1;
  ix = FUNC_D(fabs) (x);

  if (ix >= 1.0DL)		/* |x| >= 1 */
    {
      if (ix == 1.0DL)
	{			/* |x| == 1 */
	  if (sign == 0)
	    return (DEC_TYPE)(0.0DL);		/* acos(1) = 0  */
	  else
	    return (DEC_TYPE)((2.0DL * pio2_hi) + (2.0DL * pio2_lo));	/* acos(-1)= pi */
	}
      /* acos(|x| > 1) is NaN */
      DFP_EXCEPT (FE_INVALID);
      return DFP_NAN;
    }
  else if (ix < 0.5DL)	/* |x| < 0.5 */
    {
      /* |x| < 2**-57 */
      if (ix < 0.000000000000000000000000000000000000000000000000000000002DL)
	return (DEC_TYPE)(pio2_hi + pio2_lo); //Should raise INEXACT
      if (ix < 0.4375DL)	/* |x| < .4375 */
	{
	  /* Arcsine of x.  */
	  z = x * x;
	  p = (((((((((pS9 * z
		       + pS8) * z
		      + pS7) * z
		     + pS6) * z
		    + pS5) * z
		   + pS4) * z
		  + pS3) * z
		 + pS2) * z
		+ pS1) * z
	       + pS0) * z;
	  q = (((((((( z
		       + qS8) * z
		     + qS7) * z
		    + qS6) * z
		   + qS5) * z
		  + qS4) * z
		 + qS3) * z
		+ qS2) * z
	       + qS1) * z
	    + qS0;
	  r = x + x * p / q;
	  z = pio2_hi - (r - pio2_lo);
	  return (DEC_TYPE)z;
	}
      /* .4375 <= |x| < .5 */
      t = ix - 0.4375DL;
      p = ((((((((((P10 * t
		    + P9) * t
		   + P8) * t
		  + P7) * t
		 + P6) * t
		+ P5) * t
	       + P4) * t
	      + P3) * t
	     + P2) * t
	    + P1) * t
	   + P0) * t;

      q = (((((((((t
		   + Q9) * t
		  + Q8) * t
		 + Q7) * t
		+ Q6) * t
	       + Q5) * t
	      + Q4) * t
	     + Q3) * t
	    + Q2) * t
	   + Q1) * t
	+ Q0;
      r = p / q;
      if (sign)
	r = pimacosr4375 - r;
      else
	r = acosr4375 + r;
      return (DEC_TYPE)r;
    }
  else if (ix < 0.625DL)	/* |x| < 0.625 */
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

      q = (((((((((t
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
      if (sign)
	r = pimacosr5625 - p / q;
      else
	r = acosr5625 + p / q;
      return (DEC_TYPE)r;
    }
  else
    {				/* |x| >= .625 */
      z = (one - ix) * 0.5DL;
      s = sqrtd128 (z);
      /* Compute an extended precision square root from
	 the Newton iteration  s -> 0.5 * (s + z / s).
         The change w from s to the improved value is
	    w = 0.5 * (s + z / s) - s  = (s^2 + z)/2s - s = (z - s^2)/2s.
          Express s = f1 + f2 where f1 * f1 is exactly representable.
	  w = (z - s^2)/2s = (z - f1^2 - 2 f1 f2 - f2^2)/2s .
          s + w has extended precision.  */
      p = s;
     /*
      u.value = s;
      u.parts32.w2 = 0;
      u.parts32.w3 = 0;
      */
      f2 = s - p;
      w = z - p * p;
      w = w - 2.0DL * p * f2;
      w = w - f2 * f2;
      w = w / (2.0DL * s);
      /* Arcsine of s.  */
      p = (((((((((pS9 * z
		   + pS8) * z
		  + pS7) * z
		 + pS6) * z
		+ pS5) * z
	       + pS4) * z
	      + pS3) * z
	     + pS2) * z
	    + pS1) * z
	   + pS0) * z;
      q = (((((((( z
		   + qS8) * z
		 + qS7) * z
		+ qS6) * z
	       + qS5) * z
	      + qS4) * z
	     + qS3) * z
	    + qS2) * z
	   + qS1) * z
	+ qS0;
      r = s + (w + s * p / q);

      if (sign)
	w = pio2_hi + (pio2_lo - r);
      else
	w = r;
      return (DEC_TYPE)(2.0DL * w);
    }
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
