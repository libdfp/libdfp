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
  /* This algorithm can be optimized by using compiler builtins:
      x == 0 ? 0 : 32 - __builtin_clzl (x)  */

  /* Lookup table, more info on how to construct it:
     http://supertech.csail.mit.edu/papers/debruijn.pdf  */
  static const int tab32[] =
  {
     0,  9,  1, 10, 13, 21,  2, 29,
    11, 14, 16, 18, 22, 25,  3, 30,
     8, 12, 20, 28, 15, 17, 24,  7,
    19, 27, 23,  6, 26,  5,  4, 31
  };
  uInt hash;

  value |= value >> 1;
  value |= value >> 2;
  value |= value >> 4;
  value |= value >> 8;
  value |= value >> 16;

  hash = ((uInt)((value - (value >> 1))*0x077CB531U)) >> 27;
  return tab32[hash] + 1;
}

/* -------------------------------------------------------------------- */
/* Return required number of bits to represent a binary uLong (64-bits) */
/* value using a fast log2 implementation using Bruijn sequences.       */
/* -------------------------------------------------------------------- */
static inline
Int bid_required_bits_64 (uLong value)
{
  /* This algorithm can be optimized by using compiler builtins:
      x == 0 ? 0 : 64 - __builtin_clzll (x)  */

  /* Lookup table, more info on how to construct it:
     http://supertech.csail.mit.edu/papers/debruijn.pdf  */
  static const int tab64[] =
  {
    63,  0, 58,  1, 59, 47, 53,  2,
    60, 39, 48, 27, 54, 33, 42,  3,
    61, 51, 37, 40, 49, 18, 28, 20,
    55, 30, 34, 11, 43, 14, 22,  4,
    62, 57, 46, 52, 38, 26, 32, 41,
    50, 36, 17, 19, 29, 10, 13, 21,
    56, 45, 25, 31, 35, 16,  9, 12,
    44, 24, 15,  8, 23,  7,  6,  5
  };
  uLong hash;

  value |= value >> 1;
  value |= value >> 2;
  value |= value >> 4;
  value |= value >> 8;
  value |= value >> 16;
  value |= value >> 32;

  hash = ((uLong)((value - (value >> 1))*0x07EDD5E59A4E28C2UL)) >> 58;
  return tab64[hash] + 1;
}

# ifdef DECUSE128
/* ------------------------------------------------------------------ */
/* Return required bits to represent a binary uQuad (128-bits) value. */
/* ------------------------------------------------------------------ */
static inline
Int bid_required_bits_128 (Quad value)
{
  /* Use 64-bits operations to keep it simple, another solution would
   * be to construct a lookup table for int128.  */
  Int ret;
  uLong hi = (value >> 64);
  if ((ret = bid_required_bits_64 (hi)) == 0)
    {
      uLong lo = (value & 0xFFFFFFFFFFFFFFFFULL);
      return bid_required_bits_64 (lo);
    }
  return ret + 64;
}
# endif

#endif
