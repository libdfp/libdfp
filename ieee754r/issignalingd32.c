/* Returns non-zero if the _Decimal32 is a signaling nan

   Copyright (C) 2014-2015 Free Software Foundation, Inc.

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
   if not, write to the Free Software Foundation, Inc., 51 Franklin
   Street, Fifth Floor, Boston, MA 02110-1301 USA.

   Please see libdfp/COPYING.txt for more information.  */

#include <math.h>
#include <math_private.h>

int
__issignalingd32 (_Decimal32 x)
{
  uint32_t hx;
  GET_DEC32_WORD (hx, x);

  /* 0 11111 10 ... == sNaN  */
  return (hx & DEC32_SNAN_MASK) == DEC32_SNAN_MASK;
}
weak_alias (__issignalingd32, issignalingd32)
