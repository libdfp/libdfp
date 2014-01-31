/* Returns non-zero if the _Decimal128 is a signaling nan

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

   Please see libdfp/COPYING.txt for more information.  */

#include <math.h>
#include <math_private.h>

int
__issignalingd128 (_Decimal128 x)
{
  uint64_t hx;
  GET_DEC128_HIGH_WORD64 (hx, x);

  /* 0 11111 10 ... == sNaN  */
  return (hx & DEC128_SNAN_MASK64) == DEC128_SNAN_MASK64;
}
weak_alias (__issignalingd128, issignalingd128)
