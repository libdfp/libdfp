/* Return current rounding direction.

   Copyright (C) 2000, 2008, 2009 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Andreas Krebbel  <Andreas.Krebbel@de.ibm.com>

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

#include <fenv_libdfp.h>
#include <fpu_control.h>

/* Based on the binary floating point variants contributed
   by Denis Joseph Barrow (djbarrow@de.ibm.com).  */

int
__fe_dec_getround (void)
{
  fexcept_t cw;

  _FPU_GETCW (cw);

  return (cw & FPC_DFP_RM_MASK) >> 4;
}
strong_alias(__fe_dec_getround, fe_dec_getround)
hidden_def(__fe_dec_getround)

int
__fe_dec_setround (int round)
{
  if ((round|FPC_DFP_RM_MASK) != FPC_DFP_RM_MASK
      || (round > FE_DEC_TONEARESTFROMZERO))
    {
      /* ROUND is not a valid rounding mode.  */
      return 1;
    }
  __asm__ volatile ("srnmt 0(%0)"
		 		     :
		 		     : "a" (round << 4));

  return 0;
}
strong_alias(__fe_dec_setround, fe_dec_setround)
hidden_def(__fe_dec_setround)
