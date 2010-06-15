/* Template for generating Power6 isXXX functions.
   Copyright (C) 2008 IBM Corporation.

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

   Please see dfp/COPYING.txt for more information.  */

#include <math.h>
#include <ieee754r_private.h>

#define PASTE(a,b) PASTE2(a,b)
#define PASTE2(a,b) a##b
#define STRINGIFY(a) STRINGIFY2(a)
#define STRINGIFY2(a) #a

#define DECIMAL_TYPE PASTE(_Decimal,_DECIMAL_SIZE)
#define EXTERNAL_FUNCTION_NAME PASTE(FUNCTION_NAME,PASTE(d,_DECIMAL_SIZE))
#define INTERNAL_FUNCTION_NAME PASTE(__,EXTERNAL_FUNCTION_NAME)

int
INTERNAL_FUNCTION_NAME (DECIMAL_TYPE val)
{
  int result = 0;
#if _DECIMAL_SIZE == 32
  _Decimal64 dummy;
#elif _DECIMAL_SIZE == 128
  register _Decimal128 fr0 asm("fr0") = val;
#endif

  __asm__ (
#if _DECIMAL_SIZE == 32
   "dctdp %1,%2;"
#endif
#if _DECIMAL_SIZE == 128
   "dtstdcq cr0,%1," STRINGIFY(TEST_CLASS_MASK) ";"
#else
   "dtstdc cr0,%1," STRINGIFY(TEST_CLASS_MASK) ";"
#endif
   "li %0,0;"
   "bne cr0,1f;"
   "li %0,1;"
   "bnl cr0,1f;"
   "neg %0,%0;"
 "1:;"
 : "=r" (result)
#if _DECIMAL_SIZE == 32
 , "=f"(dummy)
#endif
#if _DECIMAL_SIZE == 128
 : "f" (fr0)
#else
 : "f" (val)
#endif
 : "cr0");

  return result;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
