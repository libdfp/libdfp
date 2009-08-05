/* fenv.h for libdfp and redirect to system fenv.h.

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2007, 2009 Free Software Foundation, Inc.

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

#ifndef _DFP_FENV_H
#define _DFP_FENV_H 1

/*
#ifdef _FENV_H
# error "dfp/<fenv.h> should be included before the system fenv.h."
#endif
*/

#ifdef __STDC_WANT_DEC_FP__

#include <features.h>

/* Power6[x] chips support all five of the defined rounding modes [and then
 * some].  We use the bit pattern in the Power6[x] FPSCR as the values for the
 * appropriate macros.  In order to map these to decNumber rounding modes a
 * conversion will have to take place.  */
enum
  {					/* Corresponds to DEC_ROUND_*  */
    FE_DEC_TONEAREST,			/* DEC_ROUND_HALF_EVEN : 3  */
#define FE_DEC_TONEAREST FE_DEC_TONEAREST
    FE_DEC_TOWARDZERO,			/* DEC_ROUND_DOWN : 5 */
#define FE_DEC_TOWARDZERO FE_DEC_TOWARDZERO
    FE_DEC_UPWARD,			/* DEC_ROUND_CEILING : 0  */
#define FE_DEC_UPWARD FE_DEC_UPWARD
    FE_DEC_DOWNWARD,			/* DEC_ROUND_FLOOR : 6 */
#define FE_DEC_DOWNWARD FE_DEC_DOWNWARD
    FE_DEC_TONEARESTFROMZERO,		/* DEC_ROUND_HALF_UP : 2  */
#define FE_DEC_TONEARESTFROMZERO FE_DEC_TONEARESTFROMZERO
  };

__BEGIN_DECLS

/* Establish the rounding direction for decimal floating point operations  */
extern int fe_dec_setround(int __rounding_direction) __THROW;

/* Get current decimal floating point rounding direction.  */
extern int fe_dec_getround(void) __THROW;

__END_DECLS
#endif /* __STDC_WANT_DEC_FP__  */

/* Pick up the system fenv.h.  */
#include_next <fenv.h>

#endif /* _DFP_FENV_H  */
