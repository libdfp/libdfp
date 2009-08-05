/* Raise given exceptions (soft-float edition).

   Copyright (C) 2002, 2009 Free Software Foundation, Inc.

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

   Please see libdfp/COPYING.txt for more information.  */

#if !defined feraiseexcept

/* Taken from GLIBC:
   Contributed by Aldy Hernandez <aldyh@redhat.com>, 2002.  */

#include "soft-fp.h"
#include "soft-supp.h"
#include <signal.h>
#include <bp-sym.h>

#define feraiseexcept (x) \
{						\
  __sim_exceptions |= x;			\
  if (x == 0 || __sim_disabled_exceptions & x)	\
    /* Ignore exception.  */			\
    ;						\
  else						\
    raise (SIGFPE);				\
  return 0;					\
}

#endif /* feraiseexcept */
