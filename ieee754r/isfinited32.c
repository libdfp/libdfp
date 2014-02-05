/* Returns non-zero if the _Decimal32 is non-infinite

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2007-2014 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Joseph Kerian <jkerian@us.ibm.com>
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
   if not, write to the Free Software Foundation, Inc., 59 Temple Place,
   Suite 330, Boston, MA 02111-1307 USA.

   Please see libdfp/COPYING.txt for more information.  */

#ifndef _DECIMAL_SIZE
#  define _DECIMAL_SIZE 32
   /* needed to pick up DECNUMDIGITS before including decNumber.h  */
#  include <decimal32.h>
#endif

#include <math.h>
#include <endian.h>
#include <decNumber.h>

#define FUNCTION_NAME isfinite

#include <dfpmacro.h>

int
INTERNAL_FUNCTION_NAME (DEC_TYPE x)
{
  uint8_t top_byte;
  union
  {
    DEC_TYPE dec;
    uint8_t bytes[_DECIMAL_SIZE/8];
  } u_conv;

  u_conv.dec = x;
#if __BYTE_ORDER == __BIG_ENDIAN
  top_byte = u_conv.bytes[0];
#else
  top_byte = u_conv.bytes[_DECIMAL_SIZE/8 - 1];
#endif

  if (((top_byte & DECIMAL_NaN) == DECIMAL_NaN) ||
      ((top_byte & DECIMAL_Inf) == DECIMAL_Inf))
    return 0;
  return 1;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)

/* We erroneously published a version of math.h which used 'finite' instead of
 * 'isfinite' and math.h contained a polymorphic 'isfinite()' function which
 * inlined calles to 'finited*' so we've created aliases for compatability.  */
strong_alias (INTERNAL_FUNCTION_NAME,FUNC_D(finite))
strong_alias (INTERNAL_FUNCTION_NAME,FUNC_D(__finite))
