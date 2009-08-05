/* wchar.h for libdfp and redirect to system wchar.h.

   Copyright (C) 2009 Free Software Foundation, Inc.

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

#ifndef _DFP_WCHAR_H
#define _DFP_WCHAR_H 1

#ifdef __STDC_WANT_DEC_FP__

#include <stddef.h>

__BEGIN_DECLS
extern _Decimal32 wcstod32 (__const wchar_t *__restrict __nptr, wchar_t **__restrict __endptr) __THROW;

extern _Decimal64 wcstod64 (__const wchar_t *__restrict __nptr, wchar_t **__restrict __endptr) __THROW;

extern _Decimal128 wcstod128 (__const wchar_t *__restrict __nptr, wchar_t **__restrict __endptr) __THROW;
__END_DECLS

#endif /* __STDC_WANT_DEC_FP__  */

/* Pick up the system fenv.h.  */
#include_next <wchar.h>

#endif /* _DFP_WCHAR_H  */
