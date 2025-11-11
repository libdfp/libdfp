/* _Decimal32 isinf classification function.

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2010-2015 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Steven Munroe <munroesj@us.ibm.com>
              Ryan S. Arnold <rsa@us.ibm.com>

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
#include <ieee754r_private.h>

int
__isinfd32 (_Decimal32 x)
{
  int cr0;

  _Decimal64 input = x;
  asm ("dtstdc cr0,%1,0x04\n"
       "mfcr    %0, 128\n"
       : "=r" (cr0)
       : "f" (input)
       : "cr0");

  /* cr0 bits are 28:31 and:
     - 0010 operand positive with match
     - 1010 operand negative with math  */
  return (cr0 & 0x20000000) ? (cr0 & 0x80000000 ? -1 : 1) : 0;
}
hidden_def (__isinfd32)
weak_alias (__isinfd32, isinfd32)
