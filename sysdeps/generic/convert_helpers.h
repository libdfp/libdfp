/* Generic implementations of conversion helper routines.

   Copyright (C) 2016 Free Software Foundation, Inc.

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

   Please see libdfp/COPYING.txt for more information.  */

#ifndef _CONVERTHELPERS
#define _CONVERTHELPERS

#include <math.h>
#include <dfpacc.h>

_Decimal64
combine_and_truncd64 (_Decimal128 hi, _Decimal128 mid, _Decimal128 low);

_Decimal32
combine_and_truncd32 (_Decimal128 hi, _Decimal128 mid, _Decimal128 low);

/* This is a simplified version of frexpd* which instead
   returns frexpd*(x) * 10^num_digs.  mp is assumed to be
   10^num_digs.  */
#define GETMANTANDEXP(SIZE)				  \
static inline _Decimal ## SIZE				  \
getmantandexpd ## SIZE (_Decimal ## SIZE x,		  \
			int *y,				  \
			int shift __attribute((unused)),  \
			_Decimal ## SIZE mp)		  \
{							  \
  _Decimal ## SIZE result = __frexpd ## SIZE (x,y);	  \
  return result * mp;					  \
}

GETMANTANDEXP(32)
GETMANTANDEXP(64)
GETMANTANDEXP(128)

#define fast_truncd128(a) __truncd128(a)
#define fast_truncd64(a)  __truncd64(a)
#define fast_truncd32(a)  __truncd32(a)

#ifdef __DECIMAL_BID_FORMAT__
#include "bid-private.h"
#include <stdint.h>

static inline int
getmantd32 (_Decimal32 sd, long *exp)
{
  union { _Decimal32 d; uint32_t i; } n = { .d = sd };
  int neg = n.i & (1<<31);
  int m;

  if ((n.i & 0x60000000) != 0x60000000)
    {
      *exp = (n.i >> 23) & 0xFF;
      m = n.i & 0x7FFFFF;
    }
  else
    {
      *exp = (n.i >> 21) & 0xFF;
      m = (n.i & 0x1FFFFF) | (1 << 23);
    }
  *exp -= DECIMAL32_Bias;
  return neg ? -m : m;
}

static inline long long int
getmantd64 (_Decimal64 dd, long *exp)
{
  union { _Decimal64 d; int64_t i; } n = { .d = dd };
  uint64_t neg = n.i & (1LL<<63LL);
  int64_t m;

  if (((n.i >> 61LL) & 0x3LL) != 0x3LL)
    {
      *exp = (n.i >> 53) & 0x3FF;
      m = n.i & 0x1FFFFFFFFFFFFFLL;
    }
  else
    {
      *exp = (n.i >> 51) & 0x3FF;
      m = n.i & 0x7FFFFFFFFFFFFLL;
      m |= (1LL << 53LL);
    }
  *exp -= DECIMAL64_Bias;
  return neg ? -m : m;
}

#ifdef INT128
static inline INT128
getmantd128 (_Decimal128 td, long *exp)
{
  union { _Decimal128 d;
          struct {
	    #if __BYTE_ORDER == __LITTLE_ENDIAN
	      UINT128 c:113;
	      UINT128 e:14;
	      UINT128 s:1;
	    #else
	      UINT128 s:1;
	      UINT128 e:14;
	      UINT128 c:113;
	    #endif
	  } bc;
	  uint64_t di[2];
  } n = { .d = td };
  INT128 m;

  *exp = n.bc.e;
  *exp -= DECIMAL128_Bias;
  m = n.bc.c;
  return n.bc.s ? -m : m;
}
#endif /* INT128 */

#else /* DPD backend */

#include "dpd-private.h"
static inline int
getmantd32 (_Decimal32 sd, long *exp)
{
  union ieee754r_Decimal32 sdsi;
  struct ieee754r_c_field cf;
  int m;
  sdsi.sd = sd;
  cf = c_decoder[sdsi.ieee.c];
  *exp = (cf.lm_exp << DECIMAL32_BEC_bits) | sdsi.ieee.bec;
  *exp -= DECIMAL32_Bias;
  m =  dpd_to_bin[sdsi.ieee.cc1];
  m += dpd_to_bin[sdsi.ieee.cc0] * 1000;
  m += cf.lmd * 1000000;
  if (sdsi.ieee.negative)
    m = -m;
  return m;
}

/* Merge two fields spanning a type. */
#define _M(_l,_h,_n,_p) \
  (((_p).ieee.cc ## _n ## L ## _l) | (((_p).ieee.cc ## _n ## H ## _h)<<_l))

static inline long long int
getmantd64 (_Decimal64 dd, long *exp)
{
  union ieee754r_Decimal64 dddi;
  struct ieee754r_c_field cf;
  long long int m;
  long cc1;
  dddi.dd = dd;
  cf = c_decoder[dddi.ieee.c];
  *exp = (cf.lm_exp << DECIMAL64_BEC_bits) | dddi.ieee.bec;
  *exp -= DECIMAL64_Bias;
  cc1 = _M(2,8,1,dddi);
  m =  dpd_to_bin[dddi.ieee.cc4];
  m += (long long int) dpd_to_bin[dddi.ieee.cc3] * 1000;
  m += (long long int) dpd_to_bin[dddi.ieee.cc2] * 1000000;
  m += (long long int) dpd_to_bin[          cc1] * 1000000000;
  m += (long long int) dpd_to_bin[dddi.ieee.cc0] * 1000000000000;
  m += (long long int)                    cf.lmd * 1000000000000000;
  if (dddi.ieee.negative)
    m = -m;
  return m;
}

#ifdef INT128
static inline INT128
getmantd128 (_Decimal128 td, long *exp)
{
  union ieee754r_Decimal128 tdti;
  struct ieee754r_c_field cf;
  INT128 m;
  long cc1, cc4, cc7;
  tdti.td = td;
  cf = c_decoder[tdti.ieee.c];
  *exp = (cf.lm_exp << DECIMAL128_BEC_bits) | tdti.ieee.bec;
  *exp -= DECIMAL128_Bias;
  cc1 = _M(6,4,1,tdti);
  cc4 = _M(4,6,4,tdti);
  cc7 = _M(2,8,7,tdti);
  m = cf.lmd;
  m = 1000 * m + dpd_to_bin[tdti.ieee.cc0];
  m = 1000 * m + dpd_to_bin[          cc1];
  m = 1000 * m + dpd_to_bin[tdti.ieee.cc2];
  m = 1000 * m + dpd_to_bin[tdti.ieee.cc3];
  m = 1000 * m + dpd_to_bin[          cc4];
  m = 1000 * m + dpd_to_bin[tdti.ieee.cc5];
  m = 1000 * m + dpd_to_bin[tdti.ieee.cc6];
  m = 1000 * m + dpd_to_bin[          cc7];
  m = 1000 * m + dpd_to_bin[tdti.ieee.cc8];
  m = 1000 * m + dpd_to_bin[tdti.ieee.cc9];
  m = 1000 * m + dpd_to_bin[tdti.ieee.cc10];
  if (tdti.ieee.negative)
    m = -m;
  return m;
}
#endif
#endif /* _BID_BACKEND == 1 */

#endif /* _CONVERTHELPERS */
