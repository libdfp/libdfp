/* Returns non-zero if the _Decimal128 is finite

   Copyright (C) 2008, 2010 IBM Corporation.
   Copyright (C) 2014-2015 Free Software Foundation, Inc.

   Author(s): Pete Eberlein <eberlein@us.ibm.com>
              Ryan S. Arnold <rsa@us.ibm.com>

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
#include <ieee754r_private.h>

#include "dfp_inline.h"

#undef __isfinited128

int
__isfinited128 (_Decimal128 val)
{
  return ___isfinited128 (val);
}
hidden_def (__isfinited128)
weak_alias (__isfinited128, isfinited128)

/* We erroneously published a version of math.h which used 'finite' instead of
 * 'isfinite' and math.h contained a polymorphic 'isfinite()' function which
 * inlined calles to 'finited*' so we've created aliases for compatability.  */
strong_alias (__isfinited128, finited128)
strong_alias (__isfinited128, __finited128)
