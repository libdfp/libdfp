/* Returns true if x and y have the same exponent

   Copyright (C) 2018 Free Software Foundation, Inc.

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

#ifndef _DECIMAL_SIZE
# define _DECIMAL_SIZE 32
#endif

#include <math.h>

#define FUNCTION_NAME samequantum
#include <dfpmacro.h>

_Bool
INTERNAL_FUNCTION_NAME (DEC_TYPE x, DEC_TYPE y)
{
  int result;

#if _DECIMAL_SIZE == 128
  _Decimal128 tmpx = x;
  _Decimal128 tmpy = y;
#else
  _Decimal64 tmpx = (_Decimal64) x;
  _Decimal64 tmpy = (_Decimal64) y;
#endif

  __asm__ ("lhi %0,1    \n\t"  /* Same exponent.  */
#if _DECIMAL_SIZE == 128
	   "cextr %1,%2 \n\t"
#else
	   "cedtr %1,%2 \n\t"
#endif
	   "je 1f       \n\t"  /* Jump away if exponents are equal.  */
	   "lhi %0,0    \n\t"  /* Not same exponent.  */
	   "1:"
	   : "=d" (result) : "f" (tmpx), "f" (tmpy) : "cc");

  return result;
}
weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
