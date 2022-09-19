/* Calculate the absolute value for a _Decimal32 type

   Copyright (C) 2007-2020 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Joseph Kerian <jkerian@us.ibm.com>, Pedro Caldeira <caldeira@linux.ibm.com>

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

#ifndef _DECIMAL_SIZE
#  define _DECIMAL_SIZE 32
#  include <decimal32.h>
#endif

#include <math.h>
#include <ieee754r_private.h>

#define FUNCTION_NAME fabs

#include <dfpmacro.h>

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x)
{
  DEC_TYPE result = x;
  unsigned char *p = (unsigned char*) &result;
  #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    p += sizeof(DEC_TYPE)-1;
  #endif
  *p = *p & 0x7Fu;
  return result; 
}
hidden_def (INTERNAL_FUNCTION_NAME)
weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
