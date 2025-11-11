/* Compute the quantum exponend of a finite argument.

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

#define FUNCTION_NAME quantum
#include <dfpmacro.h>

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x)
{
  long long exp;
#if _DECIMAL_SIZE == 128
  _Decimal128 tmp;
  _Decimal128 ref = 1e-6176DL;
#else
  _Decimal64 tmp;
  _Decimal64 ref = 1e-398DD;
#endif

#if _DECIMAL_SIZE == 32
  /* Preserve sNaN on extending to _Decimal64.  */
  __asm__ ("ldetr %0,%1,8" : "=f" (tmp) : "f" (x));
# else
  tmp = x;
#endif


  /* Extract the biased exponent of tmp and produce the result by inserting
     this exponent to ref.  */
  __asm__ (
#if _DECIMAL_SIZE == 128
	   "eextr %1,%0\n\t"
	   "iextr %0,%2,%1"
#else
	   "eedtr %1,%0\n\t"
	   "iedtr %0,%2,%1"
#endif
	   : "+f" (tmp), "=d" (exp) : "f" (ref));

#if _DECIMAL_SIZE == 32
  _Decimal32 tmp32;
  /* Preserve sNaN on shortening to _Decimal32.  */
  __asm__ ("ledtr %0,0,%1,8" : "=f" (tmp32) : "f" (tmp));
  return tmp32;
#else
  return tmp;
#endif
}
weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
