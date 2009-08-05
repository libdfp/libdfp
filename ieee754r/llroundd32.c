/* Rounds to the nearest long long int value, ignoring rounding mode

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

#include <decContext.h>
#include <decNumber.h>
#include <math.h>
#include <errno.h>
#include <limits.h>

#include <fenv.h>

#ifndef FUNCTION_NAME
# define FUNCTION_NAME llround
#endif
#include <dfpmacro.h>

#ifndef __ROUND_RETURN_TYPE
# define __ROUND_RETURN_TYPE long long int
# define __MIN_VALUE	LLONG_MIN
# define __MAX_VALUE	LLONG_MAX
#endif

#ifndef __ROUND_MODE
# define __ROUND_MODE DEC_ROUND_HALF_UP
#endif

static __ROUND_RETURN_TYPE
IEEE_FUNCTION_NAME (DEC_TYPE x)
{
  DEC_TYPE result;
  decContext context;
  decNumber dn_result;
  decNumber dn_x;

  FUNC_CONVERT_TO_DN (&x, &dn_x);
  if (___decNumberIsNaN (&dn_x) || ___decNumberIsInfinite (&dn_x)
	|| x > __MAX_VALUE || x < __MIN_VALUE)
    {
      DFP_EXCEPT (FE_INVALID);
      return (__ROUND_RETURN_TYPE) x;
    }

  ___decContextDefault (&context, DEFAULT_CONTEXT);
  context.round = __ROUND_MODE;
  ___decNumberToIntegralValue (&dn_result,&dn_x,&context);

  FUNC_CONVERT_FROM_DN(&dn_result, &result, &context);
  /* Use _Decimal* to __ROUND_RETURN_TYPE casting.  */
  return (__ROUND_RETURN_TYPE)result;

/*  return (__ROUND_RETURN_TYPE)___decNumberToInteger (&dn_result); */
}

__ROUND_RETURN_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x)
{
  __ROUND_RETURN_TYPE z = IEEE_FUNCTION_NAME (x);
#ifndef _IEEE_LIBDFP
  if (_LIB_VERSION == _IEEE_) return z;
  if (FUNC_D(isnan) (x) || FUNC_D(isinf) (x)
	|| x > __MAX_VALUE || x < __MIN_VALUE)
    DFP_ERRNO (EDOM);
#endif
  return z;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
