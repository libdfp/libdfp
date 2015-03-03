/* Set the exponent of x to the exp of y, trying to preserve the value of x

   Copyright (C) 2010-2015 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

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

_Decimal128
__quantized128(_Decimal128 x, _Decimal128 y)
{

  _Decimal128 result;

  asm ("dquaq %0,%1,%2,3\n\t"
	: "=f" (result)
        : "f" ((_Decimal128) y), "f" ((_Decimal128) x));
  return result;

}
weak_alias (__quantized128, quantized128)
