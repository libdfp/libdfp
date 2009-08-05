/* Returns the type of floating point number from a _Decimal32 type

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2007, 2009 Free Software Foundation, Inc.

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
#  define _DECIMAL_SIZE 32
#  include <decimal32.h>
#endif

#include <math.h>
#include <endian.h>

#define FUNCTION_NAME fpclassify

#include <float.h>
#include <dfpmacro.h>

#if BYTE_ORDER == BIG_ENDIAN
#  define END_FIX(i)	i
#else
#  define END_FIX(i)	(_DECIMAL_SIZE/8 - 1 - i)
#endif

#if _DECIMAL_SIZE == 32
#  define comb_shift		5
#  define min_normal_exp	5
#  define getExpCont(array)	((((array)[END_FIX(0)] & 0x03) << 4) |	\
				((unsigned)(array)[END_FIX(1)]>>4)	)
#elif _DECIMAL_SIZE == 64
#  define comb_shift		7
#  define min_normal_exp	14
#  define getExpCont(array)	((((array)[END_FIX(0)] & 0x03) << 6) |	\
				((unsigned)(array)[END_FIX(1)]>>2)	)
#elif _DECIMAL_SIZE == 128
#  define comb_shift		11
#  define min_normal_exp	32
#  define getExpCont(array)	((((array)[END_FIX(0)] & 0x03) << 10) |	\
				((unsigned)(array)[END_FIX(1)] << 2)  |	\
				((unsigned)(array)[END_FIX(2)] >> 6)	)
#endif

int
INTERNAL_FUNCTION_NAME (DEC_TYPE x)
{
  uint8_t top_byte;
  union
  {
    DEC_TYPE dec;
    uint8_t bytes[_DECIMAL_SIZE/8];
  } u_conv;
  uint8_t comb, msd;
  uint16_t exp;

  u_conv.dec = x;
  top_byte = u_conv.bytes[END_FIX(0)];

  if((top_byte & DECIMAL_NaN) == DECIMAL_NaN)
    return FP_NAN;
  if((top_byte & DECIMAL_Inf) == DECIMAL_Inf)
    return FP_INFINITE;

  /*  It wasn't an easy case, so extract the most significant digit
   *  and part of the exponant from the combination field*/
  comb = (top_byte >> 2) & 0x1F; /* Mask off the signbit.  */
  if (comb >= 0x18)
    {
      msd = 8 + (comb & 0x01);
      exp = (comb & 0x06) << comb_shift;
    }
  else
    {
      msd = comb & 0x07;
      exp = (comb & 0x18) << (comb_shift-2);
    }
  /* Fetch the rest of the (still biased) exp */
  exp += getExpCont(u_conv.bytes);

  /* If the Most significant digit !=0, either subnormal or normal */
  if(msd != 0)
    return (exp < min_normal_exp) ? FP_SUBNORMAL : FP_NORMAL;

  /* Clean the exponent out of the byte array */
#if _DECIMAL_SIZE == 32
  u_conv.bytes[END_FIX(1)] &= 0x0f;
#elif _DECIMAL_SIZE == 64
  u_conv.bytes[END_FIX(1)] &= 0x03;
#elif _DECIMAL_SIZE == 128
  u_conv.bytes[END_FIX(1)] = 0;
  u_conv.bytes[END_FIX(2)] &= 0x3f;
#endif

  /* Run backwards through the array, checking that each one is zero.  If we
   * don't find a non-zero then the number is FP_ZERO.  If we find non-zero then
   * we determine whether it is normal or subnormal.
   *
   * There is an issue when we receive a value like 10e-96.  Although the number
   * is representable in the normal range (i.e. 1e-95), the encoding for this
   * particular number has been given to us in this slightly screwy format.
   * Techincally -96 exceeds the minimum exponent for _Decimal32 but it is still
   * 'normal'.  We will rely on the runtime for comparison of 'x' against
   * __DEC32_MIN__.  The comparison will be correct regardless of how this
   * boundary case is encoded.
   */

  /* Hopefully none of these comparisons re-invoke fpclassifyd[32|64|128] */
  if(x == DFP_CONSTANT(0.0) || x == DFP_CONSTANT(-0.0))
    return FP_ZERO;
  else if((exp > min_normal_exp) || (x >= DFP_MIN))
    {
      /* e.g. (for the examples exp hasn't been unbiased).
       *   exp == -94
       *   x (10E-96 -> 1E-95) == DEC32_MIN (1E-95DF)
       *   x (2300E-96 -> 23E-94) > DEC32_MIN  */
      return FP_NORMAL;
    }
  else /* x < DEC_TYPE_MIN  */
    return FP_SUBNORMAL;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
