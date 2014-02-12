/* _Decimal64 quantum implementation.

   Copyright (C) 2014 Free Software Foundation, Inc.

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
#include <ieee754r_private.h>

/* Return the quantum 1 * 10^exponent(x)  */
_Bool __samequantumd64 (_Decimal64 x, _Decimal64 y)
{
  int xnan = __isnand64 (x);
  int ynan = __isnand64 (y);

  if (xnan | ynan)
    return xnan & ynan;

  return __quantumd64 (x) == __quantumd64 (y);
}
weak_alias (__samequantumd64, samequantumd64)
