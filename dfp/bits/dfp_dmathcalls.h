/* Macros for declaring _DecimalN functions

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

/* No guarantees are made about the macros in this header.
   Use it directly at your own risk!. */

__DECL_D(acos, (__DECIMAL), __THROW)
__DECL_D(acosh, (__DECIMAL), __THROW)
__DECL_D(asin, (__DECIMAL), __THROW)
__DECL_D(asinh, (__DECIMAL), __THROW)
__DECL_D(atan, (__DECIMAL), __THROW)
__DECL_D(atan2, (__DECIMAL, __DECIMAL), __THROW)
__DECL_D(atanh, (__DECIMAL), __THROW)
__DECL_D(cbrt, (__DECIMAL), __THROW)
__DECL_D(cos, (__DECIMAL), __THROW)
__DECL_D(cosh, (__DECIMAL), __THROW)
__DECL_D(drem, (__DECIMAL, __DECIMAL), __THROW)
__DECL_D(erf, (__DECIMAL), __THROW)
__DECL_D(erfc, (__DECIMAL), __THROW)
__DECL_D(exp, (__DECIMAL), __THROW)
__DECL_D(exp2, (__DECIMAL), __THROW)
__DECL_D(expm1, (__DECIMAL), __THROW)
__DECL_D(fdim, (__DECIMAL, __DECIMAL), __THROW)
__DECL_D(fma, (__DECIMAL, __DECIMAL, __DECIMAL), __THROW)
__DECL_D(fmax, (__DECIMAL, __DECIMAL), __THROW)
__DECL_D(fmin, (__DECIMAL, __DECIMAL), __THROW)
__DECL_D(fmod, (__DECIMAL, __DECIMAL), __THROW)
__DECL_D(frexp, (__DECIMAL, int*), __THROW)
__DECL_D(gamma, (__DECIMAL), __THROW)
__DECL_D(hypot, (__DECIMAL, __DECIMAL), __THROW)
__DECL_D(j0, (__DECIMAL), __THROW)
__DECL_D(j1, (__DECIMAL), __THROW)
__DECL_D(jn, (int, __DECIMAL), __THROW)
__DECL_D(ldexp, (__DECIMAL, int), __THROW)
__DECL_D(lgamma, (__DECIMAL), __THROW)
__DECL_D_R(lgamma, (__DECIMAL, int*), __THROW, _r)
__DECL_D(log, (__DECIMAL), __THROW)
__DECL_D(log10, (__DECIMAL), __THROW)
__DECL_D(log1p, (__DECIMAL), __THROW)
__DECL_D(log2, (__DECIMAL), __THROW)
__DECL_D(logb, (__DECIMAL), __THROW)
__DECL_D(modf, (__DECIMAL, __DECIMAL*), __THROW)
__DECL_D(nearbyint, (__DECIMAL), __THROW)
__DECL_D(pow, (__DECIMAL, __DECIMAL), __THROW)
__DECL_D(quantize, (__DECIMAL, __DECIMAL), __THROW)
__DECL_D(quantum, (__DECIMAL), __THROW)
__DECL_D(remainder, (__DECIMAL, __DECIMAL), __THROW)
__DECL_D(remquo, (__DECIMAL, __DECIMAL, int*), __THROW)
__DECL_D(rint, (__DECIMAL), __THROW)
__DECL_D(scalb, (__DECIMAL, __DECIMAL), __THROW)
__DECL_D(scalbln, (__DECIMAL, long int), __THROW)
__DECL_D(scalbn, (__DECIMAL, int), __THROW)
__DECL_D(significand, (__DECIMAL), __THROW)
__DECL_D(sin, (__DECIMAL), __THROW)
__DECL_D(sinh, (__DECIMAL), __THROW)
__DECL_D(sqrt, (__DECIMAL), __THROW)
__DECL_D(tan, (__DECIMAL), __THROW)
__DECL_D(tanh, (__DECIMAL), __THROW)
__DECL_D(tgamma, (__DECIMAL), __THROW)
__DECL_D(y0, (__DECIMAL), __THROW)
__DECL_D(y1, (__DECIMAL), __THROW)
__DECL_D(yn, (int, __DECIMAL), __THROW)

#ifdef __STDC_WANT_IEC_60559_DFP_EXT__
__DECL_D(fmaxmag, (__DECIMAL, __DECIMAL), __THROW __attribute_const__)
__DECL_D(fminmag, (__DECIMAL, __DECIMAL), __THROW __attribute_const__)
__DECL_D(nextdown, (__DECIMAL), __THROW __attribute_const__)
__DECL_D(nextup, (__DECIMAL), __THROW __attribute_const__)
#endif

__DECL_D(ceil, (__DECIMAL), __THROW __attribute_const__)
__DECL_D(copysign, (__DECIMAL, __DECIMAL), __THROW __attribute_const__)
__DECL_D(fabs, (__DECIMAL), __THROW __attribute_const__)
__DECL_D(floor, (__DECIMAL), __THROW __attribute_const__)
__DECL_D(nan, (__const char *), __THROW __attribute_const__)
__DECL_D(nextafter, (__DECIMAL, __DECIMAL), __THROW __attribute_const__)
__DECL_D(nexttoward, (__DECIMAL, _Decimal128), __THROW __attribute_const__)
__DECL_D(round, (__DECIMAL), __THROW __attribute_const__)
__DECL_D(roundeven, (__DECIMAL), __THROW __attribute_const__)
__DECL_D(trunc, (__DECIMAL), __THROW __attribute_const__)

__DECL_D_O(int, fpclassify, (__DECIMAL), __THROW __attribute_const__)
__DECL_D_O(int, isfinite, (__DECIMAL), __THROW __attribute_const__)
__DECL_D_O(int, isinf, (__DECIMAL), __THROW __attribute_const__)
__DECL_D_O(int, isnan, (__DECIMAL), __THROW __attribute_const__)
__DECL_D_O(int, issignaling, (__DECIMAL), __THROW __attribute_const__)

/* Deprecated but older uses of math.h may have invocations of these if they
 * used the polymorphic finite().  */
__DECL_D_O(int, finite, (__DECIMAL), __THROW __attribute_const__)

__DECL_D_O(int, ilogb, (__DECIMAL), __THROW)
__DECL_D_O(long int, llogb, (__DECIMAL), __THROW)
__DECL_D_O(long long int, llquantexp, (__DECIMAL), __THROW)
__DECL_D_O(long int, lrint, (__DECIMAL), __THROW)
__DECL_D_O(long int, lround, (__DECIMAL), __THROW)
__DECL_D_O(long long int, llrint, (__DECIMAL), __THROW)
__DECL_D_O(long long int, llround, (__DECIMAL), __THROW)
__DECL_D_O(_Bool, samequantum, (__DECIMAL, __DECIMAL), __THROW)

__DECL_D_CMP(__signbit, (__DECIMAL))
__DECL_D_CMP(isgreater, (__DECIMAL, __DECIMAL))
__DECL_D_CMP(isgreaterequal, (__DECIMAL, __DECIMAL))
__DECL_D_CMP(isless, (__DECIMAL, __DECIMAL))
__DECL_D_CMP(islessequal, (__DECIMAL, __DECIMAL))
__DECL_D_CMP(islessgreater, (__DECIMAL, __DECIMAL))
__DECL_D_CMP(isnormal, (__DECIMAL))
__DECL_D_CMP(isunordered, (__DECIMAL, __DECIMAL))
