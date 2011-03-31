/* Return current rounding direction.

   Copyright (C) 2000, 2008, 2009, 2011 Free Software Foundation, Inc.

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
#include <dfpfenv_private.h>

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
  /* This currently also allows the extended rounding modes (5, 6 and
     7) which are not covered by the standard.  Please see
     fe_decround.c in the root dir for more information.  */
  if (((round << 4) | FPC_DFP_RM_MASK) != FPC_DFP_RM_MASK)
    {
      /* ROUND is not a valid rounding mode.  */
      return 1;
    }
  __asm__ volatile ("llgfr %0,%0  \n\t"
		    "srnmt 0(%0)"
		    : "+a" (round));

  return 0;
}
strong_alias(__fe_dec_setround, fe_dec_setround)
hidden_def(__fe_dec_setround)

extern int (*__printf_dfp_getround_callback)(void);

static void __attribute__ ((constructor))__init_printf_dfp_getround (void)
{
  __printf_dfp_getround_callback = &__fe_dec_getround;
}
