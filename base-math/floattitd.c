/* Convert a signed 128-bit binary integer into nearest representable
   IEEE754R 128-bit Densely Packed Decimal Floating-point (DFP).

   Copyright (C) 2015 Free Software Foundation, Inc.
   This file is part of the Decimal Floating Point C Library.

   Contributed by Steven Munroe (munroesj@linux.vnet.ibm.com)

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

   Please see dfp/COPYING.txt for more information.  */

#include <dfpacc.h>

#include "convert_helpers.h"

/*
 * Convert a signed 128-bit binary integer into nearest representable
 * IEEE754R 128-bit Densely Packed Decimal Floating-point (DFP).
 *
 * Care must be taken, as the signed INT128 type has a maximum
 * value of ~1.701411835e+38 (or 39 decimal digits), while the
 * _Decimal128 type carries only 34 significant digits, so inexact
 * (truncated or rounded) results are possible.
 *
 * Also the PowerISA-2.05 and later provides hardware instructions to
 * convert between signed 64-bit integer values to and from 64-/128-bit
 * DFP,  but does not provide for direct conversion of 128-bit
 * integers.  So efficiency dictates that the conversion be broken into
 * chunks, that the hardware conversion can handle, then can be
 * combined into the final decimal result.
 *
 * However conversion and scaling of arbitrary binary values to decimal
 * can create large decimal ranges that can result in inexact
 * intermediate results after conversion or rounding during the
 * combine phase.
 *
 * We can avoid this by factoring (divide and modulo) the binary value
 * into power of 10 chunks that can be converted exactly to decimal
 * and combined without rounding in the decimal form.  _Decimal128
 * has 34-digits of significance and half of that is 17-digits.
 * 17-digits fit nicely with in signed 64-bit integer for conversion).
 * So we choose 10^17 as the chunk size for factoring and conversion.
 *
 * First we check if input value is less then 2^63, which we can handle
 * directly as a long to _Decimal conversion.  Otherwise
 * the algorithm factors the 128-bit binary values into three chunks;
 * low 17-digits, middle 17-digits, and highest 5-digits.  The middle
 * and low factors can be converted and combined for an exact result
 * of up to 34-digits via digit shift and add.  Values greater than
 * 34-digits may become inexact after the final combine stage,  while
 * preserving the most significant 34-digits in the result.
 */

/* These strictly control signed/unsigned behavior.  */
#ifndef UNSIGNED
# define INPUT_TYPE64 long long
# define INPUT_TYPE INT128
#else
# define INPUT_TYPE64 unsigned long long
# define INPUT_TYPE UINT128
#endif

/* Generic macros to overload for other conversions.  */
#ifndef FUNC
# define FUNC     floattitd
# define RET_TYPE _Decimal128
# define RET_SIZE 128
#endif

#define FMULTRUNC(_x) (__fmultruncd ## RET_SIZE)(_x)

RET_TYPE
__BACKEND_ (FUNC) (INPUT_TYPE a)
{
#ifndef UNSIGNED
  INT128 zero = (INT128) 0L;
  int negative = 0;
#endif

  UINT128 ten_17 = (UINT128) 100000000000000000UL;
  UINT128 two_63 = (UINT128) 0x8000000000000000UL;
  UINT128 u, x, y, z;
  unsigned long long t_low, t_mid, t_high;
  RET_TYPE result = 0.DL;
  _Decimal128 d_low, d_mid, d_high __attribute__((unused));

  /* take the absolute value and record the sign.  */
#ifndef UNSIGNED
  if (a < zero)
    {
      /* This is necessary because __builtin_abs() does not handle
       * the INT128 and we need to record the sign bit separately.
       * Can't just use copysign later because we are dealing with
       * different register sets and copysign instructions only
       * within a specific register set.  */
      u = (UINT128) -a;
      negative = 1;
    }
  else
#endif
    {
      u = (UINT128) a;
    }


  if (u < two_63)
    {
      /* If abs(a) is less than 63-bits we can convert the signed
       * value directly.  */
    result = (INPUT_TYPE64) a;
    }
  else
    {
      /* Else convert the absolute (unsigned) value to Decimal and
       * correct the sign later if needed.  */
      y = u / ten_17;
      z = u % ten_17;
      t_low = (unsigned long long) z;
      if (y < ten_17)
	{
	  t_mid = (unsigned long long) y;
	  d_mid = t_mid;
	  d_low = t_low;
	  result = (d_mid * 100000000000000000.DL) + d_low;
	}
      else
	{
	  x = y / ten_17;
	  z = y % ten_17;
	  t_mid = (unsigned long long) z;
	  t_high = (unsigned long long) x;
	  d_low = t_low;
	  d_mid = t_mid;
	  d_high = t_high;

          /* Call out to internal helpers to properly handle
	   * conversion and trunctation into a smaller format.
	   */
#if RET_SIZE == 64
	  result = combine_and_truncd64(d_high,d_mid,d_low);
#elif RET_SIZE == 32
	  result = combine_and_truncd32(d_high,d_mid,d_low);
#else
	  result = (d_high * 100000000000000000.DL) + d_mid;
	  result = (result * 100000000000000000.DL) + d_low;
#endif
	}

#ifndef UNSIGNED
      /* Converted the absolute 127-bit value of "a" to Decimal128.
       * Now correct the decimal sign if the original value
       * was negative.  */
      if (negative)
	result = -result;
#endif
    }

  return (result);
}
hidden_def (__BACKEND_ (FUNC))
