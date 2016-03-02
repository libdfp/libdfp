/* Decimal Floating Point function intended to override the libgcc version.

   Copyright (C) 2016 Free Software Foundation, Inc.

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

_Decimal32
__BACKEND_(trunctdsd2) (_Decimal128 in)
{
  /* On s390, this special version with first a cast to _Decimal64
     and afterwards to _Decimal32 is needed, because otherwise
     the gccs before gcc 5 emits a call to __dpd_trunctdsd2 (this function),
     which would lead to a Segmentation fault at runtime
     due to endless recursive calls.
     This code uses two "load rounded"-instructions one after another.
     Starting with gcc 5, these two load-instructions are emitted
     in the same way and omit this function-call at all.
  */
  return (_Decimal32) (_Decimal64)in;
}
hidden_def (__BACKEND_(trunctdsd2))
