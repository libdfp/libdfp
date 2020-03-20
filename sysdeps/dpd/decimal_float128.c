/* Conversions between _Float128 and _DecimalN types

   Copyright (C) 2020 Free Software Foundation, Inc.

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

#define _GNU_SOURCE
#define __STDC_WANT_IEC_60559_TYPES_EXT__

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Order matters to ensure decNumber object is sufficiently large. */
#include <decimal128.h>
#include <decimal64.h>
#include <decimal32.h>
#include <convert.h>

#define BUFMAX 128

#define DECL_TO_F128(fmode,dmode,N,func) \
    _Float128 \
    func ## dmode ## fmode ## 2 (_Decimal ## N a); \
    _Float128 \
    func ## dmode ## fmode ## 2 (_Decimal ## N a) \
    { \
      char buf[BUFMAX]; \
      decNumber n; \
      decContext context; \
      decContextDefault (&context, DEC_INIT_DECIMAL ## N); \
      decimal ## N ## ToNumber ((decimal ## N *)&a, &n); \
      decNumberToString (&n, buf); \
      return strtof128 (buf, NULL); \
    }

DECL_TO_F128 (kf, sd, 32, __dpd_extend);
DECL_TO_F128 (kf, dd, 64, __dpd_extend);
DECL_TO_F128 (td, kf, 128, __dpd_trunc);

#define DECL_FROM_F128(fmode,dmode,N,func) \
    _Decimal ## N \
    func ## fmode ## dmode ## 2 (_Float128 a); \
    _Decimal ## N \
    func ## fmode ## dmode ## 2 (_Float128 a) \
    { \
       _Decimal ## N f;\
       char buf[BUFMAX]; \
       decContext context; \
       strfromf128 (buf, BUFMAX, "%.35e", a); \
       decContextDefault (&context, DEC_INIT_DECIMAL ## N); \
       decimal ## N ## FromString( (decimal ## N *) &f, buf, &context); \
       if (context.status != 0) { \
         int dec_flags = context.status & \
           (DEC_IEEE_854_Inexact|DEC_IEEE_854_Invalid_operation|DEC_IEEE_854_Overflow); \
           DFP_HANDLE_EXCEPTIONS(DFP_IEEE_FLAGS(dec_flags)); \
       } \
       return f; \
    }

DECL_FROM_F128 (kf, sd, 32, __dpd_trunc);
DECL_FROM_F128 (kf, dd, 64, __dpd_trunc);
DECL_FROM_F128 (td, kf, 128, __dpd_extend);

/* Note, each function has an alias to the GCC 9+ name of the
   intrinsic.  GCC < 9 named these differently.  Also note the
   naming of KF <-> TD functions was reversed too.  */
strong_alias (__dpd_extendsdkf2, __dpd_extendsdkf);
strong_alias (__dpd_extendddkf2, __dpd_extendddkf);
strong_alias (__dpd_trunckftd2, __dpd_trunctdkf);

strong_alias (__dpd_trunckfsd2, __dpd_trunckfsd);
strong_alias (__dpd_trunckfdd2, __dpd_trunckfdd);
strong_alias (__dpd_extendtdkf2, __dpd_extendkftd);

