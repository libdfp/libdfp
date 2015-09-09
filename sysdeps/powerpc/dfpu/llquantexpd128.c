/* _Decimal128 llquantexpd128 implementation.

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
   if not, write to the Free Software Foundation, Inc., 59 Temple Place,
   Suite 330, Boston, MA 02111-1307 USA.

   Please see libdfp/COPYING.txt for more information.  */

#include <math.h>
#include <limits.h>
#include <errno.h>

long long int
__llquantexpd128 (_Decimal128 x)
{
  long long int ret;
  register _Decimal128 input asm("fr2") = x;
  _Decimal128 r;
  asm ("dxexq   %0,%1\n"
       "dcffixq %0,%0\n"
    : "=f"(r) :
      "f"(input));
  ret = (long long int)r;
  if ((ret == -1) ||  /* Infinity  */
      (ret == -2) ||  /* NaN  */
      (ret == -3))    /* SNaN  */
    {
      errno = EDOM;
      return LLONG_MIN;
    }
  return ret - 6176;
}
weak_alias (__llquantexpd128, llquantexpd128)
