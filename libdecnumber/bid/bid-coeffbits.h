/* Copyright (C) 2014 Free Software Foundation, Inc.

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

#ifndef BID_COEFFBITS_H
# define BID_COEFFBITS_H

/* ------------------------------------------------------------------- */
/* Return required number of bits to represent a binary uInt (32-bits) */
/* value using a fast log2 implementation using Bruijn sequences.      */
/* ------------------------------------------------------------------- */
static inline
Int bid_required_bits_32 (uInt value)
{
#if __GNUC__ >=3 && __GNUC_MINOR__ >= 2
  return value == 0 ? 0 : (32 - __builtin_clz (value));
#else
  /* Lookup table to count leading zeros. More info on how to construct it:
     http://supertech.csail.mit.edu/papers/debruijn.pdf  */
  static const Int tab32[] =
  {
    31, 22, 30, 21, 18, 10, 29,  2,
    20, 17, 15, 13,  9,  6, 28,  1,
    23, 19, 11,  3, 16, 14, 7,  24,
    12,  4,  8, 25,  5, 26, 27,  0
  };
  uInt hash;

  value |= value >> 1;
  value |= value >> 2;
  value |= value >> 4;
  value |= value >> 8;
  value |= value >> 16;

  hash = ((value * 0x07c4acddU) & 0xffffffffU) >> 27;
  return 32 - tab32[hash];
#endif
}

/* -------------------------------------------------------------------- */
/* Return required number of bits to represent a binary uLong (64-bits) */
/* value using a fast log2 implementation using Bruijn sequences.       */
/* -------------------------------------------------------------------- */
static inline
Int bid_required_bits_64 (uLong value)
{
#if __GNUC__ >=3 && __GNUC_MINOR__ >= 2
  return (value == 0UL ? 0 : 64 - __builtin_clzll (value));
#else
  Int ret;
  uInt hi = (value >> 32);
  if ((ret = bid_required_bits_32 (hi)) == 0)
    {
      uLong lo = (value & 0xFFFFFFFFU);
      return bid_required_bits_32 (lo);
    }
  return ret + 32;
#endif
}

# ifdef DECUSE128
/* ------------------------------------------------------------------ */
/* Return required bits to represent a binary uQuad (128-bits) value. */
/* ------------------------------------------------------------------ */
static inline
Int bid_required_bits_128 (uQuad value)
{
  /* Use 64-bits operations to keep it simple, another solution would
   * be to construct a lookup table for int128.  */
  Int ret;
  uLong hi = uQuadHi64 (value);
  if ((ret = bid_required_bits_64 (hi)) == 0)
    {
      uLong lo = uQuadLo64 (value);
      return bid_required_bits_64 (lo);
    }
  return ret + 64;
}
# endif

#endif
