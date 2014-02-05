/* _Decimal32 issignaling classification function.

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

int
__issignalingd32 (_Decimal32 x)
{
  _Decimal64 input = x;
  int cr0;

  /* DCM bit 5 is signaling NaN  */
  asm ("dtstdc cr0,%1,33\n"
       "mfcr   %0, 0\n"
       : "=r" (cr0)
       : "f" (input)
       : "cr0");

  /* cr0 bits are 28:31 and:
     - 0010 operand positive with match
     - 1010 operand negative with math  */
  return (cr0 & 0x20000000) ? 1 : 0;
}
weak_alias (__issignalingd32, issignalingd32)
