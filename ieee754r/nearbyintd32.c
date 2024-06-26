/* Round to nearest integer _Decimal32 form

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2007-2016 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Ryan S. Arnold <rsa@us.ibm.com>

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

#ifndef _DECIMAL_SIZE
# define _DECIMAL_SIZE 32
# include <decimal32.h>
#endif

#include <decContext.h>
#include <decNumber.h>
#include <math.h>
#include <errno.h>
#include <ieee754r_private.h>

#include <fenv.h>
#include <mapround.h>

#define FUNCTION_NAME nearbyint
#include <dfpmacro.h>

static DEC_TYPE
IEEE_FUNCTION_NAME (DEC_TYPE x)
{
  decNumber dn_x,dn_result;
  decContext context;
  DEC_TYPE result;

  FUNC_CONVERT_TO_DN (&x, &dn_x);
  decContextDefault (&context,DEFAULT_CONTEXT);

  if (decNumberIsNaN (&dn_x) || decNumberIsInfinite (&dn_x) ||
	decNumberIsZero (&dn_x))
	return x+x;

  context.round = __dn_getround();
  decNumberToIntegralValue (&dn_result,&dn_x,&context);

  FUNC_CONVERT_FROM_DN (&dn_result, &result, &context);
  if (context.status & DEC_Overflow)
    DFP_EXCEPT (FE_OVERFLOW);

  return result;
}

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x)
{
  DEC_TYPE z = IEEE_FUNCTION_NAME (x);
  if (!FUNC_D(__isfinite) (z) && FUNC_D(__isfinite) (x))
    DFP_ERRNO (ERANGE);
  return z;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
