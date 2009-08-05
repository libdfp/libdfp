/* Returns a reprentation of NaN in the _Decimal32 format

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2007, 2009 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Joseph Kerian <jkerian@us.ibm.com>

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

#ifndef _DECIMAL_SIZE
#  define _DECIMAL_SIZE 32
   /* needed to pick up DECNUMDIGITS before including decNumber.h  */
#  include <decimal32.h>
#endif

#include <decContext.h>
#include <decNumber.h>
#include <math.h>

#define FUNCTION_NAME nan

#include <dfpmacro.h>

/* `unused' is a GCC extension to disable warnings on intentionally unused
 * parameters.  */
DEC_TYPE
INTERNAL_FUNCTION_NAME (const char *tagp __attribute__ ((unused)))
{
/*  decContext context;
  decNumber dn_result;
  DEC_TYPE result;

  ___decContextDefault(&context, DEFAULT_CONTEXT);
  ___decNumberFromString(&dn_result, x, &context);
  dn_result.bits |= DECNAN;

  FUNC_CONVERT_FROM_DN(&dn_result, &result, &context);
*/
  return (DEC_TYPE)DEC_NAN;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
