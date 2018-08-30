/* Handle conversion from Decimal64 to binary long double (128)

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

#ifndef DECIMAL_TO_BINARY
#define DECIMAL_TO_BINARY
#define SRC 64
#define DEST 128
#define NAME extend
#endif

#include <float.h>

#include "dfpacc.h"
#include "convert.h"
#include "convert_helpers.h"

CONVERT_WRAPPER(
// extendddtf, trunctdtf
	volatile double df_part1, df_part2;
	int exp;
	SRC_TYPE dd_part1, dd_part2;

	/* Get normalized exponent.  */
	(void) getmantandexpd128 (a, &exp, 0, 0);

	/* Check if the normalized exponent is beyond the greatest or the least
	   exponent supported by IBM long double and return the result properly
	   rounded.  */
	if (exp > BINPOWOF10_LIMIT) /* Overflow.  */
	  {
	    if (DFP_EXCEPTIONS_ENABLED)
	      DFP_HANDLE_EXCEPTIONS (FE_OVERFLOW|FE_INEXACT);

	    switch (fegetround())
	      {
	        case FE_TOWARDZERO:
	          return SIGNBIT(a) ? -LDBL_MAX : LDBL_MAX;
	        case FE_DOWNWARD:
	          return SIGNBIT(a) ? -INFINITY : LDBL_MAX;
	        case FE_UPWARD:
	          return SIGNBIT(a) ? -LDBL_MAX : INFINITY;
	        case FE_TONEAREST:
	        default:
	          return SIGNBIT(a) ? -INFINITY : INFINITY;
	      }
	  }
	else if (exp < -POWOF10_MIN_DENORM_DBL_EXP) /* Underflow.  */
	  {
	    if (DFP_EXCEPTIONS_ENABLED)
	      DFP_HANDLE_EXCEPTIONS (FE_UNDERFLOW|FE_INEXACT);

	    switch (fegetround())
	      {
	        case FE_TOWARDZERO:
	          return SIGNBIT(a) ? -0.0 : 0.0;
	        case FE_DOWNWARD:
	          return SIGNBIT(a) ? -__LDBL_DENORM_MIN__ : 0.0;
	        case FE_UPWARD:
	          return SIGNBIT(a) ? -0.0 : __LDBL_DENORM_MIN__;
	        case FE_TONEAREST:
	        default:
	          return SIGNBIT(a) ? -0.0 : 0.0;
	      }
	  }

	df_part1 = a;			/* TD -> DF  */
	dd_part1 = df_part1;		/* DF -> DD./TD.  */
	dd_part2 = a - dd_part1;	/* DD./TD.  */
	df_part2 = dd_part2;		/* DD/TD -> DF.  */
	result = df_part1;
	result += df_part2;
	/* Clear inexact exception raised by DFP arithmetic.  */
	if (DFP_EXCEPTIONS_ENABLED
	    && DFP_TEST_EXCEPTIONS (FE_OVERFLOW|FE_UNDERFLOW) == 0)
	  DFP_CLEAR_EXCEPTIONS (FE_INEXACT);
)
hidden_def (PREFIXED_FUNCTION_NAME)
