/* Returns non-zero if the _Decimal128 is infinite
   Copyright (C) 2008 IBM Corporation.
   Copyright (C) 2014-2015 Free Software Foundation, Inc.

   Author(s): Pete Eberlein <eberlein@us.ibm.com>

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
__isinfd128 (_Decimal128 x)
{
  int cr0;

  asm ("dtstdcq cr0,%1,0x04\n"
       "mfcr     %0, 128\n"
       : "=r" (cr0)
       : "f" (x)
       : "cr0");

  /* cr0 bits are 28:31 and:
     - 0010 operand positive with match
     - 1010 operand negative with math  */
  return (cr0 & 0x20000000) ? (cr0 & 0x80000000 ? -1 : 1) : 0;
}
hidden_def (__isinfd128)
weak_alias (__isinfd128, isinfd128)
