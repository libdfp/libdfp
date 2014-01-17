/* _Decimal32 isinf classification function.

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2010-2014 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Steven Munroe <munroesj@us.ibm.com>
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

#include <stdio.h>

#ifndef _DECIMAL_SIZE
#  define _DECIMAL_SIZE 32
#endif

#include <math.h>

#define FUNCTION_NAME isinf

#include <dfpmacro.h>

int
INTERNAL_FUNCTION_NAME (DEC_TYPE x)
{
  int cr0;
#if _DECIMAL_SIZE == 32 || _DECIMAL_SIZE == 64
#define DTSTD "dtstdc"
  _Decimal64 input = x;
#elif _DECIMAL_SIZE == 128
# define DTSTD "dtstdcq"
  register _Decimal128 input asm("fr0") = x;
#endif

  asm (DTSTD " cr0,%1,0x04\n"
       "mfcr   %0, 0\n"
       : "=r" (cr0)
       : "f" (input)
       : "cr0");

  /* cr0 bits are 28:31 and:
     - 0010 operand positive with match
     - 1010 operand negative with math  */
  return (cr0 & 0x20000000) ? (cr0 & 0x80000000 ? -1 : 1) : 0;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
