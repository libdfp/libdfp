/* floatti* implementation for powerpc64.

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
   if not, write to the Free Software Foundation, Inc., 51 Franklin
   Street, Fifth Floor, Boston, MA 02110-1301 USA.

   Please see libdfp/COPYING.txt for more information.  */

#ifndef FUNC
# define FUNC     floattitd
# define RET_TYPE _Decimal128
# define RET_SIZE 128
#endif

/* For Power10 and newer, use an optimized routine for signed 128b integers.  */
#if defined (_ARCH_PWR10) && !defined (UNSIGNED)

#include <dfpacc.h>
#include "fenv_libdfp.h"

RET_TYPE
__BACKEND_ (FUNC) (INT128 a)
{
  union v128i128 {
    vector __int128 v128;
    __int128 i128;
  } r;
  r.i128 = a;
  _Decimal128 v;

  asm volatile ("dcffixqq %[e],%[c]\n\t" : [e] "=d" (v) : [c] "v" (r.v128));

#if RET_SIZE == 32
  /* Some older gcc versions will not inline a round from
     _Decimal128 to _Decimal32.  Inline the asm. */
  double fpscrv;
  _Decimal64 tmpd;
  _Decimal32 result;
  asm volatile ("mffscdrni %[fpscr], 7\n\t"       /* Set DRN to round-to-prepare-for-shorter, save old DRN.  */
                "drdpq %[d64], %[d128]\n\t"       /* Round v to _Decimal64.  */
                "mffscdrn %[fpscr], %[fpscr]\n\t" /* Restore DRN.  */
                "drsp %[d32], %[d64]\n\t"         /* Round v to _Decimal32.  */
		: [fpscr] "=d" (fpscrv), [d64] "=d" (tmpd), [d32] "=d" (result) , [d128] "+d" (v));
  return result;
#else
  /* For values which are rounded to _Decimal64, the rounding is
   * implementation defined. This uses the current rounding mode.  */
  return v;
#endif
}
hidden_def (__BACKEND_ (FUNC))

#else
# include "base-math/floattitd.c"
#endif
