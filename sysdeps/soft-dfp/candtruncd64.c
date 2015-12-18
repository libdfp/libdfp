/* Take 3 _Decimal128 numbers spanning 39 digits and
   convert them into one properly rounding _Decimal64 value.

   This is meant to be used internally to assist in rounding
   __int128 values to _Decimal32/64.

   Copyright (C) 2015 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Paul E. Murphy <murphyp@linux.vnet.ibm.com>

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

/* This will take at most 39 digits.  This should always be 39
   for this function.  It must defined before including this
   file as decimal*.h will attempt to define it.  */
#define DECNUMDIGITS 39

#define _DECIMAL_SIZE 64
#include <decimal64.h>
#define FUNC_NAME combine_and_truncd64

#include "candtruncd32.c"
