/* Returns non-zero if the _Decimal32 is normalized

   Copyright (C) 2008 IBM Corporation.
   Copyright (C) 2014-2018 Free Software Foundation, Inc.

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

   Please see dfp/COPYING.txt for more information.  */

#include <math.h>
#include <ieee754r_private.h>

int
__isnormald32 (_Decimal32 val)
{
  int cr0;
  _Decimal64 input;

  /* Since _Decimal32 is promoted to _Decimal64, __DEC32_SUBNORMAL_MIN__ is well
   * within the bounds of a _Decimal64.  This means that we need to do our range
   * check for __DEC32_SUBNORMAL_MIN__ before dropping into the asm code.  */
  if (val > -__DEC32_MIN__ && val < __DEC32_MIN__)
    return 0;

  input = val;

  asm ("dtstdc cr0,%1,0x08\n"
       "mfcr   %0, 128\n"
    : "=r" (cr0)
    : "f" (input)
    : "cr0");

  return (cr0 & 0x20000000) ? 1 : 0;
}
weak_alias (__isnormald32, isnormald32)
