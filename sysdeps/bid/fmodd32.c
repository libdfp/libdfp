/* Returns the remainder of a floating point division

   This implementation is based off that presented in
   "The Mathematical-Function Computation Handbook" by
   Nelson H.F. Beebe.

   decnumber is used here to workaround the limitations
   of the BID format related to ldexp/frexp.  ldexp/frexp
   were incredibly slow on BID when the pure decimal version
   was written.

   Copyright (C) 2024 Free Software Foundation, Inc.

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

#ifndef _DECIMAL_SIZE
#include <decimal32.h>
#define _DECIMAL_SIZE 32
#endif

#include <decContext.h>
#include <decNumber.h>
#include <math.h>
#include <errno.h>
#include <ieee754r_private.h>

#define FUNCTION_NAME fmod

#include <dfpmacro.h>

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x, DEC_TYPE y)
{
  DEC_TYPE dresult;
  decNumber dn_x;
  decNumber dn_y;
  decNumber result;

  /* The remainder may contain 1 extra digit. Add extra room for at least 1 more. */
  struct
  {
    decNumber d;
    decNumberUnit extra;
  } r, yabs, xabs, cmp, yabs_scaled;
  int ny;
  decContext ctxt;

  decContextDefault (&ctxt, DEFAULT_CONTEXT);
  decNumberZero (&result);
  FUNC_CONVERT_TO_DN (&x, &dn_x);
  FUNC_CONVERT_TO_DN (&y, &dn_y);

  if (decNumberIsSpecial (&dn_x) || decNumberIsSpecial (&dn_y)
      || decNumberIsZero (&dn_y))
    {
      result.bits = DECNAN;
      DFP_ERRNO (EDOM);
      goto done;
    }

  decNumberAbs (&xabs.d, &dn_x, &ctxt);
  decNumberAbs (&yabs.d, &dn_y, &ctxt);

  decNumberCompare (&result, &xabs.d, &yabs.d, &ctxt);

  if (decNumberIsZero (&result))
    {
      decNumberCopySign (&result, &result, &dn_x);
      goto done;
    }
  else if (result.bits & DECNEG)
    {
      decNumberCopy (&result, &dn_x);
      goto done;
    }

  /* All special cases accounted for, find the remainder.  */
  decNumberCopy (&r.d, &xabs.d);
  decNumberCopy (&yabs_scaled.d, &yabs.d);
  ny = yabs.d.exponent + yabs.d.digits - 1;

  /* Expand the precision to while computing the remainder.  */
  ctxt.digits++;

  decNumberCompare (&cmp.d, &r.d, &yabs.d, &ctxt);
  while (!(cmp.d.bits & DECNEG))	/* r >= yabs */
    {
      int n = (r.d.exponent + r.d.digits - 1) - ny;
      /* Rescale yabs to the nearest power of 10 less than r.  */
      yabs_scaled.d.exponent = yabs.d.exponent + n;
      decNumberCompare (&cmp.d, &yabs_scaled.d, &r.d, &ctxt);
      if (!decNumberIsZero (&cmp.d) && !(cmp.d.bits & DECNEG))
	yabs_scaled.d.exponent--;

      decNumberSubtract (&r.d, &r.d, &yabs_scaled.d, &ctxt);	/* r = r - yabs_scaled */
      decNumberCompare (&cmp.d, &r.d, &yabs.d, &ctxt);	/* cmp = r ? yabs */
    }

  ctxt.digits--;
  decNumberNormalize (&result, &r.d, &ctxt);
  if (dn_x.bits & DECNEG)
    result.bits |= DECNEG;


done:
  FUNC_CONVERT_FROM_DN (&result, &dresult, &ctxt);

  return dresult;
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
