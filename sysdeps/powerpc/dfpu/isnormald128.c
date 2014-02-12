/* Returns non-zero if the _Decimal128 is normalized

   Copyright (C) 2008 IBM Corporation.
   Copyright (C) 2014 Free Software Foundation, Inc.

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
   if not, write to the Free Software Foundation, Inc., 59 Temple Place,
   Suite 330, Boston, MA 02111-1307 USA.

   Please see dfp/COPYING.txt for more normalormation.  */

#include <math.h>
#include <ieee754r_private.h>

int
__isnormald128 (_Decimal128 x)
{
  register _Decimal128 input asm("fr0") = x;
  int cr0;

  asm ("dtstdcq cr0,%1,0x08\n"
       "mfcr     %0, 0\n"
       : "=r" (cr0)
       : "f" (input)
       : "cr0");

  return (cr0 & 0x20000000) ? (cr0 & 0x80000000 ? -1 : 1) : 0;
}
weak_alias (__isnormald128, isnormald128)
