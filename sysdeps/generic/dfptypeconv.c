/* DFP_C_TYPE to/from IEEE DFP type conversion routines definitions

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2007-2015 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Pete Eberlein <eberlein@us.ibm.com>

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

#include "dfptypeconv128.h"
#include "dfptypeconv64.h"
#include "dfptypeconv32.h"
#include <endian.h>

static inline void
__copy32 (uint32_t * src, uint32_t * dst)
{
#if __BYTE_ORDER == __BIG_ENDIAN
  *dst = *src;
#else
  *dst = __builtin_bswap32 (*src);
#endif
}

static inline void
__copy64 (uint64_t * src, uint64_t * dst)
{
#if __BYTE_ORDER == __BIG_ENDIAN
  *dst = *src;
#else
  *dst = __builtin_bswap64 (*src);
#endif
}

static inline void
__copy128 (uint64_t * src, uint64_t * dst)
{
#if __BYTE_ORDER == __BIG_ENDIAN
  *dst = *src;
  *(dst + 1) = *(src + 1);
#else
  *dst = __builtin_bswap64 (*(src + 1));
  *(dst + 1) = __builtin_bswap64 (*src);
#endif
}

void
___host_to_ieee_32 (_Decimal32 * src, decimal32 * dest)
{
  __copy32 ((uint32_t *) src, (uint32_t *) dest);
}

void
___host_to_ieee_64 (_Decimal64 * src, decimal64 * dest)
{
  __copy64 ((uint64_t *) src, (uint64_t *) dest);
}

void
___host_to_ieee_128 (_Decimal128 * src, decimal128 * dest)
{
  __copy128 ((uint64_t *) src, (uint64_t *) dest);
}

void
___ieee_32_to_host (decimal32 * src, _Decimal32 * dest)
{
  __copy32 ((uint32_t *) src, (uint32_t *) dest);
}

void
___ieee_64_to_host (decimal64 * src, _Decimal64 * dest)
{
  __copy64 ((uint64_t *) src, (uint64_t *) dest);
}

void
___ieee_128_to_host (decimal128 * src, _Decimal128 * dest)
{
  __copy128 ((uint64_t *) src, (uint64_t *) dest);
}

hidden_def (___host_to_ieee_32)
hidden_def (___host_to_ieee_64)
hidden_def (___host_to_ieee_128)
hidden_def (___ieee_32_to_host)
hidden_def (___ieee_64_to_host)
hidden_def (___ieee_128_to_host)
