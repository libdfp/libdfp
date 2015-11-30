/* Helpers for td -> ti conversions in DPD format.

   Copyright (C) 2015 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Paul E. Murphy <murphyp@linux.vnet.ibm.com>

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

   Please see dfp/COPYING.txt for more information.  */

#ifndef _TIDPDCOMMON_H_
#define _TIDPDCOMMON_H_

/*
   Split and extract components of a large _Decimal 128 number.

   This splits the number a into three components:

   a_high: Upper 16 digits
   a_low: Lower 16 digits
   return value: Middle 2 digits
 */
static inline long
split_td (_Decimal128 a, _Decimal128 * a_high, _Decimal128 * a_low)
{
  _Decimal128 a_high_18;
  union ieee754r_Decimal128 dunion;
  long digs;

  dunion.td = a;
  digs = dunion.ieee.cc5;
  dunion.ieee.cc5 = 0;		/* Set lower 18 digits to 0 */
  dunion.ieee.cc6 = 0;
  dunion.ieee.cc7H8 = 0;
  dunion.ieee.cc7L2 = 0;
  dunion.ieee.cc8 = 0;
  dunion.ieee.cc9 = 0;
  dunion.ieee.cc10 = 0;
  *a_high = dunion.td;		/* a_high should now have only 16 leading dec sig figs */
  digs = dpd_to_bcd[digs];
  digs &= 0xff0;
  dunion.ieee.cc5 = bcd_to_dpd[digs];	/* Replace d15 in mantissa */
  a_high_18 = dunion.td;	/* a_high_18 contains  18 leading dec sig figs */
  *a_low = a - a_high_18;	/* a_low contains trailing 16 sig figs */

  /* Turn two BCD digits into binary.  */
  digs >>= 4;
  return (digs & 0xf) + ((digs >> 4) & 0xf) * 10;
}

#endif /* _TIDPDCOMMON_H_ */
