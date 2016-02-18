/* s390 overrides for the generic helpers.

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

#ifndef _CONVERTHELPERS_S390
#define _CONVERTHELPERS_S390

#include "dfpfenv_private.h"

#include <stdio.h>

#define COMBINE_AND_TRUNC(a,b,c,SIZE)                                         \
static inline _Decimal ## SIZE                                                \
combine_and_truncd ## SIZE (_Decimal128 hi, _Decimal128 mid, _Decimal128 low) \
{                                                                             \
  _Decimal128 result;                                                         \
  int rnd = __fe_dec_getround();                                              \
  __fe_dec_setround (7); /* Round to prepare for shorter.  */                 \
  hi = hi * 1e17DL;                                                           \
  result = (hi + mid) * 1e17DL;                                               \
  /* These should force the add to happen under the new rounding mode.  */    \
  asm volatile ("" : : "f" (result));                                         \
  result += low;                                                              \
  asm volatile ("" : : "f" (result));                                         \
  __fe_dec_setround (rnd);                                                    \
  return result; /* Implicitly round result into return type.  */             \
}

/* Define combine_and_truncd32 and combine_and_truncd64.  */
COMBINE_AND_TRUNC(a,b,c,32)
COMBINE_AND_TRUNC(a,b,c,64)

/* This is a bit ugly, but the mantissa extraction functions do not
   have an s390 optimized version, so we defer to them.  */
#include_next "convert_helpers.h"

#endif /* _CONVERTHELPERS_S390 */
