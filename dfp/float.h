/* <float.h> for libdfp and redirect to system <float.h>

   Copyright (C) 2011, 2012 Free Software Foundation, Inc.

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

/* This uses a bit of circular including in order to get std::decimal defined.
 * without error such that we can typedef _Decimal types to decimal types.  */
#ifndef _LIBDFP_FLOAT_H
#define _LIBDFP_FLOAT_H 1
#include_next <float.h>

#pragma GCC system_header

#ifdef __cplusplus

/* If _Decimal64 is defined we don't need to do any of this.  */
#ifndef _Decimal64

/* Predeclare the decimal classes so we can typedef without errors.  */
//namespace std { namespace decimal { class decimal32; }; };
//namespace std { namespace decimal { class decimal64; }; };
//namespace std { namespace decimal { class decimal128; }; };

/* Per ISO/IEC TR 24733 the following typedefs SHOULD be defined in float.h
*
 * typedef std::decimal::decimal32  _Decimal32;
 * typedef std::decimal::decimal64  _Decimal64;
 * typedef std::decimal::decimal128 _Decimal128;
 *
 * Depending on the compiler version they may NOT be, so we'll define them
 * as a service if they are not defined in the system float.h.
 */

typedef float _Decimal32 __attribute__((mode(SD)));
typedef float _Decimal64 __attribute__((mode(DD)));
typedef float _Decimal128 __attribute__((mode(TD)));
#endif /* _Decimal64  */
#endif /* __cplusplus  */
#endif /* _LIBDFP_FLOAT_H  */
