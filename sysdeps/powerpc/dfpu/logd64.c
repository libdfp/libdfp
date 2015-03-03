/* Calculate the Natural Log of a given a _Decimal64 value

   Copyright (C) 2010-2015 Free Software Foundation, Inc.

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

#ifndef _DECIMAL_SIZE
#  define _DECIMAL_SIZE 64
#endif

#include <math.h>
#include <ieee754r_private.h>

#define FUNCTION_NAME log

#include <dfpmacro.h>

/* __LN2digits lookup table.  */
extern _Decimal128 __LN_10;
extern _Decimal128 __LN2digits [];

/* Core implementation of logd64(x).
   Separate into exponent "e" and mantisa "x" , where log(10^e*x)
   == log(10^e) + log(x). Use frexpd64 to separate the exponent from
   the mantissa. Then separate the high order 2 digits "a" from the
   remaining digits of the mantisa. Use the high order digits for a
   table lookup (log(a) == __LN2digits[]) and then use the taylor series
   to compute:

   log(x) = log(a) + (((x -a)/a) - ((((x -a)/a)^2)/2)
                   + ((((x -a)/a)^3)/3) - ((((x -a)/a)^4)/4) + ...)

   then final compute log (10*e * x) by the sum
   log(val) = (log(10) * exp ) + log(x);

   The real logd64 will need to add checks for NAN, INF, zero, one,
   and negative values of x.
 */

static DEC_TYPE
IEEE_FUNCTION_NAME (DEC_TYPE val)
{

  _Decimal64 result, top, fraction, a, x, tp;
  _Decimal64 t1, t2, t3, t4, t5, t6, t7, t8;
  _Decimal64 t9, t10, t11, t12, t13, t14, t15, t16;
  int exp;
  long tmp;

  if (__isnand64(val))
    return val+val;
  if (val == DFP_CONSTANT(0.0))
    {
      DFP_EXCEPT (FE_DIVBYZERO);
      return -DFP_HUGE_VAL;
    }
  if (val < DFP_CONSTANT(0.0))
    {
      DFP_EXCEPT (FE_INVALID);
      return DFP_NAN;
    }
  if (__isinfd64(val))
    return val;

  result = __frexpd64 (val, &exp);

  tmp = result * 100.0DD;
  top = tmp;
  top = top * 0.01DD;
  fraction = result - top;

  if (fraction != 0.00DD)
    {
      a = top; x = result;
      t1 = (x - a) / a;
      tp = t1 * t1; /* tp == ((x-a)/a)^2 */
      t2 = tp / 2.0DD; /* t2 == (((x-a)/a)^2)/2 */
      tp = tp * t1; /* tp == ((x-a)/a)^3 */
      t3 = tp / 3.0DD; /* t3 == (((x-a)/a)^3)/3 */
      tp = tp * t1; /* tp == ((x-a)/a)^4 */
      t4 = tp / 4.0DD; /* t4 == (((x-a)/a)^4)/4 */
      tp = tp * t1; /* tp == ((x-a)/a)^5 */
      t5 = tp / 5.0DD; /* t5 == (((x-a)/a)^5)/5 */
      tp = tp * t1; /* tp == ((x-a)/a)^6 */
      t6 = tp / 6.0DD; /* t6 == (((x-a)/a)^6)/6 */
      tp = tp * t1; /* tp == ((x-a)/a)^7 */
      t7 = tp / 7.0DD; /* t7 == (((x-a)/a)^7)/7 */
      tp = tp * t1; /* tp == ((x-a)/a)^8 */
      t8 = tp / 8.0DD; /* t8 == (((x-a)/a)^8)/8 */
      if ( t8 > 1.0E-16DD)
	{
	  tp = tp * t1; /* tp == ((x-a)/a)^9 */
	  t9 = tp / 9.0DD; /* t9 == (((x-a)/a)^9)/9 */
	  tp = tp * t1; /* tp == ((x-a)/a)^10 */
	  t10 = tp / 10.0DD; /* t10 == (((x-a)/a)^10)/10 */
	  tp = tp * t1; /* tp == ((x-a)/a)^11 */
	  t11 = tp / 11.0DD; /* t12 == (((x-a)/a)^11)/11 */
	  tp = tp * t1; /* tp == ((x-a)/a)^12 */
	  t12 = tp / 12.0DD; /* t12 == (((x-a)/a)^11)/12 */
	  if  (t12 > 1.0E-16DD)
	    {
	      tp = tp * t1; /* tp == ((x-a)/a)^13 */
	      t13 = tp / 13.0DD; /* t13 == (((x-a)/a)^13)/13 */
	      tp = tp * t1; /* tp == ((x-a)/a)^14 */
	      t14 = tp / 14.0DD; /* t14 == (((x-a)/a)^14)/14 */
	      tp = tp * t1; /* tp == ((x-a)/a)^15 */
	      t15 = tp / 15.0DD; /* t15 == (((x-a)/a)^15)/15 */
	      tp = tp * t1; /* tp == ((x-a)/a)^16 */
	      t16 = tp / 16.0DD; /* t16 == (((x-a)/a)^16)/16 */
	      tp = t15 - t16;
	      tp = tp + (t13 - t14);
	      tp = tp + (t11 - t12);
	    }
	  else
	    {
	      tp = t11 - t12;
	    }
	  tp = tp + (t9 - t10);
	  tp = tp + (t7 - t8);
	}
      else
        tp = t7 - t8;

/* now sum the terms from smallest to largest to avoid loss of percision */
      tp = tp + (t5 - t6);
      tp = tp + (t3 - t4);
      tp = tp + (t1 - t2);

      if (exp!=0)
	result = (__LN_10 * exp) + __LN2digits[tmp] + tp;
      else
        result = __LN2digits[tmp] + tp;
    }
  else
    {
      if (exp!=0)
	result = (__LN_10 * exp) + __LN2digits[tmp];
      else
        result = __LN2digits[tmp];
    }
  return result;
}

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x)
{
  DEC_TYPE z = IEEE_FUNCTION_NAME (x);
  if (x == DFP_CONSTANT(0.0))
    DFP_ERRNO (ERANGE);
  if (x < DFP_CONSTANT(0.0))
    DFP_ERRNO (EDOM);
  return z;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
