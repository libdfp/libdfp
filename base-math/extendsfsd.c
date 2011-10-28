/* Handle conversion from binary float (32) to Decimal32

   Copyright (C) 2007, 2008 IBM Corporation.
   Copyright (C) 2008, 2009, 2011 Free Software Foundation, Inc.

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

#ifndef BINARY_TO_DECIMAL
#define BINARY_TO_DECIMAL
#define SRC 32
#define DEST 32
#define NAME extend
#endif

#include "fenv_libdfp.h"
#include "dfpacc.h"
#include "convert.h"
#include <float.h> /* To pick up __FLT_MANT_DIG__  */

#if DEST==32
#define TEMP_TYPE	_Decimal64
#else
#define TEMP_TYPE	_Decimal128
#endif

CONVERT_WRAPPER(
// extendsfsd, extendsfdd, extendsftd
	TEMP_TYPE temp;
	float a_norm;
	long long mant;
	int exp, sexp;

	a_norm = FREXPSF (a, &exp);
	mant = a_norm * 16777216.0;	/* 24 bits of mantissa.  */
	//sexp = exp - 24;		/* Exponent adjusted for mantissa.  */
	sexp = exp - __FLT_MANT_DIG__;	/* Exponent adjusted for mantissa.  */
	temp = mant;
	if (sexp > 0)
		temp *= DECPOWOF2[sexp];
	else if (sexp < 0)
		temp /= DECPOWOF2[-sexp];
	result = temp;
	/* Clear inexact exception raised by DFP arithmetic.  */
	if (DFP_EXCEPTIONS_ENABLED
	    && DFP_TEST_EXCEPTIONS (FE_OVERFLOW|FE_UNDERFLOW) == 0)
	  DFP_CLEAR_EXCEPTIONS (FE_INEXACT);
)
