/* Returns the type of floating point number from a _Decimal32 type

   Copyright (C) 2008-2014 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

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
   if not, write to the Free Software Foundation, Inc., 59 Temple Place,
   Suite 330, Boston, MA 02111-1307 USA.

   Please see libdfp/COPYING.txt for more information.  */

#include <math.h>
#include <ieee754r_private.h>

int
__fpclassifyd128 (_Decimal128 val)
{
  int result = FP_NAN;
  register _Decimal128 fr0 asm("fr0") = val;

  /* Check in order, FP_NORMAL, FP_ZERO, FP_SUBNORMAL, FP_INFINITE,
     FP_NAN. The thought is the most likely case exits early. */
  asm (
   "dtstdcq cr0,%1,0x08;"
   "li %0,4;"
   "beq cr0,1f;"
   "dtstdcq cr0,%1,0x20;"
   "li %0,2;"
   "beq cr0,1f;"
   "dtstdcq cr0,%1,0x10;"
   "li %0,3;"
   "beq cr0,1f;"
   "dtstdcq cr0,%1,0x04;"
   "li %0,1;"
   "beq cr0,1f;"
   "li %0,0;"
   "1:;"
   : "=r" (result)
   : "f" (fr0) : "cr0");

  return result;
}
hidden_def (__fpclassifyd128)
weak_alias (__fpclassifyd128, fpclassifyd128)
