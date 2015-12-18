/* Prototypes for assisting converting larger numbers.
 
   Copyright (C) 2015 Free Software Foundation, Inc.

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

   Please see dfp/COPYING.txt for more information.  */

#ifndef _CONVERTHELPERS_
#define _CONVERTHELPERS_

#include "fenv_libdfp.h"

#define COMBINE_AND_TRUNC(a,b,c,SIZE)                                         \
static inline _Decimal ## SIZE                                                \
combine_and_truncd ## SIZE (_Decimal128 hi, _Decimal128 mid, _Decimal128 low) \
{                                                                             \
  _Decimal128 result;                                                         \
  fenv_t rnd = fegetenv_register();                                           \
  asm volatile ("mtfsfi 7, 7, 1"); /* Enforce round to prep for shorter.  */  \
  hi = __builtin_dscliq (hi, 17);                                             \
  result = (hi + mid) * 1e17DL;                                               \
  /* This is a hack. We need to ensure this and above happens under           \
     the correct rounding mode. */                                            \
  asm volatile ("daddq %0,%1,%2" : "=f" (result) : "f" (result), "f" (low) ) ;\
  fesetenv_register (rnd);                                                    \
  return result; /* Implicitly round result into return type.  */             \
}

/* Define combine_and_truncd32 and combine_and_truncd64.  */
COMBINE_AND_TRUNC(a,b,c,32)
COMBINE_AND_TRUNC(a,b,c,64)

#endif /* _CONVERTHELPERS_ */
