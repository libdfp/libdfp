/* Returns non-zero if the _Decimal32 is normalized

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
#  include <decimal32.h>
#endif

#include <decNumber.h>
#include <math.h>

#include <float.h>	/* Includes definitions of DEC##_MIN */

#define FUNCTION_NAME isnormal

#include <dfpmacro.h>
#include <ieee754r_private.h>

int
INTERNAL_FUNCTION_NAME (DEC_TYPE x)
{
  DEC_TYPE min = DFP_MIN;
  DEC_TYPE result;
  /*decNumber dn_min; */
  decContext context;
  decNumber dn_x;
  decNumber dn_result;


/*  FUNC_CONVERT_TO_DN(&min, &dn_min); */
  FUNC_CONVERT_TO_DN(&x, &dn_x);

  if(___decNumberIsNaN(&dn_x) || ___decNumberIsInfinite(&dn_x) ||
	___decNumberIsZero(&dn_x))
    return 0;

  ___decContextDefault(&context, DEFAULT_CONTEXT);
  ___decNumberAbs(&dn_result, &dn_x, &context);

  FUNC_CONVERT_FROM_DN (&dn_result, &result, &context);
  if(result<min)
    return 0;

 /* if(___decCompare(&dn_result, &dn_min) == -1)
    return 0; */

  return 1;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
