/* _Decimal32 issignaling classification function.

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

int
__issignalingd64 (_Decimal64 x)
{
  int cr0;

  /* DCM bit 5 is signaling NaN  */
  asm ("dtstdc cr0,%1,1\n"
       "mfcr   %0, 128\n"
       : "=r" (cr0)
       : "f" (x)
       : "cr0");

  /* cr0 bits are 28:31 and:
     - 0010 operand positive with match
     - 1010 operand negative with math  */
  return (cr0 & 0x20000000) ? 1 : 0;
}
weak_alias (__issignalingd64, issignalingd64)
