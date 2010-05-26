/* Set the exponent of x to the exp of y, trying to preserve the value of x

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2010 Free Software Foundation, Inc.

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

#ifndef _DECIMAL_SIZE
#  define _DECIMAL_SIZE 32
#endif

#include <math.h>

#define FUNCTION_NAME quantize

#include <dfpmacro.h>

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x, DEC_TYPE y)
{
#if _DECIMAL_SIZE == 128
  _Decimal128 result;

  __asm__("dquaq %0,%1,%2,3;"
	: "=f" (result): "f" ((_Decimal128) y), "f" ((_Decimal128) x));
#else
  /* Always use _Decimal64 asm insns for _Decimal32 since there are no
   * _Decimal32 insns.  */
  _Decimal64 result;

  __asm__("dqua %0,%1,%2,3;"
	: "=f" (result): "f" ((_Decimal64) y), "f" ((_Decimal64) x));
#endif
  return (DEC_TYPE) result;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
