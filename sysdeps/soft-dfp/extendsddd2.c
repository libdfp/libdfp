/* Handle conversion from Decimal32 to Decimal64

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
	IEEE_SRC_TYPE e;
	IEEE_DEST_TYPE r;
	
	___decContextDefault(&context, CONTEXT_INIT);
	context.round = __dn_getround();
	
	PASTE(___host_to_ieee_,SRC) ((&a), &e);
	PASTE(___decimal,PASTE(SRC,ToNumber))(&e, &d);
	/* PASTE(___decimal,PASTE(SRC,ToNumber))(&a, &d); */
	/* PASTE(___decimal,PASTE(DEST,FromNumber))(&result, &d, &context);  */
	PASTE(___decimal,PASTE(DEST,FromNumber))(&r, &d, &context);
	PASTE(PASTE(___ieee_,DEST),_to_host) (&r, (&result));

	if (context.status != 0) {
	  int dec_flags = context.status & (DEC_IEEE_854_Inexact|DEC_IEEE_854_Invalid_operation);
	  DFP_HANDLE_EXCEPTIONS(DFP_IEEE_FLAGS(dec_flags));
	}

	return result;
}
