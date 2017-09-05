/* Handle conversion from Decimal64 to binary double (64)

   Copyright (C) 2007, 2008 IBM Corporation.
   Copyright (C) 2008-2015 Free Software Foundation, Inc.

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
#define DEST 64
#define NAME trunc

#include <float.h>
#include <fenv.h>

#include "dfpacc.h"
#include "convert.h"
#include "convert_helpers.h"

CONVERT_WRAPPER(
// truncdddf
	long double temp;
	SRC_TYPE a_norm;
	long long mant;
	int	exp, sexp;

	a_norm = getmantandexpd64 (a, &exp, 16, 1e16DD);

	/* Avoid going beyond the bounds of the table, which would also
	   mean an overflow or underflow.  */
	/* The +1 is necessary because of the one digit after the decimal
	   point. */
	if (exp > BINPOWOF10_LIMIT + 1)		/* Obvious overflow.  */
	  {
	    if (DFP_EXCEPTIONS_ENABLED)
	      DFP_HANDLE_EXCEPTIONS (FE_OVERFLOW|FE_INEXACT);

	    switch (fegetround())
	      {
	        case FE_TOWARDZERO:
	          return SIGNBIT(a) ? -DBL_MAX : DBL_MAX;
	        case FE_DOWNWARD:
	          return SIGNBIT(a) ? -INFINITY : DBL_MAX;
	        case FE_UPWARD:
	          return SIGNBIT(a) ? -DBL_MAX : INFINITY;
	        case FE_TONEAREST:
	        default:
	          return SIGNBIT(a) ? -INFINITY : INFINITY;
	      }
	  }
	else if (exp < -BINPOWOF10_LIMIT)	/* Obvious underflow.  */
	  {
	    if (DFP_EXCEPTIONS_ENABLED)
	      DFP_HANDLE_EXCEPTIONS (FE_UNDERFLOW|FE_INEXACT);
	    return SIGNBIT(a) ? -0.0 : 0.0;
	  }

	mant = a_norm;			/* 16 digits of mantissa.  */
	sexp = exp - 16;		/* Exponent adjusted for mantissa.  */
	temp = mant;			/* mantissa as TF */
	if (sexp > 0)
	  result = temp * BINPOWOF10[sexp];
	else if (sexp < 0)
	  {
	    /* Avoid going beyond the bounds of the exponent table for
	       negative exponents.  */
	    if (sexp < -BINPOWOF10_LIMIT)
	      {
	        temp /= BINPOWOF10[BINPOWOF10_LIMIT];
	        sexp += BINPOWOF10_LIMIT;
	      }
	    result = temp / BINPOWOF10[-sexp];
	  }
	else
	  result = temp;
	/* Clear inexact exception raised by DFP arithmetic.  */
	if (DFP_EXCEPTIONS_ENABLED
	    && DFP_TEST_EXCEPTIONS (FE_OVERFLOW|FE_UNDERFLOW) == 0)
	  DFP_CLEAR_EXCEPTIONS (FE_INEXACT);
)
hidden_def (PREFIXED_FUNCTION_NAME)
