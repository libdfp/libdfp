/* Internal ieee754r function prototypes used by libdfp
   for generic targets

   Copyright (C) 2019 Free Software Foundation, Inc.

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
   if not, write to the Free Software Foundation, Inc., 59 Temple Place,
   Suite 330, Boston, MA 02111-1307 USA.

   Please see dfp/COPYING.txt for more information.  */
#ifndef _DFP_INLINE_H
#define _DFP_INLINE_H

#include <dfpfenv_private.h>

#ifndef SET_RESTORE_DROUND
struct _rnd_state
{
  int changed;
  int old_mode;
};

static inline __attribute__((always_inline)) void
__restore_rnd (struct _rnd_state *state)
{
  if (state->changed)
    __fe_dec_setround (state->old_mode);
}

#define SET_RESTORE_DROUND_GENERIC(mode) \
  struct _rnd_state __rnd __attribute__ ((__cleanup__ (__restore_rnd))) = \
        { 0, __fe_dec_getround () };                                      \
  if (__rnd.old_mode != (mode))                                           \
    {                                                                     \
       __rnd.changed = 1;                                                 \
       __fe_dec_setround (mode);                                          \
    }

#define SET_RESTORE_DROUND SET_RESTORE_DROUND_GENERIC
#endif

#endif
