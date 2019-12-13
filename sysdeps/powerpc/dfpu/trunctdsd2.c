/* Decimal Floating Point truncate 128-bit to 32-bit.

   Copyright (C) 2019 Free Software Foundation, Inc.

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

#include <dfpacc.h>

#include "dfp_inline.h"

_Decimal32
__dpd_trunctdsd2 (_Decimal128 d)
{
  _Decimal64 tmp;
  {
    SET_RESTORE_DROUND (7);	/* Round to prepare for shorter */
    tmp = d;
    /* TODO: The compiler should respect our wishes, but we can't give it the hint
       we've munged fpscr with inline asm. So, create a dependency between
       the rounding variable and our computation. */
    asm volatile ("":"+f" (__rnd), "+f" (d));
  }
  /* Round from D64 -> D32 using the current rounding mode. */
  return tmp;
}

hidden_def (__dpd_trunctdsd2)
