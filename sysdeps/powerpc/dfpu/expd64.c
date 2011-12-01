/* Decimal Floating Point 64-bit e^x.  POWER6 PowerPC64 version.

   Copyright (C) 2008, 2010 Free Software Foundation, Inc.

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

#ifndef _DECIMAL_SIZE
#  define _DECIMAL_SIZE 64
#  include <decimal64.h>
#endif

#define FUNCTION_NAME exp

#include <dfpmacro.h>

#include "ddlogtbls.h"

/* Core implementation of expd64(x).
   Separate into integer and fractional parts, where e^(i+f) == (e^i)*(e^f).
   We use a table lookup (expIntXDL(i)) for e^i and taylor series for e^f.
   Effectively:

   e^x = e^i * (1 + (x-i) + (((x-i)^2)/2!) + (((x-i)^3)/3!) + ...)

   The real expd64 will need add checks for NAN, INF and handle negative
   values of x.
 */
static DEC_TYPE
IEEE_FUNCTION_NAME (DEC_TYPE val)
{
  DEC_TYPE result, top, fraction, a, x, tp;
  DEC_TYPE t1, t2, t3, t4, t5, t6, t7, t8;
  DEC_TYPE t9, t10, t11, t12, t13, t14, t15, t16;
  DEC_TYPE t17, t18, t19, t20;
  long exp;
  long tmp;

  int neg = 0;

  if (__isinfd64(val))
    {
      if (val < DFP_CONSTANT(0.0))
	return DFP_CONSTANT(0.0); /* exp(-inf) = 0  */
      else
	return val;               /* exp(inf) = inf  */
    }

  if (val < DFP_CONSTANT(0.0))
    {
      neg = 1;
      val = FUNC_D(fabs) (val);
    }

  tmp = val;
  top = tmp;
  fraction = val - top;
  exp = tmp;
  if (fraction != 0.0DD)
    {
      a = top; x = val;
      t1 = (x - a);
      tp = t1 * t1; /* tp == ((x-a))^2 */
      t2 = tp * __oneOverfactDD[2]; /* t2 == (((x-a))^2)/2! */
      tp = tp * t1; /* tp == ((x-a))^3 */
      t3 = tp * __oneOverfactDD[3]; /* t3 == (((x-a))^3)/3! */
      tp = tp * t1; /* tp == ((x-a)/a)^4 */
      t4 = tp * __oneOverfactDD[4]; /* t4 == (((x-a))^4)/4! */
      tp = tp * t1; /* tp == ((x-a))^5 */
      t5 = tp * __oneOverfactDD[5]; /* t5 == (((x-a))^5)/5! */
      tp = tp * t1; /* tp == ((x-a))^6 */
      t6 = tp * __oneOverfactDD[6]; /* t6 == (((x-a))^6)/6! */
      tp = tp * t1; /* tp == ((x-a))^7 */
      t7 = tp * __oneOverfactDD[7]; /* t7 == (((x-a))^7)/7! */
      tp = tp * t1; /* tp == ((x-a))^8 */
      t8 = tp * __oneOverfactDD[8]; /* t8 == (((x-a))^8)/8! */
      if ( t8 > 1.0E-16DD)
	{
	  tp = tp * t1; /* tp == ((x-a))^9 */
	  t9 = tp * __oneOverfactDD[9]; /* t9 == (((x-a))^9)/9! */
	  tp = tp * t1; /* tp == ((x-a))^10 */
	  t10 = tp * __oneOverfactDD[10]; /* t10 == (((x-a))^10)/10! */
	  tp = tp * t1; /* tp == ((x-a))^11 */
	  t11 = tp * __oneOverfactDD[11]; /* t12 == (((x-a))^11)/11! */
	  tp = tp * t1; /* tp == ((x-a))^12 */
	  t12 = tp * __oneOverfactDD[12]; /* t12 == (((x-a))^11)/12! */
	  if  (t12 > 1.0E-16DD)
	    {
	      tp = tp * t1; /* tp == ((x-a))^13 */
	      t13 = tp * __oneOverfactDD[13]; /* t13 == (((x-a))^13)/13! */
	      tp = tp * t1; /* tp == ((x-a))^14 */
	      t14 = tp * __oneOverfactDD[14]; /* t14 == (((x-a))^14)/14! */
	      tp = tp * t1; /* tp == ((x-a))^15 */
	      t15 = tp * __oneOverfactDD[15]; /* t15 == (((x-a))^15)/15! */
	      tp = tp * t1; /* tp == ((x-a))^16 */
	      t16 = tp * __oneOverfactDD[16]; /* t16 == (((x-a))^16)/16! */
	      if  (t12 > 1.0E-16DD)
		{
		  tp = tp * t1; /* tp == ((x-a))^17 */
		  t17 = tp * __oneOverfactDD[17]; /* t17 == (((x-a))^17)/17! */
		  tp = tp * t1; /* tp == ((x-a))^18 */
		  t18 = tp * __oneOverfactDD[18]; /* t18 == (((x-a))^18)/18! */
		  tp = tp * t1; /* tp == ((x-a))^19 */
		  t19 = tp * __oneOverfactDD[19]; /* t19 == (((x-a))^19)/19! */
		  tp = tp * t1; /* tp == ((x-a))^20 */
		  t20 = tp * __oneOverfactDD[20]; /* t20 == (((x-a))^20)/20! */
		  tp = t19 + t20;
		  tp = tp + t18;
		  tp = tp + t17;
		  tp = tp + t16;
		  tp = tp + t15;
		}
	      else
		{
		  tp = t15 + t16;
		}
	      tp = tp + t14;
	      tp = tp + t13;
	      tp = tp + t12;
	      tp = tp + t11;
	    }
	  else
	    {
	      tp = t11 + t12;
	    }
	  tp = tp + t10;
	  tp = tp + t9;
	  tp = tp + t8;
	  tp = tp + t7;
	}
      else
        tp = t7 + t8;

/* now sum the terms from smallest to largest to avoid lose of percision */
      tp = tp + t6;
      tp = tp + t5;
      tp = tp + t4;
      tp = tp + t3;
      tp = tp + t2;
      tp = tp + t1;
      tp = tp + 1.DD;

      if (exp!=0)
	result = __expIntXDL[exp] * tp;
      else
        result = tp;
    }
  else
    {
      if (exp!=0)
	result = __expIntXDL[exp];
      else
        result = 1.DD;
    }

  if (neg)
    result = 1/result;

  return result;
}

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x)
{
  DEC_TYPE z = IEEE_FUNCTION_NAME (x);
#ifndef _IEEE_LIBDFP
  if(_LIB_VERSION == _IEEE_) return z;
  if (!FUNC_D(__isfinite) (z) && FUNC_D(__isfinite) (x))
    DFP_ERRNO (ERANGE);
#endif
  return z;
}
weak_alias(INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
