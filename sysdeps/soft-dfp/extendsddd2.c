/* Handle conversion from Decimal32 to Decimal64

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

#ifndef DECIMAL_TO_DECIMAL
#define DECIMAL_TO_DECIMAL
#define SRC 32
#define DEST 64
#define NAME extend
#endif

#include "mapround.h"
#include "dfpacc.h"
#include "convert.h"

DEST_TYPE
PREFIXED_FUNCTION_NAME (SRC_TYPE a)
{
	DEST_TYPE result;
	decNumber d;
	decContext context;
	
	decContextDefault(&context, CONTEXT_INIT);
	context.round = __dn_getround();
	
	PASTE(decimal,PASTE(SRC,ToNumber))((IEEE_SRC_TYPE*)&a, &d);
	/* PASTE(___decimal,PASTE(SRC,ToNumber))(&a, &d); */
	/* PASTE(___decimal,PASTE(DEST,FromNumber))(&result, &d, &context);  */
	PASTE(decimal,PASTE(DEST,FromNumber))((IEEE_DEST_TYPE*)&result, &d, &context);

	if (context.status != 0) {
	  int dec_flags = context.status & (DEC_IEEE_854_Inexact|DEC_IEEE_854_Invalid_operation);
	  DFP_HANDLE_EXCEPTIONS(DFP_IEEE_FLAGS(dec_flags));
	}

	return result;
}
hidden_def (PREFIXED_FUNCTION_NAME)
