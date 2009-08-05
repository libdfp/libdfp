/* Handle conversion from Decimal64 to binary long double (128)

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
#define SRC 64
#define DEST 128
#define NAME extend

extern double __truncdddf (_Decimal64);
extern _Decimal64 __extenddfdd (double);

#endif

#include "dfpacc.h"
#include "convert.h"

CONVERT_WRAPPER(
// extendddtf, trunctdtf
	double df_part1, df_part2;
	SRC_TYPE dd_part1, dd_part2;

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
