/* Copyright (C) 2014-2015 Free Software Foundation, Inc.

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

#ifndef _BID_UINT128_H
# define _BID_UINT128_H

/* GCC/clang supports 128 types bytes for some architectures (x86_64 for
   instance).  For archs/configurations that does not support it, a
   multiprecision implementation based on the mini GMP is used.  */

#include <stdint.h>
#include <string.h>

#ifdef __SIZEOF_INT128__
typedef unsigned __int128  u128_t;
typedef unsigned __int128 *u128_ptr;

static inline u128_t
__u128_from_string (char* sz)
{
  int radix = 10;
  int size;
  int i;
  char ch;
  unsigned int n;
  u128_t a;
  int minus = 0;

  size = strlen (sz);

  a = 0;
  i = 0;

  if (sz[i] == '-')
    {
      ++i;
      minus = 1;
    }

  while (i < size)
    {
      ch = sz[i];

      if (ch >= 'A' && ch <= 'Z')
	{
	  if (((ch - 'A') + 10) < radix)
	    n = (ch - 'A') + 10;
	  else
	    break;
	}
      else if (ch >= 'a' && ch <= 'z')
        {
	  if (((ch - 'a') + 10) < radix)
	    n = (ch - 'a') + 10;
	  else
	    break;
	}
      else if (ch >= '0' && ch <= '9')
	{
	  if ((ch - '0') < radix)
	    n = (ch - '0');
	  else
	    break;
	}
      else
	/* Completely invalid character  */
	break;

      a *= radix;
      a += n;
      ++i;
    }

  if (minus)
    a = -a;

  return a;
}

#define u128_init(__x)
#define u128_init_from_u32(__ret, __hi, __mh, __ml, __lo)   \
  ({                                                        \
    __ret = ((__uint128_t)__hi << 96) |                     \
	    ((__uint128_t)__mh << 64) |                     \
	    ((__uint128_t)__ml << 32) |                     \
	     (__uint128_t)__lo;                             \
  })
#define u128_init_from_str(__x,__str) ({ __x = __u128_from_string (__str); })
#define u128_set_u32(__x, __i)        ((__x) = (__i))
#define u128_to_u32(__x, __i)         ({ (__i) = (__x); })
#define u128_hi(__x)                  (__x >> 96)
#define u128_mh(__x)                  ((__x >> 64) & 0xFFFFFFFFU)
#define u128_ml(__x)                  ((__x >> 32) & 0xFFFFFFFFU)
#define u128_lo(__x)                  ((__x) & 0xFFFFFFFFU)
#define u128_hi64(__x)                (__x >> 64)
#define u128_lo64(__x)                ((__x) & 0xFFFFFFFFFFFFFFFFULL)
#define u128_ne_u32(__x, __y)         ( __x != __y)
#define u128_add_u128(__x, __y, __z)  ({ __z = __x + __y; })
#define u128_add_u32(__x, __y, __z)   ({ __z = __x + __y; })
#define u128_mul_u32(__x, __y, __z)   ({ __z = __x * __y; })
#define u128_mod_u32(__x, __y, __z)   ({ __z = __x % __y; })
#define u128_div_u32(__x, __y, __z)   ({ __z = __x / __y; })

#else

/* Multiprecision int 128 bits based on int 32 limbs.  */
#include <libdecnumber/mini-gmp.h>

typedef mpz_t   u128_t;
typedef mpz_ptr u128_ptr;

/* Initialize a multiprecision int 128 bits number from 4 int 32 bits
   member.  Since the MPN implementation does not support bitwise operations
   we operate on the internal structure directly.  */
static inline void
u128_init_from_u32 (u128_t ret, uint32_t hi, uint32_t mh, uint32_t ml,
	           uint32_t lo)
{
  mpz_init2  (ret, 128);
  /* mpn_z::_mp_size is total number of mpz_z::_mp_d being used to represent
     the number (for instance, if mpz_z::_mp_size of 2 the number can be
     represented in a 64-bits variable.  */
  ret->_mp_size = 4;

  ret->_mp_d[3] = hi;
  if (hi == 0)
    ret->_mp_size--;
  ret->_mp_d[2] = mh;
  if (mh == 0)
    ret->_mp_size--;
  ret->_mp_d[1] = ml;
  if (ml == 0)
    ret->_mp_size--;
  ret->_mp_d[0] = lo;
  if (lo == 0)
    ret->_mp_size--;
}

static inline 
uint32_t
__u128_extract_u32 (mpz_t x, int word)
{
  return (x->_mp_size >= (word + 1)) ? x->_mp_d[word] : 0UL;
}

static inline
uint64_t
__u128_extract_64(mpz_t x, int word)
{
  uint64_t ret;
  ret   = __u128_extract_u32(x, 2*word+1);
  ret <<= 32;
  ret  |= __u128_extract_u32(x, 2*word);
  return ret;
}

#define u128_init(__x)                mpz_init2 (__x, 128)
//#define u128_init_from_str(__x,__str) mpz_init_set_str (__x, __str, 10)
#define u128_set_u32(__x, __i)        mpz_set_ui (__x, __i)
#define u128_to_u32(__x, __i)         ({ __i = mpz_get_ui (__x); })
#define u128_hi(__x)                  __u128_extract_u32 (__x, 3)
#define u128_mh(__x)                  __u128_extract_u32 (__x, 2)
#define u128_ml(__x)                  __u128_extract_u32 (__x, 1)
#define u128_lo(__x)                  __u128_extract_u32 (__x, 0)
#define u128_hi64(__x)                __u128_extract_u64 (__x, 1)
#define u128_lo64(__x)                __u128_extract_u64 (__x, 0)
#define u128_ne_u32(__x, __y)         (mpz_cmp_ui (__x, __y) != 0)
#define u128_add_u128(__x, __y, __z)  ({ mpz_add (__z, __x, __y); })
#define u128_add_u32(__x, __y, __z)   ({ mpz_add_ui (__z, __x, __y); })
#define u128_mul_u32(__x, __y, __z)   ({ mpz_mul_ui (__z, __x, __y); })
#define u128_mod_u32(__x, __y, __z)   ({ mpz_mod_ui (__z, __x, __y); })
#define u128_div_u32(__x, __y, __z)   ({ mpz_fdiv_q_ui (__z, __x, __y); })

static inline void
__u128_from_string (mpz_t a, char* sz)
{
  int radix = 10;
  int size;
  int i;
  char ch;
  unsigned int n;
  int minus = 0;

  size = strlen (sz);

  mpz_init2 (a, 128);
  mpz_set_ui (a, 0);
  i = 0;

  if (sz[i] == '-')
    {
      ++i;
      minus = 1;
    }

  while (i < size)
    {
      ch = sz[i];

      if (ch >= 'A' && ch <= 'Z')
	{
	  if (((ch - 'A') + 10) < radix)
	    n = (ch - 'A') + 10;
	  else
	    break;
	}
      else if (ch >= 'a' && ch <= 'z')
        {
	  if (((ch - 'a') + 10) < radix)
	    n = (ch - 'a') + 10;
	  else
	    break;
	}
      else if (ch >= '0' && ch <= '9')
	{
	  if ((ch - '0') < radix)
	    n = (ch - '0');
	  else
	    break;
	}
      else
	/* Completely invalid character  */
	break;

      /* a *= radix;  */
      mpz_mul_ui (a, a, radix);
      /* a += n;  */
      mpz_add_ui (a, a, n);
      ++i;
    }

  if (minus)
    mpz_neg (a, a);
}

#define u128_init_from_str(__x,__str) __u128_from_string (__x, __str)

#endif /* __SIZEOF_INT128__  */

#endif
