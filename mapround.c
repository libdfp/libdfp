/* Get the Decimal Float rounding mode mapped to the decNumber rounding mode
   enumeration values.

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2007, 2009 Free Software Foundation.

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
   if not, write to the Free Software Foundation, Inc., 59 Temple Place,
   Suite 330, Boston, MA 02111-1307 USA.

   Please see libdfp/COPYING.txt for more information.  */

#include <fenv.h>
#include <dfpfenv_private.h>
#include <mapround.h>
#include <decContext.h>

int __dn_getround(void)
{
  /* Only match on supported rounding modes.  Power6[x] hardware supports the
   * greatest number of rounding modes.  The decNumber library supports
   * one fewer than Power6[x] hardware.  */

  /* C Rounding Mode:		Hardware Description
   * 				DecNumber description
   *
   * FE_DEC_TONEAREST:		000 Round to nearest, ties to even.
   * 				DEC_ROUND_HALF_EVEN
   *
   * FE_DEC_TOWARDZERO:		001 Round toward zero.
   * 				DEC_ROUND_DOWN
   *
   * FE_DEC_UPWARD:		010 Round toward +Infinity
   * 				DEC_ROUND_CEILING
   *
   * FE_DEC_DOWNWARD:		011 Round toward -Infinity
   * 				DEC_ROUND_FLOOR
   *
   * FE_DEC_TONEARESTFROMZERO:	100 Round to nearest, ties away from zero
   * 				DEC_ROUND_HALF_UP
   *
   * 5:				101 Round to nearest, ties toward zero
   * 				DEC_ROUND_HALF_DOWN
   *
   * 6:				110 Round away from zero
   * 				DEC_ROUND_UP
   *
   * 7:				111 Round for prepare for shorter precision
   * 				Not supported by decNumber.  */

  switch(__fe_dec_getround())
    {
      case FE_DEC_TONEAREST:
	return DEC_ROUND_HALF_EVEN;
      case FE_DEC_TOWARDZERO:
	return DEC_ROUND_DOWN;
      case FE_DEC_UPWARD:
	return DEC_ROUND_CEILING;
      case FE_DEC_DOWNWARD:
	return DEC_ROUND_FLOOR;
      case FE_DEC_TONEARESTFROMZERO:
	return DEC_ROUND_HALF_UP;
      case 5:
	return DEC_ROUND_HALF_DOWN;
      case 6:
	return DEC_ROUND_UP;
      case 7:
      default:
	return DEC_ROUND_HALF_EVEN;
    }
}
