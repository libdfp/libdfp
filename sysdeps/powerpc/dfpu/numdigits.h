/* Number of digits functions, optimized for Power6.

   Copyright (C) 2006-2008 IBM Corporation.
   Copyright (C) 2006-2015 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Pete Eberlein <eberlein@us.ibm.com>
              Michael Meissner <meissner@linux.vnet.ibm.com>
              Peter Bergner <bergner@linux.vnet.ibm.com>
	      Ryan S. Arnold <rsa@linux.vnet.ibm.com>

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

#undef DECIMAL_BIAS
#undef DECIMAL_BIAS_DOUBLE
#undef Q
#if _DECIMAL_SIZE == 32
// DECIMAL32 gets widened to DECIMAL64, so it ought to use DECIMAL64 bias
# define DECIMAL_BIAS         (101+297)
# define DECIMAL_BIAS_DOUBLE  0x0.000000000018ep-1022
# define Q ""
#elif _DECIMAL_SIZE == 64
# define DECIMAL_BIAS         398
# define DECIMAL_BIAS_DOUBLE  0x0.000000000018ep-1022
# define Q ""
#elif _DECIMAL_SIZE == 128
# define DECIMAL_BIAS         6176
# define DECIMAL_BIAS_DOUBLE  0x0.000000000182p-1022
# define Q "q"
#else
#error _DECIMAL_SIZE must be '32', '64' or '128'
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
  long long i;
#if _DECIMAL_SIZE == 32
  asm ("dctdp %0,%1\n\t"
       "dxex %0,%0\n\t" : "=d"(f) : "f"(x));
#elif _DECIMAL_SIZE == 64
  asm ("dxex %0,%1" : "=d"(f) : "d"(x));
#elif _DECIMAL_SIZE == 128
  asm ("dxexq %0,%1" : "=d"(f) : "d"(x));
#endif
  asm ("stfd %1,%0" : "=m"(i) : "d"(f));
  return i - DECIMAL_BIAS;
}

static inline DEC_TYPE
FUNC_D (setexp) (DEC_TYPE x, int exp)
{
  DEC_TYPE tmp = x;
  union {
    long long int l;
    double f;
  } e;

  e.l = exp + DECIMAL_BIAS;

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
  _Decimal64 f3, f4;
#else
  register DEC_TYPE tmp = x;
  DEC_TYPE f3, f4;
#endif
  double f1, f2;
  union _ld_t
  {
    long long int l;
    double f;
  };
  static union _ld_t u = { .f = 0x0.0000000000001p-1022 };
  static union _ld_t v = { .f = DECIMAL_BIAS_DOUBLE };
  union _ld_t t;

  asm (
    "dxex" Q " %0,%3\n\t"
    "diex" Q " %2,%4,%3\n\t"
    : "=&d"(f1), "=&d"(f2), "=&d"(f3)
    : "d"(tmp), "d"(v.f));

  /* Check for NaN and infinity, dxex returns < 0 for qnan, snan, and inf.  */
  t.f = f1;
  if (t.l < 0)
    return t.l;

  asm (
    "drrnd" Q " %0,%2,%3,1\n\t"
    "dxex" Q " %1,%0\n\t"
    : "=&d"(f4), "=&d"(f2)
    : "d"(u.f), "d"(f3));

  /* v.l holds the normalized reference exponent.
     f2 holds the computed exponent after reround.  */
  t.f = f2;
  return t.l - v.l + 1;
}

static inline DEC_TYPE
FUNC_D (left_justify) (DEC_TYPE x)
{
#undef ADJUST
#if _DECIMAL_SIZE == 32
# define ADJUST 6
  _Decimal64 tmp = x;
  _Decimal64 rnd;
#else
# if _DECIMAL_SIZE == 64
#  define ADJUST 15
# elif _DECIMAL_SIZE == 128
#  define ADJUST 33
# endif
  DEC_TYPE tmp = x;
  DEC_TYPE rnd;
#endif
  double tmp2;
  union int_dbl
  {
    long long int l;
    double f;
  };
  static union int_dbl d = { .f = 0x0.0000000000001p-1022 };
  union int_dbl d2;

  asm ("drrnd" Q " %0,%1,%2,1" : "=d"(rnd) : "d"(d.f), "d"(tmp));
  asm ("dxex" Q " %0,%1\n\t" : "=d"(tmp2) : "d"(rnd));

  d2.f = tmp2;
  d2.l -= ADJUST;
  tmp2 = d2.f;

  asm ("diex" Q " %0,%1,%0\n\t" : "=d"(rnd) : "d"(tmp2), "0"(rnd));
  asm ("dqua" Q " %0,%0,%2,1\n\t" : "=d"(rnd) : "0"(rnd), "d"(tmp));

 /* cast is necessary if input is _Decimal32 in order to convert it from
  * _Decima64 back to _Decimal32 since the calculation is done in _Decimal64.  */
  return (DEC_TYPE)rnd;
}
