/* Prototypes for assisting converting larger numbers.
 
   Copyright (C) 2015 Free Software Foundation, Inc.

   Author(s): Paul E. Murphy <murphyp@linux.vnet.ibm.com>

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

#ifndef _CONVERTHELPERS_
#define _CONVERTHELPERS_

#include "fenv_libdfp.h"

#define COMBINE_AND_TRUNC(a,b,c,SIZE)                                         \
static inline _Decimal ## SIZE                                                \
combine_and_truncd ## SIZE (_Decimal128 hi, _Decimal128 mid, _Decimal128 low) \
{                                                                             \
  _Decimal128 result;                                                         \
  fenv_t rnd = fegetenv_register();                                           \
  asm volatile ("mtfsfi 7, 7, 1"); /* Enforce round to prep for shorter.  */  \
  hi = __builtin_dscliq (hi, 17);                                             \
  result = (hi + mid) * 1e17DL;                                               \
  /* This is a hack. We need to ensure this and above happens under           \
     the correct rounding mode. */                                            \
  asm volatile ("daddq %0,%1,%2" : "=f" (result) : "f" (result), "f" (low) ) ;\
  fesetenv_register (rnd);                                                    \
  return result; /* Implicitly round result into return type.  */             \
}

/* Define combine_and_truncd32 and combine_and_truncd64.  */
COMBINE_AND_TRUNC(a,b,c,32)
COMBINE_AND_TRUNC(a,b,c,64)

/* This is an optimized copy of frexpd128 which removes
   checks and tests which aren't needed, and performs
   a free 10^x shift when setting the normalized exponent.  */
static inline _Decimal128
getmantandexpd128(_Decimal128 x, int *y, int shift, _Decimal128 _y)
{
  _Decimal128 result;
  int digits, exponent;
  _Decimal128 f3;

  double f1, f2;
  union _ld_t
  {
    long long int l;
    double f;
  };
  static union _ld_t u = { .f = 0x0.0000000000001p-1022 };
  static union _ld_t v = { .f = 0x0.0000000001820p-1022 };
  union _ld_t t;

  (void) _y; /* Mark as unused.  */

  asm (
    "dxexq  %[a],%[d]\n\t"
    "diexq  %[b],%[e],%[d]\n\t"
    "drrndq %[b],%[f],%[b],1\n\t"
    "dxexq  %[c],%[b]\n\t"
    : [a] "=&d"(f1), [b] "=&d"(f3), [c] "=&d"(f2)
    : [d] "d"(x), [e] "d"(v.f), [f] "d"(u.f));

  t.f = f2;
  digits = t.l - v.l + 1;

  t.f = f1;
  exponent = t.l - 6176;

  *y = digits + exponent;

  t.l = -digits + shift + 6176;
  asm("diexq %0, %1, %2\n\t" : "=d"(result) : "d"(t.f), "d"(x));

  return result;
}

static inline _Decimal64
getmantandexpd64(_Decimal64 x, int *y, int shift, _Decimal64 _y)
{
  _Decimal64 result;
  int digits, exponent;
  _Decimal64 f3;
  double f1, f2;

  union _ld_t
  {
    long long int l;
    double f;
  };
  static union _ld_t u = { .f = 0x0.0000000000001p-1022 };
  static union _ld_t v = { .f = 0x0.000000000018Ep-1022 };
  union _ld_t t;

  (void) _y; /* Mark as unused.  */

  asm (
    "dxex  %[a],%[d]\n\t"
    "diex  %[b],%[e],%[d]\n\t"
    "drrnd %[b],%[f],%[b],1\n\t"
    "dxex  %[c],%[b]\n\t"
    : [a] "=&d"(f1), [b] "=&d"(f3), [c] "=&d"(f2)
    : [d] "d"(x), [e] "d"(v.f), [f] "d"(u.f));

  t.f = f2;
  digits = t.l - v.l + 1;

  t.f = f1;
  exponent = t.l - 398;

  *y = digits + exponent;

  t.l = -digits + shift + 398;
  asm("diex %0, %1, %2\n\t" : "=d"(result) : "d"(t.f), "d"(x));

  return result;
}

static inline _Decimal32
getmantandexpd32(_Decimal32 x, int *y, int shift, _Decimal32 _y)
{
  int digits, exponent;
  register _Decimal64 tmp = x;
  _Decimal64 f3;

  double f1, f2;
  union _ld_t
  {
    long long int l;
    double f;
  };
  static union _ld_t u = { .f = 0x0.0000000000001p-1022 };
  static union _ld_t v = { .f = 0x0.000000000018Ep-1022 };
  union _ld_t t;

  (void) _y; /* Mark as unused.  */

  asm (
    "dxex  %[a],%[d]\n\t"
    "diex  %[b],%[e],%[d]\n\t"
    "drrnd %[b],%[f],%[b],1\n\t"
    "dxex  %[c],%[b]\n\t"
    : [a] "=&d"(f1), [b] "=&d"(f3), [c] "=&d"(f2)
    : [d] "d"(tmp), [e] "d"(v.f), [f] "d"(u.f));

  t.f = f2;
  digits = t.l - v.l + 1;

  t.f = f1;
  exponent = t.l - 398;

  *y = digits + exponent;

  t.l = -digits + shift + 398;
  asm("diex %0, %1, %2\n\t" : "=d"(tmp) : "d"(t.f), "d"(tmp));

  return (_Decimal32) tmp;
}

#endif /* _CONVERTHELPERS_ */
