/* Handle conversion from binary integer (32) to Decimal32

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

#ifndef INTEGER_TO_DECIMAL
#define INTEGER_TO_DECIMAL
#define SRC 32
#define DEST 32
#define NAME float
#endif

#include "dfpacc.h"
#include "convert.h"
#include <stdio.h>

#define BUFMAX 128

DEST_TYPE
PREFIXED_FUNCTION_NAME (SRC_TYPE a)
{
  DEST_TYPE f;
  char buf[BUFMAX];
  decContext context;
  IEEE_DEST_TYPE e;

        ___decContextDefault (&context, CONTEXT_INIT);
        context.round = DEC_ROUND_HALF_EVEN;

  /* Use a C library function to get a floating point string.  */
  sprintf (buf, INT_FMT ".0", CAST_FOR_FMT(a));
  /* Convert from the floating point string to a decimal* type.  */
  /* PASTE(___decimal,PASTE(DEST,FromString))(&f, buf, &context);  */
  PASTE(___decimal,PASTE(DEST,FromString))(&e, buf, &context);
  PASTE(PASTE(___ieee_,DEST),_to_host) (&e, (&f));

        if (context.status != 0) {
          int dec_flags = context.status & 
            (DEC_IEEE_854_Inexact|DEC_IEEE_854_Invalid_operation|DEC_IEEE_854_Overflow);
          DFP_HANDLE_EXCEPTIONS(DFP_IEEE_FLAGS(dec_flags));
        }

  return f;
}
