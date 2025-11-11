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
   if not, write to the Free Software Foundation, Inc., 51 Franklin
   Street, Fifth Floor, Boston, MA 02110-1301 USA.

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

     Starting with gcc 5, gcc emits two load-rounded-instructions and omit this
     function-call at all. But it uses the false rounding mode "according to the
     current DFP rounding mode" for the d128 -> d64 conversion!

     Starting with gcc 6, gcc emits those instructions and the correct rounding
     mode "Round to prepare for shorter precision" is used for the d128 -> d64
     conversion!  */
  _Decimal32 out;
  _Decimal64 tmp;
  asm ("ldxtr %1,15,%2,0\n\t" /* Load rounded: d128 -> d64 in
       "Round to prepare for shorter precision. " rouding-mode.  */
       "ledtr %0,0,%1,0"
       : "=f" (out), "=&f" (tmp)
       : "f" (in)
       );
  return out;
}
hidden_def (__BACKEND_(trunctdsd2))
