/* Decimal Floating Point 128-bit round to integer.  POWER6 PowerPC32 version.

   Copyright (C) 2008-2015 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 51 Franklin St - Fifth Floor, Boston,
   MA 02110-1301 USA  */

#include <math.h>

_Decimal128 __rintd128 (_Decimal128 a)
{
  register _Decimal128 fr0 asm("fr0") = a;
  asm (
   "drintxq 0,%0,%0,3\n\t"
   : "=f"(fr0) : "0"(fr0));

  return fr0;
}
weak_alias(__rintd128, rintd128)
