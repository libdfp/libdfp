/* Internal interfaces for wcstod[32|64|128] functions.

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2007-2015 Free Software Foundation, Inc.

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
   if not, write to the Free Software Foundation, Inc., 51 Franklin
   Street, Fifth Floor, Boston, MA 02110-1301 USA.

   Please see dfp/COPYING.txt for more information.  */

#ifndef _DFPWCHAR_PRIVATE_H
#define _DFPWCHAR_PRIVATE_H

extern _Decimal32 __wcstod32_internal (const wchar_t * __restrict nptr, wchar_t ** __restrict endptr, int group);
extern _Decimal64 __wcstod64_internal (const wchar_t * __restrict nptr, wchar_t ** __restrict endptr, int group);
extern _Decimal128 __wcstod128_internal (const wchar_t * __restrict nptr, wchar_t ** __restrict endptr, int group);

hidden_proto (__wcstod32_internal)
hidden_proto (__wcstod64_internal)
hidden_proto (__wcstod128_internal)

extern _Decimal32 __wcstod32_l_internal (const wchar_t * __restrict nptr, wchar_t ** __restrict endptr, int group, locale_t loc);
extern _Decimal64 __wcstod64_l_internal (const wchar_t * __restrict nptr, wchar_t ** __restrict endptr, int group, locale_t loc);
extern _Decimal128 __wcstod128_l_internal (const wchar_t * __restrict nptr, wchar_t ** __restrict endptr, int group, locale_t loc);

hidden_proto (__wcstod32_l_internal)
hidden_proto (__wcstod64_l_internal)
hidden_proto (__wcstod128_l_internal)

#endif /* _DFPWCHAR_PRIVATE_H */
