/* Return absolute value of number.

   Copyright (C) 2014 Free Software Foundation, Inc.

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

_Decimal32
__fabsd32 (_Decimal32 x)
{
  /* Both binary32 and decimal32 have the sign bit at same position,
     so the instruction works for both format.  */
  _Decimal64 tmp;
  asm ("dctdp %0, %1\n"  /* DFP Convert To DFP Long  */
       "fabs  %0, %0\n"
       : "=&f"(tmp)
       : "f" (x));
  return (_Decimal32)tmp;
}
hidden_def (__fabsd32)
weak_alias (__fabsd32, fabsd32)
