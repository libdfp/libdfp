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

#ifndef _BID_DECIMAL128_H
# define _BID_DECIMAL128_H

/* GCC/clang supports 128 types bytes for some architectures (x86_64 for
   instance).  For archs/configurations that does not support it, a
   multiprecision implementation based on the mini GMP is used.  */

#ifdef HAVE_UINT128_T
/* Native int 128 bits  */
typedef __uint128_t uQuad;

#define uQuadInit(__x)
#define uQuadInitFromUInt(__ret, __hi, __mh, __ml, __lo)   \
    __ret = ((__uint128_t)__hi << 96) |                    \
	    ((__uint128_t)__mh << 64) |                    \
	    ((__uint128_t)__ml << 32) |                    \
	     (__uint128_t)__lo
#define uQuadSetUInt(__x, __i)        ((__x) = (__i))
#define uQuadToUInt(__x, __i)         (__i) = (__x)
#define uQuadHi(__x)                  (__x >> 96)
#define uQuadMh(__x)                  ((__x >> 64) & 0xFFFFFFFFU)
#define uQuadMl(__x)                  ((__x >> 32) & 0xFFFFFFFFU)
#define uQuadLo(__x)                  ((__x) & 0xFFFFFFFFU)
#define uQuadHi64(__x)                (__x >> 64)
#define uQuadLo64(__x)                ((__x) & 0xFFFFFFFFFFFFFFFFULL)
#define uQuadNEUInt(__x, __y)         ( __x != __y)
#define uQuadAddQuad(__x, __y, __z)   __z = __x + __y
#define uQuadMulUInt(__x, __y, __z)   __z = __x * __y
#define uQuadModUInt(__x, __y, __z)   __z = __x % __y
#define uQuadDivUInt(__x, __y, __z)   __z = __x / __y

#else
/* Multiprecision int 128 bits based on int 32 limbs.  */
#include <mini-gmp.c>

typedef mpz_t uQuad;

/* Initialize a multiprecision int 128 bits number from 4 int 32 bits
   member.  Since the MPN implementation does not support bitwise operations
   we operate on the internal structure directly.  */
static inline void
uQuadInitFromUInt (uQuad ret, uInt hi, uInt mh, uInt ml, uInt lo)
{
  mpz_init2  (ret, 128);
  /* mpn_z::_mp_size is total number of mpz_z::_mp_d being used to represent
     the number (for instance, if mpz_z::_mp_size of 2 the number can be
     represented in a 64-bits variable.  */
  ret->_mp_size = 0;

  ret->_mp_d[3] = hi;
  ret->_mp_d[2] = mh;
  ret->_mp_d[1] = ml;
  ret->_mp_d[0] = lo;
  
  if (hi != 0)
    ret->_mp_size = 4;
  else if (mh != 0)
    ret->_mp_size = 3;
  else if (ml != 0)
    ret->_mp_size = 2;
  else if (lo != 0)
    ret->_mp_size = 1;
}

static inline
unsigned long int
__dec_extract_word (mpz_t x, int word)
{
  return (x->_mp_size >= (word + 1)) ? x->_mp_d[word] : 0UL;
}

static inline
unsigned long long int
__dec_extract_doubleword (mpz_t x, int word)
{
  unsigned long long int ret;
  ret   = __dec_extract_word(x, 2*word+1);
  ret <<= 32;
  ret  |= __dec_extract_word(x, 2*word);
  return ret;
}

#define uQuadInit(__x)                mpz_init2 (__x, 128)
#define uQuadSetUInt(__x, __i)        mpz_set_ui (__x, __i)
#define uQuadToUInt(__x, __i)         __i = mpz_get_ui (__x)
#define uQuadHi(__x)                  __dec_extract_word (__x, 3)
#define uQuadMh(__x)                  __dec_extract_word (__x, 2)
#define uQuadMl(__x)                  __dec_extract_word (__x, 1)
#define uQuadLo(__x)                  __dec_extract_word (__x, 0)
#define uQuadHi64(__x)                __dec_extract_doubleword (__x, 1)
#define uQuadLo64(__x)                __dec_extract_doubleword (__x, 0)
#define uQuadNEUInt(__x, __y)         (mpz_cmp_ui (__x, __y) != 0)
#define uQuadAddQuad(__x, __y, __z)   mpz_add (__z, __x, __y)
#define uQuadMulUInt(__x, __y, __z)   mpz_mul_ui (__z, __x, __y)
#define uQuadModUInt(__x, __y, __z)   mpz_mod_ui (__z, __x, __y)
#define uQuadDivUInt(__x, __y, __z)   mpz_fdiv_q_ui (__z, __x, __y)
#endif

#endif
