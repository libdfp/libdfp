/* Decimal Floating Point function intended to override the libgcc version.

   Copyright (C) 2008-2016 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Andreas Krebbel  <Andreas.Krebbel@de.ibm.com>

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

#include <dfpacc.h>

CMPINT
__BACKEND_(netd2) (_Decimal128 x, _Decimal128 y)
{
  return x != y;
}
hidden_def (__BACKEND_(netd2))
