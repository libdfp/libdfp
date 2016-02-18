/* Handle conversion from Decimal32 to binary long double (128)

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
   if not, write to the Free Software Foundation, Inc., 59 Temple Place,
   Suite 330, Boston, MA 02111-1307 USA.

   Please see libdfp/COPYING.txt for more information.  */

#ifndef DECIMAL_TO_BINARY
# define DECIMAL_TO_BINARY
# define SRC 32
# define DEST 128
# define NAME extend
# include "decimal32.h"
# include <dfptypeconv32.h>
#endif

#include "dfpacc.h"
#include "convert.h"
#include <stdio.h>

#define CONTEXT_INIT PASTE(DEC_INIT_DECIMAL,DEST)

#define BUFMAX 128

DEST_TYPE
PREFIXED_FUNCTION_NAME (SRC_TYPE a)
{
  /* decNumber's decimal* types have the same format as C's _Decimal*
     types, but they have different calling conventions.  */
  char buf[BUFMAX];
  decNumber n;
  decContext context;
  IEEE_SRC_TYPE e;

  decContextDefault (&context, CONTEXT_INIT);

  PASTE(___host_to_ieee_,SRC) ((&a), &e);
  PASTE(decimal,PASTE(SRC,ToNumber))(&e, &n);

  decNumberToString (&n, buf);

  /* Use a C library function to convert to the integral type.  */
  return strtold (buf, NULL);
}
hidden_def (PREFIXED_FUNCTION_NAME)
