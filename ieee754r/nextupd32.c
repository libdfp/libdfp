/* Returns the next representable neighbor of x in the direction of plus infinity

   Copyright (C) 2007-2019 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Pedro Caldeira <caldeira@linux.ibm.com>

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
#include <float.h>
#include <errno.h>
#include <ieee754r_private.h>
#include <stdio.h>

#define FUNCTION_NAME nextup

#include <dfpmacro.h>

DEC_TYPE
INTERNAL_FUNCTION_NAME (DEC_TYPE x)
{
	return FUNC_D(__nextafter) (x, DEC_INFINITY);
}

weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
