/* Decimal Floating Point 32-bit round to integer.  POWER6 PowerPC32 version.

   Copyright (C) 2008 Free Software Foundation, Inc.
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

_Decimal32 __rintd32 (_Decimal32 a)
{
  asm (
   "dctdp %0,%0\n\t"
   "drintx 0,%0,%0,3\n\t"
   "drsp %0,%0\n\t"
   : "=f"(a) : "0"(a));

  return a;
}
weak_alias(__rintd32, rintd32)
