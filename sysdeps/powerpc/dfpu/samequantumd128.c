/* _Decimal128 quantum implementation.

   Copyright (C) 2014-2015 Free Software Foundation, Inc.

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
   if not, write to the Free Software Foundation, Inc., 51 Franklin
   Street, Fifth Floor, Boston, MA 02110-1301 USA.

   Please see libdfp/COPYING.txt for more information.  */

#include <math.h>
#include <ieee754r_private.h>

/* Return the quantum 1 * 10^exponent(x)  */
_Bool __samequantumd128 (_Decimal128 x, _Decimal128 y)
{
  int xnan = __isnand128 (x);
  int ynan = __isnand128 (y);

  if (xnan | ynan)
    return xnan & ynan;

  return __quantumd128 (x) == __quantumd128 (y);
}
weak_alias (__samequantumd128, samequantumd128)
