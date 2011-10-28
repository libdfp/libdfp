/* Handle conversion from binary double (64) to Decimal32

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
#define SRC 64
#define DEST 32
#define NAME trunc
#endif

#include "dfpacc.h"
#include "convert.h"
#include <float.h> /* To pick up __DBL_MANT_DIG__  */

CONVERT_WRAPPER(
// truncdfsd, extenddfdd, extenddftd
	_Decimal128 temp; /* Needs to be big enough so that temp = mant doesn't round.  */
	double a_norm;
	long long mant;
	int exp, sexp;

	a_norm = FREXPDF (a, &exp);
	mant = a_norm * 9007199254740992.0;	/* 53 bits of mantissa.  */
	//sexp = exp - 53;			/* Exponent adjusted for mantissa.  */
	sexp = exp - __DBL_MANT_DIG__;		/* Exponent adjusted for mantissa.  */
	temp = mant;				/* DI -> TD.  */
	if (sexp > 0)
		temp *= DECPOWOF2[sexp];
	else if (sexp < 0)
		temp /= DECPOWOF2[-sexp];
	result = (DEST_TYPE)temp; /* Cast to the resultant type.  */
	/* Clear inexact exception raised by DFP arithmetic.  */
	if (DFP_EXCEPTIONS_ENABLED
	    && DFP_TEST_EXCEPTIONS (FE_OVERFLOW|FE_UNDERFLOW) == 0)
	  DFP_CLEAR_EXCEPTIONS (FE_INEXACT);
)
