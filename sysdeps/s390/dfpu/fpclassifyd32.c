/* _Decimal32 fpclassify classification function.

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
   if not, write to the Free Software Foundation, Inc., 59 Temple Place,
   Suite 330, Boston, MA 02111-1307 USA.

   Please see libdfp/COPYING.txt for more information.  */

#ifndef _DECIMAL_SIZE
# define _DECIMAL_SIZE 32
#endif

#include <math.h>
#include <ieee754r_private.h>

#ifndef FUNCTION_NAME
# define FUNCTION_NAME fpclassify
#endif
#include <dfpmacro.h>

int
INTERNAL_FUNCTION_NAME (DEC_TYPE x)
{
  int result;
  /* Test the data class of x in order FP_NORMAL, FP_ZERO, FP_SUBNORMAL,
     FP_INFINITE, FP_NAN.  The most likely case exits early.  */
#if _DECIMAL_SIZE == 128
# define INSN_TDC "tdcxt"
#elif _DECIMAL_SIZE == 64
# define INSN_TDC "tdcdt"
#else
# define INSN_TDC "tdcet"
#endif

  __asm__ (INSN_TDC " %1,192  \n\t"
	   "lhi %0,4          \n\t" /* FP_NORMAL  */
	   "jl 1f             \n\t"
	   INSN_TDC " %1,3072 \n\t"
	   "lhi %0,2          \n\t" /* FP_ZERO  */
	   "jl 1f             \n\t"
	   INSN_TDC " %1,768  \n\t"
	   "lhi %0,3          \n\t" /* FP_SUBNORMAL  */
	   "jl 1f             \n\t"
	   INSN_TDC " %1,48   \n\t"
	   "lhi %0,1          \n\t" /* FP_INFINITY  */
	   "jl 1f             \n\t"
	   "lhi %0,0          \n\t" /* FP_NAN  */
	   "1:"
	   : "=d" (result) : "f" (x) : "cc");

  return result;
}
hidden_def (INTERNAL_FUNCTION_NAME)
weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
