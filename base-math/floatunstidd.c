/* Convert an unsigned 128-bit binary integer into nearest representable
   IEEE754R 64-bit Densely Packed Decimal Floating-point (DFP).

   Copyright (C) 2015 Free Software Foundation, Inc.
   This file is part of the Decimal Floating Point C Library.

   Contributed by Paul E. Murphy (murphyp@linux.vnet.ibm.com)

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

#define FUNC floatunstidd
#define RET_TYPE _Decimal64
#define RET_SIZE 64
#define UNSIGNED
#include "floattitd.c"
