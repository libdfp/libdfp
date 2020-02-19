/* Set the exponent of x to the exp of y, trying to preserve the value of x

   Copyright (C) 2018 Free Software Foundation, Inc.

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

#ifndef _DECIMAL_SIZE
# define _DECIMAL_SIZE 32
#endif

#include <math.h>
#include <ieee754r_private.h>

#define FUNCTION_NAME quantize
#include <dfpmacro.h>

#if _DECIMAL_SIZE == 32
# undef _DECIMAL_SIZE
# define _DECIMAL_SIZE 64
/* As we use _Decimal64 instructions for _Decimal32 inputs,
   we need to check for overflow by calling numdigitsd64
   which is defined in numdigits.h.  */
# include <numdigits.h>
# undef _DECIMAL_SIZE
# define _DECIMAL_SIZE 32
#endif

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x, DEC_TYPE y)
{
#if _DECIMAL_SIZE == 128
  _Decimal128 result;

  __asm__ ("qaxtr %0,%1,%2,0;"
	   : "=f" (result): "f" ((_Decimal128) x), "f" ((_Decimal128) y));

#else /* _DECIMAL_SIZE == 64 || 32  */
  /* Always use _Decimal64 asm insns for _Decimal32 since there are no
     _Decimal32 insns.  */
  _Decimal64 result;

  __asm__ ("qadtr %0,%1,%2,0;"
	   : "=f" (result): "f" ((_Decimal64) x), "f" ((_Decimal64) y));

# if _DECIMAL_SIZE == 32
  /* Verify if the result don't causes overflow when converted to _Decimal32.
     If yes return NaN.  */
  if (numdigitsd64 (result) > 7)
    return (_Decimal32) DEC_NAN;
# endif
#endif

  return (DEC_TYPE) result;
}
hidden_def (INTERNAL_FUNCTION_NAME)
weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
