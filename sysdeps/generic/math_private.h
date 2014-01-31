/* Helper macros and functions to manipulate IEEE 754 bitwise.

   Copyright (C) 2014 Free Software Foundation, Inc.

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

   Please see dfp/COPYING.txt for more information.  */

#ifndef _MATH_PRIVATE_H
# define _MATH_PRIVATE_H

#include <endian.h>
#include <stdint.h>

typedef union
{
  _Decimal128 value;
  struct
  {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    uint32_t w4;
    uint32_t w3;
    uint32_t w2;
    uint32_t w1;
#elif __BYTE_ORDER == __BIG_ENDIAN
    uint32_t w1;
    uint32_t w2;
    uint32_t w3;
    uint32_t w4;
#endif
  } parts32;
  struct
  {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    uint64_t dw2;
    uint64_t dw1;
#elif __BYTE_ORDER == __BIG_ENDIAN
    uint64_t dw1;
    uint64_t dw2;
#endif
  } parts64;
} ieee754r_dec128_shape_type;

/* Get more significant 64 bit int from a _Decimal128  */
#define GET_DEC128_HIGH_WORD64(i,d)				\
do {								\
  ieee754r_dec128_shape_type gf_u;				\
  gf_u.value = (d);						\
  (i) = gf_u.parts64.dw1;					\
} while (0)

#define DEC128_SNAN_MASK64	UINT64_C(0x7E00000000000000)


typedef union
{
  _Decimal64 value;
  struct
  {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    uint32_t lsw;
    uint32_t msw;
#elif __BYTE_ORDER == __BIG_ENDIAN
    uint32_t msw;
    uint32_t lsw;
#endif
  } parts;
  uint64_t word;
} ieee754r_dec64_shape_type;

/* Get a 64 bit int from a _Decimal64  */
#define GET_DEC64_WORD64(i,d)					\
do {								\
  ieee754r_dec64_shape_type gf_u;				\
  gf_u.value = (d);						\
  (i) = gf_u.word;						\
} while (0)

#define DEC64_SNAN_MASK64	UINT64_C(0x7E00000000000000)


typedef union
{
  _Decimal32 value;
  uint32_t word;
} ieee754r_dec32_shape_type;

/* Get a 32 bit int from a _Decimal32  */
#define GET_DEC32_WORD(i,d)					\
do {								\
  ieee754r_dec32_shape_type gf_u;				\
  gf_u.value = (d);						\
  (i) = gf_u.word;						\
} while (0)

#define DEC32_SNAN_MASK		0x7E000000

#endif
