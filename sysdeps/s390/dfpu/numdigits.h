/* Number of digits functions, optimized for S/390 z9-ec.

   Copyright (C) 2008 IBM Corporation.
   Copyright (C) 2009-2016 Free Software Foundation, Inc.

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
FUNC_D (left_justify) (DEC_TYPE x)
{
#if _DECIMAL_SIZE == 128
  register DEC_TYPE tmp asm ("f0") = x;
  register DEC_TYPE rnd asm ("f4");
#elif _DECIMAL_SIZE == 64
  DEC_TYPE tmp = x;
  DEC_TYPE rnd;
#else
  _Decimal64 tmp = (_Decimal64)x;
  _Decimal64 rnd;
#endif

  int exp;

  asm (
#if _DECIMAL_SIZE == 128
       "rrxtr %0,%2,%3,1\n\t" /* Reround  */
       "eextr %1,%0\n\t" /* Extract Biased Exponent - extended dfp  */
#else /* _DECIMAL_SIZE == 32 || 64  */
       "rrdtr %0,%2,%3,1\n\t" /* Reround  */
       "eedtr %1,%0\n\t" /* Extract Biased Exponent - long dfp  */
#endif

       /* The following magic numbers result from the precision of the
		   data type minus 1.  */
#if _DECIMAL_SIZE == 32
       "ahi %1,-6\n\t"
#elif _DECIMAL_SIZE == 64
       "ahi %1,-15\n\t"
#else /* _DECIMAL_SIZE == 128 */
       "ahi %1,-33\n\t"
#endif

#if _DECIMAL_SIZE == 128
       "iextr %0,%0,%1\n\t" /* Insert Biased Exponent - extended dfp  */
       "qaxtr %0,%2,%0,1"
#else /* _DECIMAL_SIZE == 32 || 64  */
       "iedtr %0,%0,%1\n\t" /* Insert Biased Exponent - long dfp  */
       "qadtr %0,%2,%0,1"
#endif
       : "=f"(rnd), "=d"(exp), "+f"(tmp) : "d"(1)); /* quantize  */

#if _DECIMAL_SIZE == 32
  return (_Decimal32)rnd;
#else
  return rnd;
#endif
}
