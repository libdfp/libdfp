/* Returns non-zero if the _Decimal32 is infinite

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2007, 2009 Free Software Foundation, Inc.

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
   if not, write to the Free Software Foundation, Inc., 59 Temple Place,
   Suite 330, Boston, MA 02111-1307 USA.

   Please see libdfp/COPYING.txt for more information.  */

#ifndef _DECIMAL_SIZE
#  define _DECIMAL_SIZE 32
#  include <decimal32.h>
#endif

#include <math.h>
#include <endian.h>

#define FUNCTION_NAME isinf

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
#if BYTE_ORDER == BIG_ENDIAN
  top_byte = u_conv.bytes[0];
#else
  top_byte = u.conv.bytes[_DECIMAL_SIZE/8 -1];
#endif

  return (top_byte & DECIMAL_Inf) == DECIMAL_Inf;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
