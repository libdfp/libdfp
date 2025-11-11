/*  _Decimal64 copy sign of a number.

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
   if not, write to the Free Software Foundation, Inc., 51 Franklin
   Street, Fifth Floor, Boston, MA 02110-1301 USA.

   Please see libdfp/COPYING.txt for more information.  */

#include <math.h>
#include <ieee754r_private.h>

_Decimal64
__copysignd64 (_Decimal64 x, _Decimal64 y)
{
  /* Both binary64 and decimal64 have the sign bit at same position,
     so the instruction works for both format.  */
  _Decimal64 ret;
  asm ("fcpsgn  %0, %1, %2\n"
       : "=d"(ret)
       : "d"(y), "d"(x));
  return ret;
}
weak_alias (__copysignd64, copysignd64)
