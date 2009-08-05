/* Returns the result of integral[0->inf, t**(x-1) * e**(-t) dt]

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
#  include <decimal32.h>
#  define _DECIMAL_SIZE 32
#endif

#include <decContext.h>
#include <decNumber.h>
#include <math.h>
#include <errno.h>

#define FUNCTION_NAME tgamma

#include <dfpmacro.h>

extern DEC_TYPE
FUNC_D (__lgamma_r) (DEC_TYPE, int *);
//extern int
//FUNC_D (__builtin_signbit) (DEC_TYPE);


static DEC_TYPE
IEEE_FUNCTION_NAME (DEC_TYPE x)
{
  DEC_TYPE result, gamma;
  int local_signgam;

  if (x == DFP_CONSTANT(0.0))	/*  Pole error if x== +-0 */
    {
      DFP_EXCEPT (FE_DIVBYZERO);
//      return FUNC_D (__builtin_signbit) (x) ? -DFP_HUGE_VAL : DFP_HUGE_VAL;
      return (x<0) ? -DFP_HUGE_VAL : DFP_HUGE_VAL;
    }
  if (x < DFP_CONSTANT(0.0) && (!FUNC_D (isinf) (x) && FUNC_D (rint) (x) == x) )
    {
      DFP_EXCEPT (FE_INVALID);
      return DFP_NAN;
    }

  gamma = FUNC_D(__lgamma_r) (x,&local_signgam);
  result = local_signgam * FUNC_D(exp) (gamma);

  return result;
}


DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x)
{
  DEC_TYPE z = IEEE_FUNCTION_NAME (x);
#ifndef _IEEE_LIBDFP
  if (_LIB_VERSION == _IEEE_) return z;
  if (!FUNC_D(__finite) (z) && FUNC_D(__finite) (x))
    DFP_ERRNO (ERANGE);
  if (x < DFP_CONSTANT(0.0) && (FUNC_D (isinf) (x) && FUNC_D (rint) (x) == x) )
    DFP_ERRNO (EDOM);
#endif
  return z;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
