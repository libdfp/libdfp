/* Handle conversion from Decimal32 to binary float (32)

   Copyright (C) 2007, 2008 IBM Corporation.
   Copyright (C) 2008, 2009 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Pete Eberlein <eberlein@us.ibm.com>

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

#ifndef DECIMAL_TO_BINARY
#define DECIMAL_TO_BINARY
#define SRC 32
#define DEST 32
#define NAME trunc
#endif

#include "dfpacc.h"
#include "convert.h"

CONVERT_WRAPPER(
// truncsdsf, extendsddf
	double temp;
	SRC_TYPE a_norm;
	long long mant;
	int	exp, sexp;

	a_norm = FREXPD32 (a, &exp);
	mant = a_norm * 1E+7DF;		/* 7 digits of mantissa.  */
	sexp = exp - 7;			/* Exponent adjusted for mantissa.  */
	temp = mant;
	if (sexp > 0)
	  temp *= BINPOWOF10[sexp];
	else if (sexp < 0)
	  temp /= BINPOWOF10[-sexp];
	result = temp;
)
