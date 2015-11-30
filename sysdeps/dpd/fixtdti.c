/* DPD encoded Decimal128 to uint128 conversion routine.

   Copyright (C) 2015 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Paul E. Murphy <murphyp@linux.vnet.ibm.com>

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

#define SRC_SIZE 128
#define DSUFF DL

#ifndef SIGNED
# define SIGNED 1
# define FUNC fixtdti
# define QTYPE
#endif

static inline QTYPE __int128 convert_td_ti(_Decimal128 d);

#define D128_CONVERSION(a)  convert_td_ti(a)
#include "fixsdti.c"

#include "numdigits.h"
#include "tidpdcommon.h"

static RET_TYPE
convert_td_ti(_Decimal128 a)
{
  /* Ensure number is rounded to an integral value.  */
  _Decimal128 a_abs = __truncd128 (a);

#if SIGNED
  long neg = 0;
  if (a_abs < 0)
    {
      a_abs = -a_abs;
      neg = 1;
    }
#else
  a_abs = a;
#endif

  /* Do a quick conversion if possible.  */
  if (a_abs < 1e16DL)
    return (RET_TYPE) QUICK_CONVERT (a);

  /* The general idea:

     Extract number into three parts:
     High 16 decimal digits: a_high
     Middle 2 decimal digits: a_mid
     Low 16 decimal digits: a_low

     0 <= exp <= 38.

     return = (a_high * 1e18 + a_mid * 1e16 + a_low) * exp

     TODO:

     Note, any exp > 7 can be done in two
     halves, and likewise, exp > 23 can be
     done in one with a big multiply. That
     work waits for another day.
   */
  _Decimal128 a_high, a_low;
  long digs;
  uint64_t a_low_di, a_high_di;
  RET_TYPE a_low_ti, a_high_ti, result;
  RET_TYPE ten_16_ti = 10000000000000000LL;
  int exp;

  exp = getexpd128 (a_abs);
  digs = split_td (a_abs, &a_high, &a_low);

  /* Now we have
     a_high = top 16 decimal digits
     a_low = bottom 16 decimal digits
     digs = middle 2 digits * 16 in BCD

     Now... shift a_high right 18 digits, and convert
     as a uint64, compute digs, and convert a_low as
     a uint64
   */
  a_low_di = ((_Decimal64) setexpd128 (a_low, 0));
  a_high_di = ((_Decimal64) setexpd128 (a_high, -18));

  /* Convert into appropriate parts and minimize 128b multiplies.  */
  a_low_ti = a_low_di;
  a_high_di = a_high_di * 100LL + digs;
  a_high_ti = (RET_TYPE) a_high_di * ten_16_ti;
  result = (a_low_ti + a_high_ti) * (RET_TYPE) exp10_ti[exp];

  /* No need to worry about overflow with __int128 */
#if SIGNED
  if (neg)
    result = -result;
#endif

  return result;
}
