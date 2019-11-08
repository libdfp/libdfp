/* Convert string representing a number to Decimal Float value, using given locale.

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2007-2015 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Joseph Kerian <jkerian@us.ibm.com>

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

#ifndef FLOAT
#define FLOAT		_Decimal128
#define FLOAT_HUGE_VAL	DEC_INFINITY
#define FLT		DEC128
#define FLOAT_SIZE	128
#define FLOAT_ZERO	0.DL
#define SET_MANTISSA(x,y)
#define PRINTF_SPEC "%DDf"
#define __DEC_MANT_DIG__ __DEC128_MANT_DIG__
#define __DEC_MAX_EXP__ __DEC128_MAX_EXP__
#define __DEC_MIN_EXP__ __DEC128_MIN_EXP__
#endif

#include "strtod32.c"
