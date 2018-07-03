/* Returns non-zero if the _Decimal32 is finite

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

#ifndef FUNCTION_NAME
# define FUNCTION_NAME isfinite
#endif
#include <dfpmacro.h>

int
INTERNAL_FUNCTION_NAME (DEC_TYPE x)
{
  int result;
  /* Returns non-zero if data class is not FP_NAN and not FP_INFINITE.  */
  __asm__ ("lhi %0,0      \n\t" /* Is not finite.  */
	   /* Is data class zero, subnormal or normal?  */
#if _DECIMAL_SIZE == 128
	   "tdcxt %1,4032 \n\t"
#elif _DECIMAL_SIZE == 64
	   "tdcdt %1,4032 \n\t"
#else
	   "tdcet %1,4032 \n\t"
#endif
	   "je 1f         \n\t" /* Jump away if it is nan or infinite.  */
	   "lhi %0,1      \n\t" /* Is finite.  */
	   "1:"
	   : "=d" (result) : "f" (x) : "cc");

  return result;
}
hidden_def (INTERNAL_FUNCTION_NAME)
weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)

/* We erroneously published a version of math.h which used 'finite' instead of
 * 'isfinite' and math.h contained a polymorphic 'isfinite()' function which
 * inlined calles to 'finited*' so we've created aliases for compatability.  */
#define EXTERNAL_FINITE_NAME  FUNC_D(finite)
#define INTERNAL_FINITE_NAME  PASTE(__,EXTERNAL_FINITE_NAME)
strong_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FINITE_NAME)
strong_alias (INTERNAL_FUNCTION_NAME, INTERNAL_FINITE_NAME)
