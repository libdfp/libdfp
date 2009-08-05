/* Decimal Float fe_dec_getround and fe_dec_setround definitions.

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

#include <decroundtls.h>
#include <dfpfenv_private.h>

/* The default uses decimal rounding mode stored in TLS.  This file should be
 * overridden in the sysdeps/powerpc/dfpu if hardware supports decimal
 * rounding mode.  */

int __fe_dec_setround(int __rounding_direction)
{

  /* Only match on supported rounding modes.  The decNum library supports
   * additional ones that aren't valid C rounding modes.  We also allow rounding
   * modes to be set covertly for the three additional hardware rounding modes
   * supported by Power6[x] that aren't in the DFP Draft Technical Report.  */

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
  if (__rounding_direction > 7)
    return 1;
  *__decrm_location() = __rounding_direction;
  return 0;
}
strong_alias(__fe_dec_setround, fe_dec_setround)
hidden_def(__fe_dec_setround)

int __fe_dec_getround(void)
{
  return *__decrm_location();
}
strong_alias(__fe_dec_getround, fe_dec_getround)
hidden_def(__fe_dec_getround)

extern int (*__printf_dfp_getround_callback)(void);

extern void __init_printf_dfp_getround (void);

void __attribute__ ((constructor))__init_printf_dfp_getround (void)
{
  __printf_dfp_getround_callback = &__fe_dec_getround;
}
hidden_def (__init_printf_dfp_getround);
