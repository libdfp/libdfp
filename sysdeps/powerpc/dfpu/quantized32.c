/* Set the exponent of x to the exp of y, trying to preserve the value of x

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2010-2015 Free Software Foundation, Inc.

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

_Decimal32
__quantized32 (_Decimal32 x, _Decimal32 y)
{
  /* Always use _Decimal64 asm insns for _Decimal32 since there are no
   * _Decimal32 insns.  */
  _Decimal64 result;
  register _Decimal64 tmp;
  _Decimal64 f3, f4;

  double f2;
  union _ld_t
  {
    long long int l;
    double f;
  };
  static union _ld_t u = { .f = 0x0.0000000000001p-1022 };
  static union _ld_t v = { .f = 0x0.000000000018ep-1022 };
  union _ld_t t;

  __asm__("dqua %0,%1,%2,3;"
	: "=f" (result): "f" ((_Decimal64) y), "f" ((_Decimal64) x));

  /* Get the number of digits.
     Based on numdigits.h.  */
  tmp = result;
  asm (
    "diex %0,%4,%3\n\t"
    "drrnd %2,%5,%0,1\n\t"
    "dxex %1,%2\n\t"
    : "=&d"(f3), "=&d"(f2), "=&d"(f4)
    : "d"(tmp), "d"(v.f), "d"(u.f));

  /* v.l holds the normalized reference exponent.
     f2 holds the computed exponent after reround.  */
  t.f = f2;
  /* Verify if the result don't causes overflow when converted to 32 bits.
     If yes return NaN  */
  if (t.l - v.l + 1 > 7)
    return (_Decimal32) DEC_NAN;

  return (_Decimal32) result;
}
weak_alias (__quantized32, quantized32)
