/* Decimal Float fe_dec_getround and fe_dec_setround definitions.

   Copyright (C) 2019-2020 Free Software Foundation, Inc.

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

#include <decroundtls.h>
#include <dfpfenv_private.h>
#include <dfp/fenv.h>
#include "bid_conf.h"
#include "bid_functions.h"

#include "bid_gcc_intrinsics.h"

/* The default uses decimal rounding mode stored in TLS.  This file should be
 * overridden in the sysdeps/powerpc/dfpu if hardware supports decimal
 * rounding mode.  */

static int rndtobid[] = {
  ROUNDING_TO_NEAREST,
  ROUNDING_TO_ZERO,
  ROUNDING_UP,
  ROUNDING_DOWN,
  ROUNDING_TIES_AWAY,
};

static int rndtodfp[] = {
  FE_DEC_TONEAREST,
  FE_DEC_DOWNWARD,
  FE_DEC_UPWARD,
  FE_DEC_TOWARDZERO,
  FE_DEC_TONEARESTFROMZERO,
};

#define NUM_RND 5

int
__fe_dec_setround (int rnd)
{
  if (rnd < 0 || rnd >= NUM_RND)
    return 1;
  __dfp_set_round (rndtobid[rnd]);
  return 0;
}

strong_alias (__fe_dec_setround, fe_dec_setround)
hidden_def (__fe_dec_setround)
     int __fe_dec_getround (void)
{
  int rnd = __dfp_get_round ();
  if (rnd < 0 || rnd >= NUM_RND)
    return -1;
  return rndtodfp[rnd];
}

strong_alias (__fe_dec_getround, fe_dec_getround)
hidden_def (__fe_dec_getround)
     extern int (*__printf_dfp_getround_callback) (void);

     static void
       __attribute__((constructor)) __init_printf_dfp_getround (void)
{
  __printf_dfp_getround_callback = &__fe_dec_getround;
}
