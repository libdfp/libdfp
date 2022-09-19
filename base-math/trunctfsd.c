/* Handle conversion from binary long double (128) to Decimal32

   Copyright (C) 2016 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   The Decimal Floating Point C Library is free software; you can
   redistribute it and/or modify it under the terms of the GNU Lesser
   General Public License version 2.1.

   The Decimal Floating Point C Library is distributed in the hope that
   it will be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
   the GNU Lesser General Public License version 2.1 for more details.

   You should have received a copy of the GNU Lesser General Public
   License version 2.1 along with the Decimal Floating Point C Library;
   if not, write to the Free Software Foundation, Inc., 51 Franklin
   Street, Fifth Floor, Boston, MA 02110-1301 USA.

   Please see libdfp/COPYING.txt for more information.  */

#ifndef BINARY_TO_DECIMAL
# define BINARY_TO_DECIMAL
# define SRC 128
# define DEST 32
# define NAME trunc
# include "decimal32.h"
#endif

#include "dfpacc.h"
#include "convert.h"
#include <stdio.h>

#define CONTEXT_INIT PASTE(DEC_INIT_DECIMAL,DEST)

#define BUFMAX 128

DEST_TYPE
PREFIXED_FUNCTION_NAME (SRC_TYPE a)
{
  DEST_TYPE f;
  char buf[BUFMAX];
  decContext context;

  snprintf (buf, BUFMAX, "%.36Le", a);

  decContextDefault (&context, CONTEXT_INIT);
  PASTE(decimal,PASTE(DEST,FromString))((IEEE_DEST_TYPE*)&f, buf, &context);

  if (context.status != 0) {
    int dec_flags = context.status & 
    (DEC_IEEE_854_Inexact|DEC_IEEE_854_Invalid_operation|DEC_IEEE_854_Overflow);
    DFP_HANDLE_EXCEPTIONS(DFP_IEEE_FLAGS(dec_flags));
  }

  return f;
}
hidden_def (PREFIXED_FUNCTION_NAME)
