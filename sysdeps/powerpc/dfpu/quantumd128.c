/* _Decimal128 quantum implementation.

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
   if not, write to the Free Software Foundation, Inc., 59 Temple Place,
   Suite 330, Boston, MA 02111-1307 USA.

   Please see libdfp/COPYING.txt for more information.  */

#include <math.h>

/* Return the quantum 1 * 10^exponent(x)  */
_Decimal128 __quantumd128 (_Decimal128 x)
{
  register _Decimal128 input asm("fr0") = x;
  _Decimal128 ret;
  _Decimal128 ref = 1e-6176DL;
  asm ("dxexq   %1,%1\n"      /* Extract exponent  */
       "diexq   %0,%1,%2\n"   /* Sets it to reference value  */
    : "=d"(ret) :
      "d"(input), "d"(ref));
  return ret;
}
weak_alias (__quantumd128, quantumd128)
