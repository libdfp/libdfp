/* Number of digits functions, optimized for S/390 z9-ec.

   Copyright (C) 2008 IBM Corporation.
   Copyright (C) 2009-2015 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Andreas Krebbel  <Andreas.Krebbel@de.ibm.com>

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

/* Alow this file to be included more than once, so don't use the usual
   include guards.  */

#define NUMDIGITS_SUPPORT 1

#ifndef DEC_TYPE
#error DEC_TYPE must be declared
#endif

#ifndef _DECIMAL_SIZE
#error _DECIMAL_SIZE must be declared
#endif

#include "dpd-private.h"

#undef DECIMAL_BIAS
#if _DECIMAL_SIZE == 32
// DECIMAL32 gets widened to DECIMAL64, so it ought to use DECIMAL64 bias
#  define DECIMAL_BIAS (101+297)
#elif _DECIMAL_SIZE == 64
#  define DECIMAL_BIAS 398
#elif _DECIMAL_SIZE == 128
#  define DECIMAL_BIAS 6176
#endif

#ifndef PASTE
# define PASTE(x,y) PASTE2(x,y)
# define PASTE2(x,y) x##y
#endif

#ifndef FUNC_D
# define FUNC_D(x) PASTE(x,PASTE(d,_DECIMAL_SIZE))
#endif



static inline int
FUNC_D (getexp) (DEC_TYPE x)
{
  int result;

#if _DECIMAL_SIZE == 32
  _Decimal64 tmp = (_Decimal64)x;
#elif _DECIMAL_SIZE == 64
  _Decimal64 tmp = x;
#elif _DECIMAL_SIZE == 128
  register _Decimal128 tmp asm ("f0") = x;
#endif

  asm (
#if _DECIMAL_SIZE == 128
  "eextr %0,%1"
#else /* 32 and 64 bit */
  "eedtr %0,%1"
#endif
  : "=d"(result) : "f"(tmp));

  return result - DECIMAL_BIAS;
}


static inline DEC_TYPE
FUNC_D (setexp) (DEC_TYPE x, int exp)
{
  int biased_exp = exp + DECIMAL_BIAS;

#if _DECIMAL_SIZE == 32
  _Decimal64 tmp = (_Decimal64)x;
#elif _DECIMAL_SIZE == 128
  register DEC_TYPE tmp asm ("f0") = x;
#else
  DEC_TYPE tmp = x;
#endif

  asm (
    "llgfr %1,%1   \n\t"
#if _DECIMAL_SIZE == 128
    "iextr %0,%0,%1"
#else
    "iedtr %0,%0,%1"
#endif
    : "+f" (tmp), "+d" (biased_exp));

#if _DECIMAL_SIZE == 32
  return (_Decimal32)tmp;
#else
  return tmp;
#endif
}


static inline int
FUNC_D (numdigits) (DEC_TYPE x)
{
  int result;

#if _DECIMAL_SIZE == 128
  register DEC_TYPE tmp asm ("f0") = x;
#elif _DECIMAL_SIZE == 32
  _Decimal64 tmp = (_Decimal64)x;
#else
  DEC_TYPE tmp = x;
#endif

  asm (
#if _DECIMAL_SIZE == 128
    "esxtr %0,%1"
#else /* 32 and 64 bit */
    "esdtr %0,%1"
#endif
   : "=d"(result) : "f"(tmp));

  /* The Power hardware implementation returns 1 even for a zero
     mantissa.  The software implementation in soft-dfp mimics that
     behaviour.  The S/390 instructions return 0 for zero values.  So
     we have to adjust this here in order to match the behaviour of
     the existing functions.  */
  return (result == 0 ? 1 : result);
}

static DEC_TYPE
__attribute__((unused))
FUNC_D (left_justify) (DEC_TYPE x)
{
#if _DECIMAL_SIZE == 128
# define LJ_INSN_REROUND "rrxtr"
# define LJ_INSN_EXTRACT_EXPONENT "eextr"
# define LJ_INSN_INSERT_EXPONENT "iextr"
# define LJ_INSN_QUANTIZE "qaxtr"
  /* The following magic numbers result from the precision of the
     data type minus 1.  */
# define LJ_ADJUST_EXPONENT 33
  register DEC_TYPE tmp asm ("f0") = x;
  register DEC_TYPE rnd asm ("f4");
#else
# define LJ_INSN_REROUND "rrdtr"
# define LJ_INSN_EXTRACT_EXPONENT "eedtr"
# define LJ_INSN_INSERT_EXPONENT "iedtr"
# define LJ_INSN_QUANTIZE "qadtr"
# if _DECIMAL_SIZE == 64
#  define LJ_ADJUST_EXPONENT 15
  DEC_TYPE tmp = x;
  DEC_TYPE rnd;
# else
#  define LJ_ADJUST_EXPONENT 6
  _Decimal64 tmp = (_Decimal64)x;
  _Decimal64 rnd;
# endif
#endif

  /* Note: The extract / insert biased exponent instructions work with a
     64bit signed register.  But we just use a 32bit signed register as on 31bit
     a 64bit datatype would be stored in a pair of two 32bit registers.
     The biased exponent is always within a 32bit value. This also applies for
     the negative values for NaN and infinity.  */
  int exp;

  asm (LJ_INSN_REROUND " %0,%1,%2,5\n\t" : "=f"(rnd) : "f"(tmp), "d"(1));
  asm (LJ_INSN_EXTRACT_EXPONENT " %0,%1\n\t" : "=d"(exp) : "f"(rnd));

  /* Check for NaN and infinity, The extract biased exponent instruction
     returns < 0 for qnan, snan, and inf.  */
  if (exp >= LJ_ADJUST_EXPONENT)
    exp -= LJ_ADJUST_EXPONENT;
  else if (exp < 0)
    return x;
  else
    exp = 0;

  asm ("lgfr %1,%1\n\t"	/* Extend 32bit to 64bit signed register.  */
       LJ_INSN_INSERT_EXPONENT " %0,%0,%1\n\t" : "+f"(rnd), "+d"(exp));
  asm (LJ_INSN_QUANTIZE " %0,%1,%0,5\n\t" : "+f"(rnd) : "f"(tmp));

#if _DECIMAL_SIZE == 32
  return (_Decimal32)rnd;
#else
  return rnd;
#endif
#undef LJ_INSN_REROUND
#undef LJ_INSN_EXTRACT_EXPONENT
#undef LJ_INSN_INSERT_EXPONENT
#undef LJ_INSN_QUANTIZE
#undef LJ_ADJUST_EXPONENT
}
