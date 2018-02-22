/* Handle conversion from Decimal128 to binary float (32)

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
#define SRC 128
#define DEST 32
#define NAME trunc

#include <float.h>
#include <fenv.h>

#include "dfpacc.h"
#include "convert.h"
#include "convert_helpers.h"

CONVERT_WRAPPER(
// trunctdsf
	double temp;
	SRC_TYPE a_norm;
	long long mant;
	int	exp, sexp;

	a_norm = getmantandexpd128 (a, &exp, 15, 1e15DL);
	/* Handle obvious overflow and underflow to avoid going beyond the
	   bounds of the exponent table.  */
	if (exp > 39)		/* Obvious overflow.  */
	  {
	    if (DFP_EXCEPTIONS_ENABLED)
	      DFP_HANDLE_EXCEPTIONS (FE_OVERFLOW|FE_INEXACT);

	    switch (fegetround())
	      {
	        case FE_TOWARDZERO:
	          return SIGNBIT(a) ? -FLT_MAX : FLT_MAX;
	        case FE_DOWNWARD:
	          return SIGNBIT(a) ? -INFINITY :FLT_MAX;
	        case FE_UPWARD:
	          return SIGNBIT(a) ? -FLT_MAX : INFINITY;
	        case FE_TONEAREST:
	        default:
	          return SIGNBIT(a) ? -INFINITY : INFINITY;
	      }
	  }
	else if (exp < -39)	/* Obvious underflow. */
	  {
	    if (DFP_EXCEPTIONS_ENABLED)
	      DFP_HANDLE_EXCEPTIONS (FE_UNDERFLOW|FE_INEXACT);

	    switch (fegetround())
	      {
	        case FE_TONEAREST:
	          mant = llabs(a_norm);
	          if (exp < -45 || ((exp == -45) && (mant >= 700649230000000)))
	            return SIGNBIT(a) ? -0.0 : 0.0;
	          else
	            return SIGNBIT(a) ? -__FLT_DENORM_MIN__ : __FLT_DENORM_MIN__;
	        case FE_DOWNWARD:
	          return SIGNBIT(a) ? -__FLT_DENORM_MIN__ : 0.0;
	        case FE_UPWARD:
	          return SIGNBIT(a) ? -0.0 : __FLT_DENORM_MIN__;
	        case FE_TOWARDZERO:
	        default:
	          return  SIGNBIT(a) ? -0.0 : 0.0;
	      }
	  }

	mant = a_norm;				/* 15 digits of mantissa.  */
	sexp = exp - 15;			/* Exponent adjusted for mantissa.  */
	temp = mant;
	if (sexp > 0)
	  temp *= BINPOWOF10[sexp];
	else if (sexp < 0)
	  temp /= BINPOWOF10[-sexp];
        result = temp;
)
hidden_def (PREFIXED_FUNCTION_NAME)
