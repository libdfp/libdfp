/* Returns the unbiased exponent of the passed _Decimal32 value as an long int.

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

#define _RETURN_TYPE   long int
#define FUNCTION_NAME  llogb
#define _MAX_VALUE     LONG_MAX
#define _MIN_VALUE     LONG_MIN
#define _FBLOG0        FP_LLOGB0
#define _FBLOGNAN      FP_LLOGBNAN

#include "ilogbd32.c"
