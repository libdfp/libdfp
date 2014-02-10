/* _Decimal32 quantumd32 implementation.

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

/* Return the quantum 1 * 10^exponent(x)  */
_Decimal32 __quantumd32 (_Decimal32 x)
{
  _Decimal64 ret;
  _Decimal64 ref = 1e-398DD;
  asm ("dxex    %0,%1\n"
       "diex    %0,%0,%2\n"
    : "=f"(ret) :
      "f"((_Decimal64) x), "f"(ref));
  return ret;
}
weak_alias (__quantumd32, quantumd32)
