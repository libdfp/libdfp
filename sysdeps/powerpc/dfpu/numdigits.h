/* Number of digits functions, optimized for Power6.

   Copyright (C) 2006, 2007, 2008 IBM Corporation.
   Copyright (C) 2006, 2007, 2008, 2009, 2010 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Pete Eberlein <eberlein@us.ibm.com>
              Michael Meissner <meissner@linux.vnet.ibm.com>
              Peter Bergner <bergner@linux.vnet.ibm.com>

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

   Please see dfp/COPYING.txt for more information.  */

/* Alow this file to be included more than once, so don't use the usual
   include guards.  */

#define NUMDIGITS_SUPPORT 1

#ifndef DEC_TYPE
#error DEC_TYPE must be declared
#endif

#ifndef _DECIMAL_SIZE
#error _DECIMAL_SIZE must be declared
#endif

#include <dpd-private.h>

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
# define FUNC_D(x)		PASTE(x,PASTE(d,_DECIMAL_SIZE))
#endif

static inline int
FUNC_D (getexp) (DEC_TYPE x)
{
  double f;
  int i;
#if _DECIMAL_SIZE == 32
  asm ("dctdp %0,%1\n\t"
       "dxex %0,%0\n\t" : "=d"(f) : "f"(x));
#elif _DECIMAL_SIZE == 64
  asm ("dxex %0,%1" : "=d"(f) : "d"(x));
#elif _DECIMAL_SIZE == 128
  /* dec quad needs to be in an even-odd fr pair */
  register DEC_TYPE tmp asm ("fr0") = x;
  asm ("dxexq %0,%1" : "=d"(f) : "d"(tmp));
#endif
  asm ("stfiwx %1,%y0" : "=Z"(i) : "d"(f));
  return i - DECIMAL_BIAS;
}

#ifndef PASTE
# define PASTE(x,y) PASTE2(x,y)
# define PASTE2(x,y) x##y
#endif

static inline DEC_TYPE
FUNC_D (setexp) (DEC_TYPE x, int exp)
{
#if _DECIMAL_SIZE == 128
  register DEC_TYPE tmp asm ("fr0") = x;
#else
  register DEC_TYPE tmp = x;
#endif
  union {
    int i[2];
    double f;
  } e;

  e.i[0] = 0;
  e.i[1] = exp + DECIMAL_BIAS;

  asm (
#if _DECIMAL_SIZE == 32
   "dctdp %0,%0\n\t"
   "diex %0,%1,%0\n\t"
   "drsp %0,%0\n\t" 
#elif _DECIMAL_SIZE == 64
   "diex %0,%1,%0\n\t"
#elif _DECIMAL_SIZE == 128
    "diexq %0,%1,%0\n\t"
#endif
    : "=f"(tmp) : "f"(e.f), "0"(tmp));
  return tmp;
}

static inline int
FUNC_D (numdigits) (DEC_TYPE x)
{
#if _DECIMAL_SIZE == 32
  register _Decimal64 tmp = x;
#else
  register DEC_TYPE tmp = x;
#endif
  double f1, f2;
  DEC_TYPE f3;
  int i1, i2;
  static union
  {
    int i[2];
    double f;
  } u = { { 0, 1 } };
  asm (
#if _DECIMAL_SIZE != 128
    "dxex %2,%5\n\t"
    "drrnd %4,%6,%5,1\n\t"
    "dxex %3,%4\n\t"
#else /* _DECIMAL_SIZE == 128 */
    "dxexq %2,%5\n\t"
    "drrndq %4,%6,%5,1\n\t"
    "dxexq %3,%4\n\t"
#endif
    "stfiwx %2,%y0\n\t"
    "stfiwx %3,%y1\n\t"
    : "=Z"(i1), "=Z"(i2), "=&d"(f1), "=&d"(f2), "=&d"(f3)
    : "d"(tmp), "d"(u.f));
  return i2 - i1 + 1;
}

static inline DEC_TYPE
FUNC_D (left_justify) (DEC_TYPE x)
{
#if _DECIMAL_SIZE == 32
  register _Decimal64 tmp = x;
  register _Decimal64 rnd;
#else
  register DEC_TYPE tmp = x;
  register DEC_TYPE rnd;
#endif
  double tmp2;
  union int_dbl
  {
    int i[2];
    double f;
  };
  static union int_dbl d = { { 0, 1 } };
#if _DECIMAL_SIZE==32
# define ADJUST 6
#elif _DECIMAL_SIZE==64
# define ADJUST 15
#elif _DECIMAL_SIZE==128
# define ADJUST 33
#else
# error "Unknown decimal size"
#endif

#ifdef __VSX__
  static vector int vsx_adjust = { 0, ADJUST, 0, 0 };
  register vector int tmp3;
#else
  union int_dbl d2;
#endif

#if _DECIMAL_SIZE != 128
# define Q ""
#else
# define Q "q"
#endif

  asm ("drrnd" Q " %0,%1,%2,1" : "=d"(rnd) : "d"(d.f), "d"(tmp));
  asm ("dxex" Q " %0,%1\n\t" : "=d"(tmp2) : "d"(rnd));

#ifdef __VSX__
  asm ("xxlxor %x0,%x1,%x1" : "=v"(tmp3) : "d"(tmp2));
  asm ("vsubuwm %0,%1,%2" : "=v"(tmp3) : "v"(tmp3), "v"(vsx_adjust));
  asm ("xxlxor %x0,%x1,%x1" : "=d"(tmp2) : "v"(tmp3));
#else
  d2.f = tmp2;
  d2.i[1] -= ADJUST;
  tmp2 = d2.f;
#endif
  asm ("diex" Q " %0,%1,%0\n\t" : "=d"(rnd) : "d"(tmp2), "0"(rnd));
  asm ("dqua" Q " %0,%0,%2,1\n\t" : "=d"(rnd) : "0"(rnd), "d"(tmp));

 /* cast is necessary if input is _Decimal32 in order to convert it from
  * _Decima64 back to _Decimal32 since the calculation is done in _Decimal64.  */
  return (DEC_TYPE)rnd;
}
