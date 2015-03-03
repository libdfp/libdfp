/* Return absolute value of number.

   Copyright (C) 2014-2015 Free Software Foundation, Inc.

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

_Decimal128
__fabsd128 (_Decimal128 x)
{
  /* Half part os decimal128 constainst the sign bit at same position as
     binary64, so the instruction works for both formats.  */
  register _Decimal128 input asm("fr0") = x;
  _Decimal128 ret;
  asm ("fabs  %1, %0\n"
       : "=f"(ret)
       : "f"(input));
  return ret;
}
hidden_def (__fabsd128)
weak_alias (__fabsd128, fabsd128)
