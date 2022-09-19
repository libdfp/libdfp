/* Handle conversion from Decimal32 to binary integer (32)

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
   if not, write to the Free Software Foundation, Inc., 51 Franklin
   Street, Fifth Floor, Boston, MA 02110-1301 USA.

   Please see libdfp/COPYING.txt for more information.  */

#ifndef DECIMAL_TO_INTEGER
#define DECIMAL_TO_INTEGER
#define SRC 32
#define DEST 32
#define NAME fix
#endif

#include "dfpacc.h"
#include "convert.h"
#include <string.h>
/*#include <stdlib.h> */

#define BUFMAX 128

DEST_TYPE
PREFIXED_FUNCTION_NAME (SRC_TYPE a)
{
  /* decNumber's decimal* types have the same format as C's _Decimal*
     types, but they have different calling conventions.  */

  /* TODO: Decimal float to integer conversions should raise FE_INVALID
     if the result value does not fit into the result type.  */

  char buf[BUFMAX];
  char *pos;
  decNumber qval, n1, n2;
  decContext context;

  /* Use a large context to avoid losing precision.  */
  decContextDefault (&context, DEC_INIT_DECIMAL128);
  /* Need non-default rounding mode here.  */
  context.round = DEC_ROUND_DOWN;

  PASTE(decimal,PASTE(SRC,ToNumber))((IEEE_SRC_TYPE*)&a, &n1);
  /* PASTE(decimal,PASTE(SRC,ToNumber))(&a, &n1); */

  /* Rescale if the exponent is less than zero.  */
  decNumberToIntegralValue (&n2, &n1, &context);
  /* Get a value to use for the quantize call.  */
  decNumberFromString (&qval, (char *) "1.0", &context);
  /* Force the exponent to zero.  */
  decNumberQuantize (&n1, &n2, &qval, &context);
  /* Get a string, which at this point will not include an exponent.  */
  decNumberToString (&n1, buf);
  /* Ignore the fractional part.  */
  pos = strchr (buf, '.');
  if (pos)
    *pos = 0;
  /* Use a C library function to convert to the integral type.  */
  return STR_TO_INT (buf, NULL, 10);
}
hidden_def (PREFIXED_FUNCTION_NAME)
