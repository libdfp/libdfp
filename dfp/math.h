/* math.h for libdfp and redirect to system math.h.  This also replaces the
   system math.h classification macros.

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2009-2020 Free Software Foundation, Inc.

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

   Please see libdfp/COPYING.txt for more information.  */

#ifndef _DFP_MATH_H
#define _DFP_MATH_H 1

#ifdef _MATH_H
# error "dfp/<math.h> should be included before the system math.h."
#endif

/* Include the system math.h first so that we can undef and redefine the
 * classification functions to select for _Decimal[32|64|128] types.  */
#include_next <math.h>

/* Include this after the #include_next <math.h> so that if the user has
 * -I/<path>/include/dfp/ on their compilation line but doesn't define
 *  __STDC_WANT_DEC_FPP that they still get the system math.h.  */
#ifdef __STDC_WANT_DEC_FP__

#include <stdbool.h>

/* math.h provides prototypes for the classification macros below. */
#include <math.h>

#define DEC_INFINITY	__builtin_infd32()
#define DEC_NAN		__builtin_nand32("")
#define HUGE_VAL_D32	__builtin_infd32()
#define HUGE_VAL_D64	__builtin_infd64()
#define HUGE_VAL_D128	__builtin_infd128()

/* Some useful constants for DFP support (with the DL specifier).  Proper
 * truncation to DD and DF will be handled by GCC.  */
#define M_Edl		2.71828182845904523536028747135266249DL /* e */
#define M_LOG2Edl	1.4426950408889634073599246810018921DL  /* log_2 e */
#define M_LOG10Edl	0.4342944819032518276511289189166051DL  /* log_10 e */
#define M_LN2dl		0.6931471805599453094172321214581766DL  /* log_e 2 */
#define M_LN10dl	2.3025850929940456840179914546843642DL  /* log_e 10 */
#define M_PIdl		3.1415926535897932384626433832795029DL  /* pi */
#define M_PI_2dl	1.5707963267948966192313216916397514DL  /* pi/2 */
#define M_PI_4dl	0.7853981633974483096156608458198757DL  /* pi/4 */
#define M_1_PIdl	0.3183098861837906715377675267450287DL  /* 1/pi */
#define M_2_PIdl	0.6366197723675813430755350534900574DL  /* 2/pi */
#define M_2_SQRTPIdl	1.1283791670955125738961589031215452DL  /* 2/sqrt(pi) */
#define M_SQRT2dl	1.4142135623730950488016887242096981DL  /* sqrt(2) */
#define M_SQRT1_2dl	0.7071067811865475244008443621048490DL  /* 1/sqrt(2) */

#define __DECL_D(f, a, attr) __DECL_D_X(__DECIMAL, f, a, attr, __DECIMALN, )
#define __DECL_D_R(f, a, attr, r) __DECL_D_X(__DECIMAL, f, a, attr, __DECIMALN, r)
#define __DECL_D_O(rtn, f, a, attr) __DECL_D_X(rtn, f, a, attr, __DECIMALN, )
#define __DECL_D_X(rtn, f, a, attr, n, r) __DECL_D_X2(rtn, f, a, attr, n, r)
#define __DECL_D_X2(rtn, f, a, attr, n, r) \
	extern rtn f ## d ## n ## r a attr; \
	extern rtn __ ## f ## d ## n ## r a attr;

#define __DECL_D_CMP(f,a) __DECL_D_CMP_X(f,a,__DECIMALN)
#define __DECL_D_CMP_X(f,a,n) __DECL_D_CMP_X2(f,a,n)
#define __DECL_D_CMP_X2(f,a,n) extern int f ## d ## n a __THROW __attribute_const__;

#ifndef __attribute_const__
#define __attribute__ ((__const__))
#endif

__BEGIN_DECLS

#define __DECIMAL _Decimal32
#define __DECIMALN 32
#include <bits/dfp_dmathcalls.h>
#undef __DECIMAL
#undef __DECIMALN

#define __DECIMAL _Decimal64
#define __DECIMALN 64
#include <bits/dfp_dmathcalls.h>
#undef __DECIMAL
#undef __DECIMALN

#define __DECIMAL _Decimal128
#define __DECIMALN 128
#include <bits/dfp_dmathcalls.h>
#undef __DECIMAL
#undef __DECIMALN

__END_DECLS

#undef __DECL_D
#undef __DECL_D_R
#undef __DECL_D_O
#undef __DECL_D_X
#undef __DECL_D_X2
#undef __DECL_D_CMP
#undef __DECL_D_CMP_X
#undef __DECL_D_CMP_X2

/* This method works for both the GCC and the XLC compilers for detecting
 * whether a value is a Decimal Floating Point type.  */
#define __dfp_compatible(X) ((_Decimal128)(__typeof__(X))1.E-50DL == 1.E-50DL)

/* Override math.h classifications with our own. */
#undef fpclassify
#undef signbit
#undef isfinite
#undef isnormal
#undef isinf
#undef fabs
#undef isnan

# ifdef __NO_LONG_DOUBLE_MATH
#  define __dfp_ldbl(f,x) f(x)
# else
#  define __dfp_ldbl(f,x) (sizeof(x) == sizeof(double) ? f(x) : f ## l (x))
# endif

# define __dfp_dN(f,x) (sizeof (x) == sizeof(_Decimal128) ? f ## d128 (x) :   \
	(sizeof (x) == sizeof(_Decimal64) ? f ## d64 (x) : f ## d32 (x)))

/* Return number of classification appropriate for X.  */
#define __dfp_fpclassify(x) \
     (sizeof (x) == sizeof (float)					      \
      ? __fpclassifyf (x)						      \
      : __dfp_ldbl (__fpclassify, x))

#define __dfp_signbit(x) \
     (sizeof (x) == sizeof (float)					      \
      ? __signbitf (x)							      \
      : __dfp_ldbl(__signbit, x))

#define __dfp_isfinite(x) \
     (sizeof (x) == sizeof (float)					      \
      ? __finitef (x)							      \
      : __dfp_ldbl(__finite, x))

#define __dfp_isinf(x) \
     (sizeof (x) == sizeof (float)					      \
      ? __isinff (x)							      \
      : __dfp_ldbl(__isinf, x))

#define __dfp_isnan(x) \
     (sizeof (x) == sizeof (float)		      			      \
      ? __isnanf (x)				      			      \
      : __dfp_ldbl(__isnan, x))

/* A special case here.  Older glibc may not support this.  */
#ifdef issignaling
#undef issignaling
#define __dfp_issignaling(x)						      \
     (sizeof (x) == sizeof (float)					      \
      ? __issignalingf (x)						      \
      : __dfp_ldbl(__issignaling, x))
#else
#define __dfp_issignaling(x) x
#endif

#define fpclassify(x) \
  (!__dfp_compatible(x) ? __dfp_fpclassify(x) : __dfp_dN(__fpclassify, x))

#define signbit(x) \
  (!__dfp_compatible(x) ? __dfp_signbit(x) : __dfp_dN(__signbit, x))

#define isfinite(x)   \
  (!__dfp_compatible(x) ? __dfp_isfinite(x) : __dfp_dN(__isfinite, x))

#define isnormal(x) (fpclassify (x) == FP_NORMAL)

#define isinf(x) \
  (!__dfp_compatible(x)? __dfp_isinf(x) : __dfp_dN(__isinf, x))

#define isnan(x) \
  (!__dfp_compatible(x) ? __dfp_isnan(x) : __dfp_dN(__isnan, x))

#define issignaling(x) \
  (!__dfp_compatible(x) ? __dfp_issignaling(x) : __dfp_dN(__issignaling, x))

#endif /* __STDC_WANT_DEC_FP__  */

#endif /* _DFP_MATH_H  */
