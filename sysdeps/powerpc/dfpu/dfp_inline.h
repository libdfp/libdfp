/* Internal ieee754r function prototypes used by libdfp
   for powerpc

   Copyright (C) 2019 Free Software Foundation, Inc.

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

   Please see dfp/COPYING.txt for more information.  */

#ifndef _DFP_INLINE_H
#define _DFP_INLINE_H

#include <dfp/fenv.h>

inline _Decimal128
___quantized128 (_Decimal128 x, _Decimal128 y)
{
  _Decimal128 result;
  asm ("dquaq %0,%1,%2,3\n\t" : "=f" (result) : "f" (y), "f" (x));
  return result;
};

inline _Decimal64
___quantized64 (_Decimal64 x, _Decimal64 y)
{
  _Decimal64 result;
  asm ("dqua %0,%1,%2,3\n\t" : "=f" (result) : "f" (y), "f" (x));
  return result;
};

#define __quantized64(x, y) ___quantized64(x, y)
#define __quantized128(x, y) ___quantized128(x, y)

inline _Decimal128
___isfinited128 (_Decimal128 x)
{
  int cr0;

  asm ("dtstdcq cr0,%1,0x38\n"
       "mfcr     %0, 128\n"
    : "=r" (cr0)
    : "f" (x)
    : "cr0");

  return (cr0 & 0x20000000) ? 1 : 0;
};

inline _Decimal64
___isfinited64 (_Decimal64 x)
{
  int cr0;

  asm ("dtstdc cr0,%1,0x38\n"
       "mfcr %0, 128\n"
    : "=r" (cr0)
    : "f" (x)
    : "cr0");
  return (cr0 & 0x20000000) ? 1 : 0;
};

inline void
__restore_rnd (double *state)
{
  asm volatile ( "mtfsf 1, %0, 0, 1\n" : : "f" (*state));
}

#define __isfinited32(x) ___isfinited64((_Decimal64)x)
#define __isfinited64(x) ___isfinited64(x)
#define __isfinited128(x) ___isfinited128(x)

#ifdef _ARCH_PWR9
#define SET_RESTORE_DROUND(mode) \
  double __rnd __attribute__ ((__cleanup__ (__restore_rnd))); \
  asm volatile ("mffscdrni %0, %1\n" : "=f"(__rnd) : "i" (mode));
#else
/* Note, mffsl decodes as mffs on older machines, so take the free speedup where available. */
/* The mffls snippet is taken from glibc. Thanks Paul Clarke! */
#define SET_RESTORE_DROUND(mode) \
  double __rnd __attribute__ ((__cleanup__ (__restore_rnd))); \
  asm volatile (".machine push; .machine \"power9\"; mffsl %0; .machine pop; mtfsfi 7, %1, 1\n" : "=f" (__rnd) : "i" (mode));
#endif

#endif
