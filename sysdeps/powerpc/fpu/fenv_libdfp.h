/* Some FPSCR functions.

   Copyright (C) 2009 IBM Corporation.
   Copyright (C) 2007 Free Software Foundation, Inc.

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

   Please see dfp/COPYING.txt for more information.  */

#ifndef _FENV_LIBDFP_H
#define _FENV_LIBDFP_H	1

#include <fenv.h>


typedef union
{
  fenv_t fenv;
  unsigned int l[2];
} fenv_union_t;

# define fegetenv_register() \
   ({ fenv_t env; asm volatile ("mffs %0" : "=f" (env)); env; })

#ifndef _ARCH_PWR6
/* This could have been done in another sysdeps directory but it is easiest
 * here.  */
# define fesetenv_register(env) \
   do { \
     double d = (env); \
     asm volatile ("mtfsf 0xff,%0" : : "f" (d)); \
   } while(0)
#else
/* Power6 & Power7 have a 64-bit FPSCR.  */
# define fesetenv_register(env) \
   do { \
     double d = (env); \
     asm volatile ("mtfsf 0xff,%0,1,0; " : : "f" (d)); \
   } while(0)
#endif /* _ARCH_PWR6  */

#endif /* _FENV_LIBDFP_H  */
