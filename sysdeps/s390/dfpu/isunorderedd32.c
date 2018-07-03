/* Returns true if either _Decimal32 is NaN

   Copyright (C) 2018 Free Software Foundation, Inc.

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
# define _DECIMAL_SIZE 32
#endif

#include <math.h>

#ifndef FUNCTION_NAME
# define FUNCTION_NAME isunordered
#endif
#include <dfpmacro.h>

int
INTERNAL_FUNCTION_NAME (DEC_TYPE x, DEC_TYPE y)
{
  /* GCC on s390x is omitting the call of isnandXY functions.
     Instead it emits the corresponding instructions.
     Note: If GCC emits function calls, an extra plt reference will be added
     and the testcase check-localplt will fail.  */
  return FUNC_D(isnan) (x) || FUNC_D(isnan) (y);
}
weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
