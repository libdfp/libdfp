/* Round a _Decimal32 type to the nearest integer value, rounding halfway
   cases to even.

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

#define FUNCTION_NAME roundeven
#include <dfpmacro.h>

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x)
{
#if _DECIMAL_SIZE == 128
  _Decimal128 tmp = x;
#else
  _Decimal64 tmp = (_Decimal64) x;
#endif

  /* Round toward to nearest with ties to even without inexact exception.  */
  __asm__ (
#if _DECIMAL_SIZE == 128
	   "fixtr %0,8,%0,4"
#else
	   "fidtr %0,8,%0,4"
#endif
	   : "+f" (tmp));
  return (DEC_TYPE) tmp;
}
weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
