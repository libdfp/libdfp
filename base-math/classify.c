/* Classification functions for binary floating point types

   Copyright (C) 2007, 2008 IBM Corporation.
   Copyright (C) 2008-2015 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Pete Eberlein <eberlein@us.ibm.com>

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

#include <math.h>
#include <math_private.h>
#include "convert.h"

extern int __dfp_classify_sf (float a);

int __dfp_classify_sf (float a)
{
  uint32_t wx;

  if (a == 0.0f)
    return FP_ZERO;
  GET_FLOAT_WORD (wx, a);
  wx = wx & 0x7fffffff;
  if (wx >= 0x7f800000)
    return (wx > 0x7f800000) ? FP_NAN : FP_INFINITE;
  return FP_NORMAL;
}

extern int __dfp_classify_df (double a);

int __dfp_classify_df (double a)
{
  uint64_t wx;

  if (a == 0.0)
    return FP_ZERO;
  EXTRACT_WORDS64 (wx, a);
  wx = wx & UINT64_C(0x7fffffffffffffff);
  if (wx >= UINT64_C(0x7FF0000000000000))
    return (wx > UINT64_C(0x7FF0000000000000)) ? FP_NAN : FP_INFINITE;
  return FP_NORMAL;
}
