/* _Decimal64 fpclassify classification function.

   Copyright (C) 2010-2015 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Ryan S. Arnold <rsa@us.ibm.com>

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

int
__fpclassifyd64 (_Decimal64 x)
{ 
  int result = 0;
  /* Check in order, FP_NORMAL, FP_ZERO, FP_SUBNORMAL, FP_INFINITE,
     FP_NAN.  The most likely case exits early.  */
  asm (
    "dtstdc cr0,%1,0x08;"
    "li %0,4;"
    "beq cr0,1f;"
    "dtstdc cr0,%1,0x20;"
    "li %0,2;"
    "beq cr0,1f;"

    "dtstdc cr0,%1,0x10;"
    "li %0,3;"
    "beq cr0,1f;"

    "dtstdc cr0,%1,0x04;"
    "li %0,1;"
    "beq cr0,1f;"
    "li %0,0;"
    "1:;"
    : "=r" (result)
    : "f" (x)
    : "cr0");
  return result;
}
hidden_def (__fpclassifyd64)
weak_alias (__fpclassifyd64, fpclassifyd64)
