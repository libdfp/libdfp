/* Returns non-zero if the _Decimal32 is nan

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
   if not, write to the Free Software Foundation, Inc., 51 Franklin
   Street, Fifth Floor, Boston, MA 02110-1301 USA.

   Please see libdfp/COPYING.txt for more information.  */

#ifndef _DECIMAL_SIZE
# define _DECIMAL_SIZE 32
#endif

#include <math.h>
#include <ieee754r_private.h>

#ifndef FUNCTION_NAME
# define FUNCTION_NAME isnan
#endif
#include <dfpmacro.h>

int
INTERNAL_FUNCTION_NAME (DEC_TYPE x)
{
  int result;
  __asm__ ("lhi %0,1     \n\t" /* Is nan.  */
	   /* Is data class quiet nan, signaling nan?  */
#if _DECIMAL_SIZE == 128
	   "tdcxt %1,15 \n\t"
#elif _DECIMAL_SIZE == 64
	   "tdcdt %1,15 \n\t"
#else
	   "tdcet %1,15 \n\t"
#endif
	   "jl 1f        \n\t" /* Jump away if it is nan.  */
	   "lhi %0,0     \n\t" /* Is not nan.  */
	   "1:"
	   : "=d" (result) : "f" (x) : "cc");

  return result;
}
hidden_def (INTERNAL_FUNCTION_NAME)
weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
