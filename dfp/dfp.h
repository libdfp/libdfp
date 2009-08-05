/* Prototypes for libdfp specific functions not defined by any standard.

   Copyright (C) 2008, 2009 Free Software Foundation, Inc.

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

#ifndef _DFP_H
#define _DFP_H
#include <features.h>

__BEGIN_DECLS

/* In order to inform libc that a decimal floating point type needs to be
 * printed libdfp needs to inform libc, prior to the first invocation of printf,
 * that it has printf callbacks available for decimal floating point types.  The
 * user should invoke this function to register the libdfp printf callbacks with
 * libc.  */
extern int register_printf_dfp (void);

__END_DECLS

#endif  /* _DFP_H  */
