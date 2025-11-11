/* fix*ti implementation for powerpc64.

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

#ifndef SRC_SIZE
#  define SRC_SIZE 32
#  define FUNC fixsdti
#  define SHIFT_EXP 7
#  define DSUFF DF
#  define SIGNED 1
#endif

/* For Power10 and newer, use an optimized routine for signed 128b integers.  */
#if defined (_ARCH_PWR10) && SIGNED == 1

# define _DECIMAL_SIZE SRC_SIZE
# include <dfpacc.h>
# include <dfpmacro.h>

INT128 __BACKEND_(FUNC) (DEC_TYPE a)
{
  _Decimal128 v = a;
  union v128i128 {
    vector __int128 v128;
    __int128 i128;
  } r;
  asm ( "drintnq 0, %0, %0, 1\n\t" : "+d" (v) );
  asm ( "dctfixqq %[e],%[c]\n\t"    : [e] "=v" (r.v128) : [c] "d" (v) );
  return r.i128;
}
hidden_def (__BACKEND_(FUNC))


#else
# include "base-math/fixsdti.c"
#endif
