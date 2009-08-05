/* Handle conversion from binary long double (128) to Decimal32

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

#ifndef BINARY_TO_DECIMAL
#define BINARY_TO_DECIMAL
#define SRC 128
#define DEST 32
#define NAME trunc

extern _Decimal32 __truncdfsd(double);

#endif

#include "dfpacc.h"
#include "convert.h"

CONVERT_WRAPPER(
// truncdfsd, extenddfdd, extenddftd
	DEST_TYPE temp;
	union {
		SRC_TYPE ld;
		double d[2];
	} ldd;

	ldd.ld = a;
	temp = ldd.d[0];
	result = temp;
	temp = ldd.d[1];
	result += temp;
	/* Clear inexact exception raised by DFP arithmetic.  */
	if (DFP_EXCEPTIONS_ENABLED
	    && DFP_TEST_EXCEPTIONS (FE_OVERFLOW|FE_UNDERFLOW) == 0)
	  DFP_CLEAR_EXCEPTIONS (FE_INEXACT);
)
