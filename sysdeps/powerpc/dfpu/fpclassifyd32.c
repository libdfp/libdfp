/* _Decimal32 fpclassify classification function.

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2010-2015 Free Software Foundation, Inc.

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

#include <math.h>
#include <ieee754r_private.h>

int
__fpclassifyd32 (_Decimal32 x)
{ 
  int result = 0;

  /* Since _Decimal32 is promoted to _Decimal64, __DEC32_SUBNORMAL_MIN__ is well
   * within the bounds of a _Decimal64.  This means that we need to do our range
   * check for __DEC32_SUBNORMAL_MIN__ before dropping into the asm code.  This
   * is crude.  The asm code should probably be broken into two parts with this
   * code in between so the zero, <-1 and > 1 checks aren't redundant.  */
  if (x != 0 && x > -1 && x < 1 && x <= 0.000001E-95DF)
    return FP_SUBNORMAL;

  /* Check in order, FP_NORMAL, FP_ZERO, FP_SUBNORMAL, FP_INFINITE,
     FP_NAN.  The most likely case exits early.  */
  asm (
    "dtstdc cr0,%1,0x08;"
    "li %0,4;"
    "beq cr0,1f;"
    "dtstdc cr0,%1,0x20;"
    "li %0,2;"
    "beq cr0,1f;"
    "dtstdc cr0,%1,0x04;"
    "li %0,1;"
    "beq cr0,1f;"
    "li %0,0;"
    "1:;"
    : "=r" (result)
    : "f" ((_Decimal64) x)
    : "cr0");
  return result;
}
hidden_def (__fpclassifyd32)
weak_alias (__fpclassifyd32, fpclassifyd32)
