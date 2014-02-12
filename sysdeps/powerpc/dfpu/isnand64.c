/* _Decimal64 isNaN classification function.

   Copyright (C) 2010-2014 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Ryan S. Arnold <rsa@us.ibm.com>

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
#include <ieee754r_private.h>

int
__isnand64 (_Decimal64 x)
{
  int cr0;

  asm ("dtstdc 0,%1,3\n"
       "mfcr   %0, 0\n"
    : "=r" (cr0)
    : "f" (x)
    : "cr0");

  return (cr0 & 0x20000000) ? 1 : 0;
}
hidden_def (__isnand64)
weak_alias (__isnand64, isnand64)
