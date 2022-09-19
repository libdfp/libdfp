/* Returns non-zero if the _Decimal64 is non-infinite

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2007-2015 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Joseph Kerian <jkerian@us.ibm.com>

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

#include <math.h>
#include <math_private.h>
#include <ieee754r_private.h>

int
__isfinited64 (_Decimal64 x)
{
  uint64_t hx;
  GET_DEC64_WORD64 (hx, x);

  if (((hx & DEC64_NAN_MASK64) == DEC64_NAN_MASK64) ||
      ((hx & DEC64_INF_MASK64) == DEC64_INF_MASK64))
    return 0;
  return 1;
}
hidden_def (__isfinited64)
weak_alias (__isfinited64, isfinited64)

/* We erroneously published a version of math.h which used 'finite' instead of
 * 'isfinite' and math.h contained a polymorphic 'isfinite()' function which
 * inlined calles to 'finited*' so we've created aliases for compatability.  */
strong_alias (__isfinited64, finited64)
strong_alias (__isfinited64, __finited64)
