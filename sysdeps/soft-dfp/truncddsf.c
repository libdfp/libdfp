/* Handle conversion from Decimal64 to binary float (32)

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

#define DECIMAL_TO_BINARY
#define SRC 64
#define DEST 32
#define NAME trunc

#include "dfpacc.h"
#include "convert.h"

CONVERT_WRAPPER(
// truncddsf
	double temp;
	_Decimal64 a_norm;
	long long mant;
	int exp, sexp;
	
	a_norm = FREXPD64 (a, &exp);
	
	/* Check for values that would overflow the exponent table, which
	   would be obvious overflow and underflow.  */
	if (exp > 39)		/* Obvious overflow.  */
	  {
	    if (DFP_EXCEPTIONS_ENABLED)
	      DFP_HANDLE_EXCEPTIONS (FE_OVERFLOW|FE_INEXACT);
	    return SIGNBIT(a) ? -INFINITY : INFINITY;
	  }
	else if (exp < -39)	/* Obvious underflow.  */
	  {
	    if (DFP_EXCEPTIONS_ENABLED)
	      DFP_HANDLE_EXCEPTIONS (FE_UNDERFLOW|FE_INEXACT);
	    return SIGNBIT(a) ? -0.0 : 0.0;
	  }

	mant = a_norm * 1.E+16DD;	/* 16 digits of mantissa.  */
	sexp = exp - 16;		/* Exponent adjusted for mantissa.  */
	temp = mant;
	if (sexp > 0)
	  temp *= BINPOWOF10[sexp];
	else if (sexp < 0)
	  temp /= BINPOWOF10[-sexp];
	result = temp;
)
