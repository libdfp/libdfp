/* DFP_C_TYPE to/from IEEE DFP type conversion routines definitions

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2007, 2009 Free Software Foundation.

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
#include <string.h>
#include <endian.h>

typedef struct {
  char elem[4];
} bytes32;

#define SWAPBYTES(s,d) \
    (d)->elem[0] = (s)->elem[3]; \
    (d)->elem[1] = (s)->elem[2]; \
    (d)->elem[2] = (s)->elem[1]; \
    (d)->elem[3] = (s)->elem[0];


void ___host_to_ieee_32 (_Decimal32 *src, decimal32 *dest) {
#if BYTE_ORDER == BIG_ENDIAN
  memcpy(dest, src, 4);
#else
  bytes32 *s, *d;
  s = (bytes32*) src;
  d = (bytes32*) dest;
  SWAPBYTES(s,d);
#endif
}

void ___host_to_ieee_64 (_Decimal64 *src, decimal64 *dest) {
#if BYTE_ORDER == BIG_ENDIAN
  memcpy(dest, src, 8);
#else
  bytes32 *s, *d;
  s = (bytes32*) src;
  d = (bytes32*) dest;
  ++d;
  SWAPBYTES(s,d);
  --d; ++s;
  SWAPBYTES(s,d);
#endif
}

void ___host_to_ieee_128 (_Decimal128 *src, decimal128 *dest) {
#if BYTE_ORDER == BIG_ENDIAN
  memcpy(dest, src, 16);
#else
  bytes32 *s, *d;
  s = (bytes32*) src;
  d = (bytes32*) dest;
  d += 3;
  SWAPBYTES(s,d);
  --d; ++s;
  SWAPBYTES(s,d);
  --d; ++s;
  SWAPBYTES(s,d);
  --d; ++s;
  SWAPBYTES(s,d);
#endif
}

void ___ieee_32_to_host (decimal32 *src, _Decimal32 *dest) {
#if BYTE_ORDER == BIG_ENDIAN
  memcpy(dest, src, 4);
#else
  bytes32 *s, *d;
  s = (bytes32*) src;
  d = (bytes32*) dest;
  SWAPBYTES(s,d);
#endif
}

void ___ieee_64_to_host (decimal64 *src, _Decimal64 *dest) {
#if BYTE_ORDER == BIG_ENDIAN
  memcpy(dest, src, 8);
#else
  bytes32 *s, *d;
  s = (bytes32*) src;
  d = (bytes32*) dest;
  ++d;
  SWAPBYTES(s,d);
  --d; ++s;
  SWAPBYTES(s,d);
#endif
}

void ___ieee_128_to_host (decimal128 *src, _Decimal128 *dest) {
#if BYTE_ORDER == BIG_ENDIAN
  memcpy(dest, src, 16);
#else
  bytes32 *s, *d;
  s = (bytes32*) src;
  d = (bytes32*) dest;
  d += 3;
  SWAPBYTES(s,d);
  --d; ++s;
  SWAPBYTES(s,d);
  --d; ++s;
  SWAPBYTES(s,d);
  --d; ++s;
  SWAPBYTES(s,d);
#endif
}

hidden_def(___host_to_ieee_32)
hidden_def(___host_to_ieee_64)
hidden_def(___host_to_ieee_128)
hidden_def(___ieee_32_to_host)
hidden_def(___ieee_64_to_host)
hidden_def(___ieee_128_to_host)
