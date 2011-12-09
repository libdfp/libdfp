/* stdlib.h for libdfp and redirect to system stdlib.h

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2009, 2011 Free Software Foundation, Inc.

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

#ifndef _DFP_STDLIB_H
#define _DFP_STDLIB_H 1

#ifdef __STDC_WANT_DEC_FP__

#include <features.h>
#include <stddef.h>

__BEGIN_DECLS

/* Convert a string to a _Decimal32 number.  */
extern _Decimal32 strtod32 (__const char *__restrict __nptr,
                     char **__restrict __endptr)
     __THROW __nonnull ((1)) __wur;
/* Convert a string to a _Decimal64 number.  */

extern _Decimal64 strtod64 (__const char *__restrict __nptr,
                     char **__restrict __endptr)
     __THROW __nonnull ((1)) __wur;

/* Convert a string to a _Decimal128 number.  */
extern _Decimal128 strtod128 (__const char *__restrict __nptr,
                     char **__restrict __endptr)
     __THROW __nonnull ((1)) __wur;

__END_DECLS

#endif /* __STDC_WANT_DEC_FP__  */

/* Pick up the system stdlib.h.  */
#include_next <stdlib.h>

#endif /* _DFP_STDLIB_H  */
