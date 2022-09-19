/* Returns non-zero if the _Decimal64 is normalized

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

   Please see dfp/COPYING.txt for more information.  */

#include <math.h>
#include <ieee754r_private.h>

int
__isnormald64 (_Decimal64 x)
{
  int cr0;

  asm ("dtstdc cr0,%1,0x08\n"
       "mfcr   %0, 128\n"
    : "=r" (cr0)
    : "f" (x)
    : "cr0");

  return (cr0 & 0x20000000) ? 1 : 0;
}
weak_alias (__isnormald64, isnormald64)
