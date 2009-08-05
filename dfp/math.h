/* math.h for libdfp and redirect to system math.h.  This also replaces the
   system math.h classification macros.

   Copyright (C) 2006 IBM Corporation.
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

#ifndef _DFP_MATH_H
#define _DFP_MATH_H 1

#ifdef _MATH_H
# error "dfp/<math.h> should be included before the system math.h."
#endif

#ifdef __STDC_WANT_DEC_FP__

/* Include the system math.h first so that we can undef and redefine the
 * classification functions to select for _Decimal[32|64|128] types.  */
#include_next <math.h>

#include <features.h>
__BEGIN_DECLS

#define DEC_INFINITY	__builtin_infd32()
#define DEC_NAN		(0.0DF * DEC_INFINITY)
#define HUGE_VAL_D64	__builtin_infd64()

/* #define DEC_INFINITY   (9999999.E96DF + 1.E96df)
 * #define DEC_NAN (0.0DF * DEC_INFINITY)
 * #define HUGE_VAL_D64   (9.999999999999999E384DD + 1.E384dd) */

#define HUGE_VAL_D32	HUGE_VAL_D64
#define HUGE_VAL_D128	HUGE_VAL_D64

/* This method works for both the GCC and the XLC compilers for detecting
 * whether a value is a Decimal Floating Point type.  */
#define __dfp_compatible(X) ((_Decimal128)(__typeof__(X))1.E-50DL == 1.E-50DL)

/* Some useful constants for DFP support (with the DL specifier).  Proper
 * truncation to DD and DF will be handled by GCC.  */
#  define M_Edl		2.7182818284590452353602874713526625DL  /* e */
#  define M_LOG2Edl	1.4426950408889634073599246810018921DL  /* log_2 e */
#  define M_LOG10Edl	0.4342944819032518276511289189166051DL  /* log_10 e */
#  define M_LN2dl	0.6931471805599453094172321214581766DL  /* log_e 2 */
#  define M_LN10dl	2.3025850929940456840179914546843642DL  /* log_e 10 */
#  define M_PIdl	3.1415926535897932384626433832795029DL  /* pi */
#  define M_PI_2dl	1.5707963267948966192313216916397514DL  /* pi/2 */
#  define M_PI_4dl	0.7853981633974483096156608458198757DL  /* pi/4 */
#  define M_1_PIdl	0.3183098861837906715377675267450287DL  /* 1/pi */
#  define M_2_PIdl	0.6366197723675813430755350534900574DL  /* 2/pi */
#  define M_2_SQRTPIdl	1.1283791670955125738961589031215452DL  /* 2/sqrt(pi) */
#  define M_SQRT2dl	1.4142135623730950488016887242096981DL  /* sqrt(2) */
#  define M_SQRT1_2dl	0.7071067811865475244008443621048490DL  /* 1/sqrt(2) */

/* We need the definition of _Bool.  */
#include <stdbool.h>

extern _Decimal32 acosd32 (_Decimal32 __x) __THROW;
extern _Decimal32 __acosd32 (_Decimal32 __x) __THROW;

extern _Decimal32 asind32 (_Decimal32 __x) __THROW;
extern _Decimal32 __asind32 (_Decimal32 __x) __THROW;

extern _Decimal32 atand32 (_Decimal32 __x) __THROW;
extern _Decimal32 __atand32 (_Decimal32 __x) __THROW;

extern _Decimal32 atan2d32 (_Decimal32 __y, _Decimal32 __x) __THROW;
extern _Decimal32 __atan2d32 (_Decimal32 __y, _Decimal32 __x) __THROW;

extern _Decimal32 cosd32 (_Decimal32 __x) __THROW;
extern _Decimal32 __cosd32 (_Decimal32 __x) __THROW;

extern _Decimal32 sind32 (_Decimal32 __x) __THROW;
extern _Decimal32 __sind32 (_Decimal32 __x) __THROW;

extern _Decimal32 tand32 (_Decimal32 __x) __THROW;
extern _Decimal32 __tand32 (_Decimal32 __x) __THROW;

extern _Decimal32 coshd32 (_Decimal32 __x) __THROW;
extern _Decimal32 __coshd32 (_Decimal32 __x) __THROW;

extern _Decimal32 sinhd32 (_Decimal32 __x) __THROW;
extern _Decimal32 __sinhd32 (_Decimal32 __x) __THROW;

extern _Decimal32 tanhd32 (_Decimal32 __x) __THROW;
extern _Decimal32 __tanhd32 (_Decimal32 __x) __THROW;

extern _Decimal32 acoshd32 (_Decimal32 __x) __THROW;
extern _Decimal32 __acoshd32 (_Decimal32 __x) __THROW;

extern _Decimal32 asinhd32 (_Decimal32 __x) __THROW;
extern _Decimal32 __asinhd32 (_Decimal32 __x) __THROW;

extern _Decimal32 atanhd32 (_Decimal32 __x) __THROW;
extern _Decimal32 __atanhd32 (_Decimal32 __x) __THROW;

extern _Decimal32 expd32 (_Decimal32 __x) __THROW;
extern _Decimal32 __expd32 (_Decimal32 __x) __THROW;

extern _Decimal32 frexpd32 (_Decimal32 __x, int *__exponent) __THROW;
extern _Decimal32 __frexpd32 (_Decimal32 __x, int *__exponent) __THROW;

extern _Decimal32 ldexpd32 (_Decimal32 __x, int __exponent) __THROW;
extern _Decimal32 __ldexpd32 (_Decimal32 __x, int __exponent) __THROW;

extern _Decimal32 logd32 (_Decimal32 __x) __THROW;
extern _Decimal32 __logd32 (_Decimal32 __x) __THROW;

extern _Decimal32 log10d32 (_Decimal32 __x) __THROW;
extern _Decimal32 __log10d32 (_Decimal32 __x) __THROW;

extern _Decimal32 modfd32 (_Decimal32 __x, _Decimal32 *__iptr) __THROW;
extern _Decimal32 __modfd32 (_Decimal32 __x, _Decimal32 *__iptr) __THROW;

extern _Decimal32 expm1d32 (_Decimal32 __x) __THROW;
extern _Decimal32 __expm1d32 (_Decimal32 __x) __THROW;

extern _Decimal32 log1pd32 (_Decimal32 __x) __THROW;
extern _Decimal32 __log1pd32 (_Decimal32 __x) __THROW;

extern _Decimal32 logbd32 (_Decimal32 __x) __THROW;
extern _Decimal32 __logbd32 (_Decimal32 __x) __THROW;

extern _Decimal32 exp2d32 (_Decimal32 __x) __THROW;
extern _Decimal32 __exp2d32 (_Decimal32 __x) __THROW;

extern _Decimal32 log2d32 (_Decimal32 __x) __THROW;
extern _Decimal32 __log2d32 (_Decimal32 __x) __THROW;

extern _Decimal32 powd32 (_Decimal32 __x, _Decimal32 __y) __THROW;
extern _Decimal32 __powd32 (_Decimal32 __x, _Decimal32 __y) __THROW;

extern _Decimal32 sqrtd32 (_Decimal32 __x) __THROW;
extern _Decimal32 __sqrtd32 (_Decimal32 __x) __THROW;

extern _Decimal32 hypotd32 (_Decimal32 __x, _Decimal32 __y) __THROW;
extern _Decimal32 __hypotd32 (_Decimal32 __x, _Decimal32 __y) __THROW;

extern _Decimal32 cbrtd32 (_Decimal32 __x) __THROW;
extern _Decimal32 __cbrtd32 (_Decimal32 __x) __THROW;

extern _Decimal32 ceild32 (_Decimal32 __x) __THROW __attribute__ ((__const__));
extern _Decimal32 __ceild32 (_Decimal32 __x) __THROW __attribute__ ((__const__));

extern _Decimal32 fabsd32 (_Decimal32 __x) __THROW __attribute__ ((__const__));
extern _Decimal32 __fabsd32 (_Decimal32 __x) __THROW __attribute__ ((__const__));

extern _Decimal32 floord32 (_Decimal32 __x) __THROW __attribute__ ((__const__));
extern _Decimal32 __floord32 (_Decimal32 __x) __THROW __attribute__ ((__const__));

extern _Decimal32 fmodd32 (_Decimal32 __x, _Decimal32 __y) __THROW;
extern _Decimal32 __fmodd32 (_Decimal32 __x, _Decimal32 __y) __THROW;

extern int isinfd32 (_Decimal32 __value) __THROW __attribute__ ((__const__));
extern int __isinfd32 (_Decimal32 __value) __THROW __attribute__ ((__const__));

extern int finited32 (_Decimal32 __value) __THROW __attribute__ ((__const__));
extern int __finited32 (_Decimal32 __value) __THROW __attribute__ ((__const__));

extern _Decimal32 dremd32 (_Decimal32 __x, _Decimal32 __y) __THROW;
extern _Decimal32 __dremd32 (_Decimal32 __x, _Decimal32 __y) __THROW;

extern _Decimal32 significandd32 (_Decimal32 __x) __THROW;
extern _Decimal32 __significandd32 (_Decimal32 __x) __THROW;

extern _Decimal32 copysignd32 (_Decimal32 __x, _Decimal32 __y) __THROW __attribute__ ((__const__));
extern _Decimal32 __copysignd32 (_Decimal32 __x, _Decimal32 __y) __THROW __attribute__ ((__const__));

extern _Decimal32 nand32 (__const char *__tagb) __THROW __attribute__ ((__const__));
extern _Decimal32 __nand32 (__const char *__tagb) __THROW __attribute__ ((__const__));

extern int isnand32 (_Decimal32 __value) __THROW __attribute__ ((__const__));
extern int __isnand32 (_Decimal32 __value) __THROW __attribute__ ((__const__));

extern _Decimal32 j0d32 (_Decimal32) __THROW;
extern _Decimal32 __j0d32 (_Decimal32) __THROW;

extern _Decimal32 j1d32 (_Decimal32) __THROW;
extern _Decimal32 __j1d32 (_Decimal32) __THROW;

extern _Decimal32 jnd32 (int, _Decimal32) __THROW;
extern _Decimal32 __jnd32 (int, _Decimal32) __THROW;

extern _Decimal32 y0d32 (_Decimal32) __THROW;
extern _Decimal32 __y0d32 (_Decimal32) __THROW;

extern _Decimal32 y1d32 (_Decimal32) __THROW;
extern _Decimal32 __y1d32 (_Decimal32) __THROW;

extern _Decimal32 ynd32 (int, _Decimal32) __THROW;
extern _Decimal32 __ynd32 (int, _Decimal32) __THROW;

extern _Decimal32 erfd32 (_Decimal32) __THROW;
extern _Decimal32 __erfd32 (_Decimal32) __THROW;

extern _Decimal32 erfcd32 (_Decimal32) __THROW;
extern _Decimal32 __erfcd32 (_Decimal32) __THROW;

extern _Decimal32 lgammad32 (_Decimal32) __THROW;
extern _Decimal32 __lgammad32 (_Decimal32) __THROW;

extern _Decimal32 tgammad32 (_Decimal32) __THROW;
extern _Decimal32 __tgammad32 (_Decimal32) __THROW;

extern _Decimal32 gammad32 (_Decimal32) __THROW;
extern _Decimal32 __gammad32 (_Decimal32) __THROW;

extern _Decimal32 lgammad32_r (_Decimal32, int *__signgamp) __THROW;
extern _Decimal32 __lgammad32_r (_Decimal32, int *__signgamp) __THROW;

extern _Decimal32 rintd32 (_Decimal32 __x) __THROW;
extern _Decimal32 __rintd32 (_Decimal32 __x) __THROW;

extern _Decimal32 nextafterd32 (_Decimal32 __x, _Decimal32 __y) __THROW __attribute__ ((__const__));
extern _Decimal32 __nextafterd32 (_Decimal32 __x, _Decimal32 __y) __THROW __attribute__ ((__const__));

extern _Decimal32 nexttowardd32 (_Decimal32 __x, _Decimal128 __y) __THROW __attribute__ ((__const__));
extern _Decimal32 __nexttowardd32 (_Decimal32 __x, _Decimal128 __y) __THROW __attribute__ ((__const__));

extern _Decimal32 remainderd32 (_Decimal32 __x, _Decimal32 __y) __THROW;
extern _Decimal32 __remainderd32 (_Decimal32 __x, _Decimal32 __y) __THROW;

extern _Decimal32 scalbnd32 (_Decimal32 __x, int __n) __THROW;
extern _Decimal32 __scalbnd32 (_Decimal32 __x, int __n) __THROW;

extern int ilogbd32 (_Decimal32 __x) __THROW;
extern int __ilogbd32 (_Decimal32 __x) __THROW;

extern _Decimal32 scalblnd32 (_Decimal32 __x, long int __n) __THROW;
extern _Decimal32 __scalblnd32 (_Decimal32 __x, long int __n) __THROW;

extern _Decimal32 nearbyintd32 (_Decimal32 __x) __THROW;
extern _Decimal32 __nearbyintd32 (_Decimal32 __x) __THROW;

extern _Decimal32 roundd32 (_Decimal32 __x) __THROW __attribute__ ((__const__));
extern _Decimal32 __roundd32 (_Decimal32 __x) __THROW __attribute__ ((__const__));

extern _Decimal32 truncd32 (_Decimal32 __x) __THROW __attribute__ ((__const__));
extern _Decimal32 __truncd32 (_Decimal32 __x) __THROW __attribute__ ((__const__));

extern _Decimal32 remquod32 (_Decimal32 __x, _Decimal32 __y, int *__quo) __THROW;
extern _Decimal32 __remquod32 (_Decimal32 __x, _Decimal32 __y, int *__quo) __THROW;

extern long int lrintd32 (_Decimal32 __x) __THROW;
extern long int __lrintd32 (_Decimal32 __x) __THROW;

extern long long int llrintd32 (_Decimal32 __x) __THROW;
extern long long int __llrintd32 (_Decimal32 __x) __THROW;

extern long int lroundd32 (_Decimal32 __x) __THROW;
extern long int __lroundd32 (_Decimal32 __x) __THROW;

extern long long int llroundd32 (_Decimal32 __x) __THROW;
extern long long int __llroundd32 (_Decimal32 __x) __THROW;

extern _Decimal32 fdimd32 (_Decimal32 __x, _Decimal32 __y) __THROW;
extern _Decimal32 __fdimd32 (_Decimal32 __x, _Decimal32 __y) __THROW;

extern _Decimal32 fmaxd32 (_Decimal32 __x, _Decimal32 __y) __THROW;
extern _Decimal32 __fmaxd32 (_Decimal32 __x, _Decimal32 __y) __THROW;

extern _Decimal32 fmind32 (_Decimal32 __x, _Decimal32 __y) __THROW;
extern _Decimal32 __fmind32 (_Decimal32 __x, _Decimal32 __y) __THROW;

extern int __fpclassifyd32 (_Decimal32 __value) __THROW __attribute__ ((__const__));

extern int __signbitd32 (_Decimal32 __value) __THROW __attribute__ ((__const__));

extern _Decimal32 fmad32 (_Decimal32 __x, _Decimal32 __y, _Decimal32 __z) __THROW;
extern _Decimal32 __fmad32 (_Decimal32 __x, _Decimal32 __y, _Decimal32 __z) __THROW;

extern _Decimal32 scalbd32 (_Decimal32 __x, _Decimal32 __n) __THROW;
extern _Decimal32 __scalbd32 (_Decimal32 __x, _Decimal32 __n) __THROW;

extern _Decimal32 quantized32 (_Decimal32 __x, _Decimal32 __y) __THROW;
extern _Decimal32 __quantized32 (_Decimal32 __x, _Decimal32 __y) __THROW;

extern _Bool samequantumd32 (_Decimal32 __x, _Decimal32 __y) __THROW;
extern _Bool __samequantumd32 (_Decimal32 __x, _Decimal32 __y) __THROW;

extern _Decimal64 acosd64 (_Decimal64 __x) __THROW;
extern _Decimal64 __acosd64 (_Decimal64 __x) __THROW;

extern _Decimal64 asind64 (_Decimal64 __x) __THROW;
extern _Decimal64 __asind64 (_Decimal64 __x) __THROW;

extern _Decimal64 atand64 (_Decimal64 __x) __THROW;
extern _Decimal64 __atand64 (_Decimal64 __x) __THROW;

extern _Decimal64 atan2d64 (_Decimal64 __y, _Decimal64 __x) __THROW;
extern _Decimal64 __atan2d64 (_Decimal64 __y, _Decimal64 __x) __THROW;

extern _Decimal64 cosd64 (_Decimal64 __x) __THROW;
extern _Decimal64 __cosd64 (_Decimal64 __x) __THROW;

extern _Decimal64 sind64 (_Decimal64 __x) __THROW;
extern _Decimal64 __sind64 (_Decimal64 __x) __THROW;

extern _Decimal64 tand64 (_Decimal64 __x) __THROW;
extern _Decimal64 __tand64 (_Decimal64 __x) __THROW;

extern _Decimal64 coshd64 (_Decimal64 __x) __THROW;
extern _Decimal64 __coshd64 (_Decimal64 __x) __THROW;

extern _Decimal64 sinhd64 (_Decimal64 __x) __THROW;
extern _Decimal64 __sinhd64 (_Decimal64 __x) __THROW;

extern _Decimal64 tanhd64 (_Decimal64 __x) __THROW;
extern _Decimal64 __tanhd64 (_Decimal64 __x) __THROW;

extern _Decimal64 acoshd64 (_Decimal64 __x) __THROW;
extern _Decimal64 __acoshd64 (_Decimal64 __x) __THROW;

extern _Decimal64 asinhd64 (_Decimal64 __x) __THROW;
extern _Decimal64 __asinhd64 (_Decimal64 __x) __THROW;

extern _Decimal64 atanhd64 (_Decimal64 __x) __THROW;
extern _Decimal64 __atanhd64 (_Decimal64 __x) __THROW;

extern _Decimal64 expd64 (_Decimal64 __x) __THROW;
extern _Decimal64 __expd64 (_Decimal64 __x) __THROW;

extern _Decimal64 frexpd64 (_Decimal64 __x, int *__exponent) __THROW;
extern _Decimal64 __frexpd64 (_Decimal64 __x, int *__exponent) __THROW;

extern _Decimal64 ldexpd64 (_Decimal64 __x, int __exponent) __THROW;
extern _Decimal64 __ldexpd64 (_Decimal64 __x, int __exponent) __THROW;

extern _Decimal64 logd64 (_Decimal64 __x) __THROW;
extern _Decimal64 __logd64 (_Decimal64 __x) __THROW;

extern _Decimal64 log10d64 (_Decimal64 __x) __THROW;
extern _Decimal64 __log10d64 (_Decimal64 __x) __THROW;

extern _Decimal64 modfd64 (_Decimal64 __x, _Decimal64 *__iptr) __THROW;
extern _Decimal64 __modfd64 (_Decimal64 __x, _Decimal64 *__iptr) __THROW;

extern _Decimal64 expm1d64 (_Decimal64 __x) __THROW;
extern _Decimal64 __expm1d64 (_Decimal64 __x) __THROW;

extern _Decimal64 log1pd64 (_Decimal64 __x) __THROW;
extern _Decimal64 __log1pd64 (_Decimal64 __x) __THROW;

extern _Decimal64 logbd64 (_Decimal64 __x) __THROW;
extern _Decimal64 __logbd64 (_Decimal64 __x) __THROW;

extern _Decimal64 exp2d64 (_Decimal64 __x) __THROW;
extern _Decimal64 __exp2d64 (_Decimal64 __x) __THROW;

extern _Decimal64 log2d64 (_Decimal64 __x) __THROW;
extern _Decimal64 __log2d64 (_Decimal64 __x) __THROW;

extern _Decimal64 powd64 (_Decimal64 __x, _Decimal64 __y) __THROW;
extern _Decimal64 __powd64 (_Decimal64 __x, _Decimal64 __y) __THROW;

extern _Decimal64 sqrtd64 (_Decimal64 __x) __THROW;
extern _Decimal64 __sqrtd64 (_Decimal64 __x) __THROW;

extern _Decimal64 hypotd64 (_Decimal64 __x, _Decimal64 __y) __THROW;
extern _Decimal64 __hypotd64 (_Decimal64 __x, _Decimal64 __y) __THROW;

extern _Decimal64 cbrtd64 (_Decimal64 __x) __THROW;
extern _Decimal64 __cbrtd64 (_Decimal64 __x) __THROW;

extern _Decimal64 ceild64 (_Decimal64 __x) __THROW __attribute__ ((__const__));
extern _Decimal64 __ceild64 (_Decimal64 __x) __THROW __attribute__ ((__const__));

extern _Decimal64 fabsd64 (_Decimal64 __x) __THROW __attribute__ ((__const__));
extern _Decimal64 __fabsd64 (_Decimal64 __x) __THROW __attribute__ ((__const__));

extern _Decimal64 floord64 (_Decimal64 __x) __THROW __attribute__ ((__const__));
extern _Decimal64 __floord64 (_Decimal64 __x) __THROW __attribute__ ((__const__));

extern _Decimal64 fmodd64 (_Decimal64 __x, _Decimal64 __y) __THROW;
extern _Decimal64 __fmodd64 (_Decimal64 __x, _Decimal64 __y) __THROW;

extern int isinfd64 (_Decimal64 __value) __THROW __attribute__ ((__const__));
extern int __isinfd64 (_Decimal64 __value) __THROW __attribute__ ((__const__));

extern int finited64 (_Decimal64 __value) __THROW __attribute__ ((__const__));
extern int __finited64 (_Decimal64 __value) __THROW __attribute__ ((__const__));

extern _Decimal64 dremd64 (_Decimal64 __x, _Decimal64 __y) __THROW;
extern _Decimal64 __dremd64 (_Decimal64 __x, _Decimal64 __y) __THROW;

extern _Decimal64 significandd64 (_Decimal64 __x) __THROW;
extern _Decimal64 __significandd64 (_Decimal64 __x) __THROW;

extern _Decimal64 copysignd64 (_Decimal64 __x, _Decimal64 __y) __THROW __attribute__ ((__const__));
extern _Decimal64 __copysignd64 (_Decimal64 __x, _Decimal64 __y) __THROW __attribute__ ((__const__));

extern _Decimal64 nand64 (__const char *__tagb) __THROW __attribute__ ((__const__));
extern _Decimal64 __nand64 (__const char *__tagb) __THROW __attribute__ ((__const__));

extern int isnand64 (_Decimal64 __value) __THROW __attribute__ ((__const__));
extern int __isnand64 (_Decimal64 __value) __THROW __attribute__ ((__const__));

extern _Decimal64 j0d64 (_Decimal64) __THROW;
extern _Decimal64 __j0d64 (_Decimal64) __THROW;

extern _Decimal64 j1d64 (_Decimal64) __THROW;
extern _Decimal64 __j1d64 (_Decimal64) __THROW;

extern _Decimal64 jnd64 (int, _Decimal64) __THROW;
extern _Decimal64 __jnd64 (int, _Decimal64) __THROW;

extern _Decimal64 y0d64 (_Decimal64) __THROW;
extern _Decimal64 __y0d64 (_Decimal64) __THROW;

extern _Decimal64 y1d64 (_Decimal64) __THROW;
extern _Decimal64 __y1d64 (_Decimal64) __THROW;

extern _Decimal64 ynd64 (int, _Decimal64) __THROW;
extern _Decimal64 __ynd64 (int, _Decimal64) __THROW;

extern _Decimal64 erfd64 (_Decimal64) __THROW;
extern _Decimal64 __erfd64 (_Decimal64) __THROW;

extern _Decimal64 erfcd64 (_Decimal64) __THROW;
extern _Decimal64 __erfcd64 (_Decimal64) __THROW;

extern _Decimal64 lgammad64 (_Decimal64) __THROW;
extern _Decimal64 __lgammad64 (_Decimal64) __THROW;

extern _Decimal64 tgammad64 (_Decimal64) __THROW;
extern _Decimal64 __tgammad64 (_Decimal64) __THROW;

extern _Decimal64 gammad64 (_Decimal64) __THROW;
extern _Decimal64 __gammad64 (_Decimal64) __THROW;

extern _Decimal64 lgammad64_r (_Decimal64, int *__signgamp) __THROW;
extern _Decimal64 __lgammad64_r (_Decimal64, int *__signgamp) __THROW;

extern _Decimal64 rintd64 (_Decimal64 __x) __THROW;
extern _Decimal64 __rintd64 (_Decimal64 __x) __THROW;

extern _Decimal64 nextafterd64 (_Decimal64 __x, _Decimal64 __y) __THROW __attribute__ ((__const__));
extern _Decimal64 __nextafterd64 (_Decimal64 __x, _Decimal64 __y) __THROW __attribute__ ((__const__));

extern _Decimal64 nexttowardd64 (_Decimal64 __x, _Decimal128 __y) __THROW __attribute__ ((__const__));
extern _Decimal64 __nexttowardd64 (_Decimal64 __x, _Decimal128 __y) __THROW __attribute__ ((__const__));

extern _Decimal64 remainderd64 (_Decimal64 __x, _Decimal64 __y) __THROW;
extern _Decimal64 __remainderd64 (_Decimal64 __x, _Decimal64 __y) __THROW;

extern _Decimal64 scalbnd64 (_Decimal64 __x, int __n) __THROW;
extern _Decimal64 __scalbnd64 (_Decimal64 __x, int __n) __THROW;

extern int ilogbd64 (_Decimal64 __x) __THROW;
extern int __ilogbd64 (_Decimal64 __x) __THROW;

extern _Decimal64 scalblnd64 (_Decimal64 __x, long int __n) __THROW;
extern _Decimal64 __scalblnd64 (_Decimal64 __x, long int __n) __THROW;

extern _Decimal64 nearbyintd64 (_Decimal64 __x) __THROW;
extern _Decimal64 __nearbyintd64 (_Decimal64 __x) __THROW;

extern _Decimal64 roundd64 (_Decimal64 __x) __THROW __attribute__ ((__const__));
extern _Decimal64 __roundd64 (_Decimal64 __x) __THROW __attribute__ ((__const__));

extern _Decimal64 truncd64 (_Decimal64 __x) __THROW __attribute__ ((__const__));
extern _Decimal64 __truncd64 (_Decimal64 __x) __THROW __attribute__ ((__const__));

extern _Decimal64 remquod64 (_Decimal64 __x, _Decimal64 __y, int *__quo) __THROW;
extern _Decimal64 __remquod64 (_Decimal64 __x, _Decimal64 __y, int *__quo) __THROW;

extern long int lrintd64 (_Decimal64 __x) __THROW;
extern long int __lrintd64 (_Decimal64 __x) __THROW;

extern long long int llrintd64 (_Decimal64 __x) __THROW;
extern long long int __llrintd64 (_Decimal64 __x) __THROW;

extern long int lroundd64 (_Decimal64 __x) __THROW;
extern long int __lroundd64 (_Decimal64 __x) __THROW;

extern long long int llroundd64 (_Decimal64 __x) __THROW;
extern long long int __llroundd64 (_Decimal64 __x) __THROW;

extern _Decimal64 fdimd64 (_Decimal64 __x, _Decimal64 __y) __THROW;
extern _Decimal64 __fdimd64 (_Decimal64 __x, _Decimal64 __y) __THROW;

extern _Decimal64 fmaxd64 (_Decimal64 __x, _Decimal64 __y) __THROW;
extern _Decimal64 __fmaxd64 (_Decimal64 __x, _Decimal64 __y) __THROW;

extern _Decimal64 fmind64 (_Decimal64 __x, _Decimal64 __y) __THROW;
extern _Decimal64 __fmind64 (_Decimal64 __x, _Decimal64 __y) __THROW;

extern int __fpclassifyd64 (_Decimal64 __value) __THROW __attribute__ ((__const__));

extern int __signbitd64 (_Decimal64 __value) __THROW __attribute__ ((__const__));

extern _Decimal64 fmad64 (_Decimal64 __x, _Decimal64 __y, _Decimal64 __z) __THROW;
extern _Decimal64 __fmad64 (_Decimal64 __x, _Decimal64 __y, _Decimal64 __z) __THROW;

extern _Decimal64 scalbd64 (_Decimal64 __x, _Decimal64 __n) __THROW;
extern _Decimal64 __scalbd64 (_Decimal64 __x, _Decimal64 __n) __THROW;

extern _Decimal64 quantized64 (_Decimal64 __x, _Decimal64 __y) __THROW;
extern _Decimal64 __quantized64 (_Decimal64 __x, _Decimal64 __y) __THROW;

extern _Bool samequantumd64 (_Decimal64 __x, _Decimal64 __y) __THROW;
extern _Bool __samequantumd64 (_Decimal64 __x, _Decimal64 __y) __THROW;

extern _Decimal128 acosd128 (_Decimal128 __x) __THROW;
extern _Decimal128 __acosd128 (_Decimal128 __x) __THROW;

extern _Decimal128 asind128 (_Decimal128 __x) __THROW;
extern _Decimal128 __asind128 (_Decimal128 __x) __THROW;

extern _Decimal128 atand128 (_Decimal128 __x) __THROW;
extern _Decimal128 __atand128 (_Decimal128 __x) __THROW;

extern _Decimal128 atan2d128 (_Decimal128 __y, _Decimal128 __x) __THROW;
extern _Decimal128 __atan2d128 (_Decimal128 __y, _Decimal128 __x) __THROW;

extern _Decimal128 cosd128 (_Decimal128 __x) __THROW;
extern _Decimal128 __cosd128 (_Decimal128 __x) __THROW;

extern _Decimal128 sind128 (_Decimal128 __x) __THROW;
extern _Decimal128 __sind128 (_Decimal128 __x) __THROW;

extern _Decimal128 tand128 (_Decimal128 __x) __THROW;
extern _Decimal128 __tand128 (_Decimal128 __x) __THROW;

extern _Decimal128 coshd128 (_Decimal128 __x) __THROW;
extern _Decimal128 __coshd128 (_Decimal128 __x) __THROW;

extern _Decimal128 sinhd128 (_Decimal128 __x) __THROW;
extern _Decimal128 __sinhd128 (_Decimal128 __x) __THROW;

extern _Decimal128 tanhd128 (_Decimal128 __x) __THROW;
extern _Decimal128 __tanhd128 (_Decimal128 __x) __THROW;

extern _Decimal128 acoshd128 (_Decimal128 __x) __THROW;
extern _Decimal128 __acoshd128 (_Decimal128 __x) __THROW;

extern _Decimal128 asinhd128 (_Decimal128 __x) __THROW;
extern _Decimal128 __asinhd128 (_Decimal128 __x) __THROW;

extern _Decimal128 atanhd128 (_Decimal128 __x) __THROW;
extern _Decimal128 __atanhd128 (_Decimal128 __x) __THROW;

extern _Decimal128 expd128 (_Decimal128 __x) __THROW;
extern _Decimal128 __expd128 (_Decimal128 __x) __THROW;

extern _Decimal128 frexpd128 (_Decimal128 __x, int *__exponent) __THROW;
extern _Decimal128 __frexpd128 (_Decimal128 __x, int *__exponent) __THROW;

extern _Decimal128 ldexpd128 (_Decimal128 __x, int __exponent) __THROW;
extern _Decimal128 __ldexpd128 (_Decimal128 __x, int __exponent) __THROW;

extern _Decimal128 logd128 (_Decimal128 __x) __THROW;
extern _Decimal128 __logd128 (_Decimal128 __x) __THROW;

extern _Decimal128 log10d128 (_Decimal128 __x) __THROW;
extern _Decimal128 __log10d128 (_Decimal128 __x) __THROW;

extern _Decimal128 modfd128 (_Decimal128 __x, _Decimal128 *__iptr) __THROW;
extern _Decimal128 __modfd128 (_Decimal128 __x, _Decimal128 *__iptr) __THROW;

extern _Decimal128 expm1d128 (_Decimal128 __x) __THROW;
extern _Decimal128 __expm1d128 (_Decimal128 __x) __THROW;

extern _Decimal128 log1pd128 (_Decimal128 __x) __THROW;
extern _Decimal128 __log1pd128 (_Decimal128 __x) __THROW;

extern _Decimal128 logbd128 (_Decimal128 __x) __THROW;
extern _Decimal128 __logbd128 (_Decimal128 __x) __THROW;

extern _Decimal128 exp2d128 (_Decimal128 __x) __THROW;
extern _Decimal128 __exp2d128 (_Decimal128 __x) __THROW;

extern _Decimal128 log2d128 (_Decimal128 __x) __THROW;
extern _Decimal128 __log2d128 (_Decimal128 __x) __THROW;

extern _Decimal128 powd128 (_Decimal128 __x, _Decimal128 __y) __THROW;
extern _Decimal128 __powd128 (_Decimal128 __x, _Decimal128 __y) __THROW;

extern _Decimal128 sqrtd128 (_Decimal128 __x) __THROW;
extern _Decimal128 __sqrtd128 (_Decimal128 __x) __THROW;

extern _Decimal128 hypotd128 (_Decimal128 __x, _Decimal128 __y) __THROW;
extern _Decimal128 __hypotd128 (_Decimal128 __x, _Decimal128 __y) __THROW;

extern _Decimal128 cbrtd128 (_Decimal128 __x) __THROW;
extern _Decimal128 __cbrtd128 (_Decimal128 __x) __THROW;

extern _Decimal128 ceild128 (_Decimal128 __x) __THROW __attribute__ ((__const__));
extern _Decimal128 __ceild128 (_Decimal128 __x) __THROW __attribute__ ((__const__));

extern _Decimal128 fabsd128 (_Decimal128 __x) __THROW __attribute__ ((__const__));
extern _Decimal128 __fabsd128 (_Decimal128 __x) __THROW __attribute__ ((__const__));

extern _Decimal128 floord128 (_Decimal128 __x) __THROW __attribute__ ((__const__));
extern _Decimal128 __floord128 (_Decimal128 __x) __THROW __attribute__ ((__const__));

extern _Decimal128 fmodd128 (_Decimal128 __x, _Decimal128 __y) __THROW;
extern _Decimal128 __fmodd128 (_Decimal128 __x, _Decimal128 __y) __THROW;

extern int isinfd128 (_Decimal128 __value) __THROW __attribute__ ((__const__));
extern int __isinfd128 (_Decimal128 __value) __THROW __attribute__ ((__const__));

extern int finited128 (_Decimal128 __value) __THROW __attribute__ ((__const__));
extern int __finited128 (_Decimal128 __value) __THROW __attribute__ ((__const__));

extern _Decimal128 dremd128 (_Decimal128 __x, _Decimal128 __y) __THROW;
extern _Decimal128 __dremd128 (_Decimal128 __x, _Decimal128 __y) __THROW;

extern _Decimal128 significandd128 (_Decimal128 __x) __THROW;
extern _Decimal128 __significandd128 (_Decimal128 __x) __THROW;

extern _Decimal128 copysignd128 (_Decimal128 __x, _Decimal128 __y) __THROW __attribute__ ((__const__));
extern _Decimal128 __copysignd128 (_Decimal128 __x, _Decimal128 __y) __THROW __attribute__ ((__const__));

extern _Decimal128 nand128 (__const char *__tagb) __THROW __attribute__ ((__const__));
extern _Decimal128 __nand128 (__const char *__tagb) __THROW __attribute__ ((__const__));

extern int isnand128 (_Decimal128 __value) __THROW __attribute__ ((__const__));
extern int __isnand128 (_Decimal128 __value) __THROW __attribute__ ((__const__));

extern _Decimal128 j0d128 (_Decimal128) __THROW;
extern _Decimal128 __j0d128 (_Decimal128) __THROW;

extern _Decimal128 j1d128 (_Decimal128) __THROW;
extern _Decimal128 __j1d128 (_Decimal128) __THROW;

extern _Decimal128 jnd128 (int, _Decimal128) __THROW;
extern _Decimal128 __jnd128 (int, _Decimal128) __THROW;

extern _Decimal128 y0d128 (_Decimal128) __THROW;
extern _Decimal128 __y0d128 (_Decimal128) __THROW;

extern _Decimal128 y1d128 (_Decimal128) __THROW;
extern _Decimal128 __y1d128 (_Decimal128) __THROW;

extern _Decimal128 ynd128 (int, _Decimal128) __THROW;
extern _Decimal128 __ynd128 (int, _Decimal128) __THROW;

extern _Decimal128 erfd128 (_Decimal128) __THROW;
extern _Decimal128 __erfd128 (_Decimal128) __THROW;

extern _Decimal128 erfcd128 (_Decimal128) __THROW;
extern _Decimal128 __erfcd128 (_Decimal128) __THROW;

extern _Decimal128 lgammad128 (_Decimal128) __THROW;
extern _Decimal128 __lgammad128 (_Decimal128) __THROW;

extern _Decimal128 tgammad128 (_Decimal128) __THROW;
extern _Decimal128 __tgammad128 (_Decimal128) __THROW;

extern _Decimal128 gammad128 (_Decimal128) __THROW;
extern _Decimal128 __gammad128 (_Decimal128) __THROW;

extern _Decimal128 lgammad128_r (_Decimal128, int *__signgamp) __THROW;
extern _Decimal128 __lgammad128_r (_Decimal128, int *__signgamp) __THROW;

extern _Decimal128 rintd128 (_Decimal128 __x) __THROW;
extern _Decimal128 __rintd128 (_Decimal128 __x) __THROW;

extern _Decimal128 nextafterd128 (_Decimal128 __x, _Decimal128 __y) __THROW __attribute__ ((__const__));
extern _Decimal128 __nextafterd128 (_Decimal128 __x, _Decimal128 __y) __THROW __attribute__ ((__const__));

extern _Decimal128 nexttowardd128 (_Decimal128 __x, _Decimal128 __y) __THROW __attribute__ ((__const__));
extern _Decimal128 __nexttowardd128 (_Decimal128 __x, _Decimal128 __y) __THROW __attribute__ ((__const__));

extern _Decimal128 remainderd128 (_Decimal128 __x, _Decimal128 __y) __THROW;
extern _Decimal128 __remainderd128 (_Decimal128 __x, _Decimal128 __y) __THROW;

extern _Decimal128 scalbnd128 (_Decimal128 __x, int __n) __THROW;
extern _Decimal128 __scalbnd128 (_Decimal128 __x, int __n) __THROW;

extern int ilogbd128 (_Decimal128 __x) __THROW;
extern int __ilogbd128 (_Decimal128 __x) __THROW;

extern _Decimal128 scalblnd128 (_Decimal128 __x, long int __n) __THROW;
extern _Decimal128 __scalblnd128 (_Decimal128 __x, long int __n) __THROW;

extern _Decimal128 nearbyintd128 (_Decimal128 __x) __THROW;
extern _Decimal128 __nearbyintd128 (_Decimal128 __x) __THROW;

extern _Decimal128 roundd128 (_Decimal128 __x) __THROW __attribute__ ((__const__));
extern _Decimal128 __roundd128 (_Decimal128 __x) __THROW __attribute__ ((__const__));

extern _Decimal128 truncd128 (_Decimal128 __x) __THROW __attribute__ ((__const__));
extern _Decimal128 __truncd128 (_Decimal128 __x) __THROW __attribute__ ((__const__));

extern _Decimal128 remquod128 (_Decimal128 __x, _Decimal128 __y, int *__quo) __THROW;
extern _Decimal128 __remquod128 (_Decimal128 __x, _Decimal128 __y, int *__quo) __THROW;

extern long int lrintd128 (_Decimal128 __x) __THROW;
extern long int __lrintd128 (_Decimal128 __x) __THROW;

extern long long int llrintd128 (_Decimal128 __x) __THROW;
extern long long int __llrintd128 (_Decimal128 __x) __THROW;

extern long int lroundd128 (_Decimal128 __x) __THROW;
extern long int __lroundd128 (_Decimal128 __x) __THROW;

extern long long int llroundd128 (_Decimal128 __x) __THROW;
extern long long int __llroundd128 (_Decimal128 __x) __THROW;

extern _Decimal128 fdimd128 (_Decimal128 __x, _Decimal128 __y) __THROW;
extern _Decimal128 __fdimd128 (_Decimal128 __x, _Decimal128 __y) __THROW;

extern _Decimal128 fmaxd128 (_Decimal128 __x, _Decimal128 __y) __THROW;
extern _Decimal128 __fmaxd128 (_Decimal128 __x, _Decimal128 __y) __THROW;

extern _Decimal128 fmind128 (_Decimal128 __x, _Decimal128 __y) __THROW;
extern _Decimal128 __fmind128 (_Decimal128 __x, _Decimal128 __y) __THROW;

extern int __fpclassifyd128 (_Decimal128 __value) __THROW __attribute__ ((__const__));

extern int __signbitd128 (_Decimal128 __value) __THROW __attribute__ ((__const__));

extern _Decimal128 fmad128 (_Decimal128 __x, _Decimal128 __y, _Decimal128 __z) __THROW;
extern _Decimal128 __fmad128 (_Decimal128 __x, _Decimal128 __y, _Decimal128 __z) __THROW;

extern _Decimal128 scalbd128 (_Decimal128 __x, _Decimal128 __n) __THROW;
extern _Decimal128 __scalbd128 (_Decimal128 __x, _Decimal128 __n) __THROW;

extern _Decimal128 quantized128 (_Decimal128 __x, _Decimal128 __y) __THROW;
extern _Decimal128 __quantized128 (_Decimal128 __x, _Decimal128 __y) __THROW;

extern _Bool samequantumd128 (_Decimal128 __x, _Decimal128 __y) __THROW;
extern _Bool __samequantumd128 (_Decimal128 __x, _Decimal128 __y) __THROW;

/* Classification functions.  We undefine those defined by the system math.h
 * so that we can add the _Decimal[32|64|128] types recognition to the macros.
 */

#ifdef fpclassify
# undef fpclassify
#endif

/* Return number of classification appropriate for X.  */
# ifdef __NO_LONG_DOUBLE_MATH
#  define ____fpclassify(x) \
     (sizeof (x) == sizeof (float) ? __fpclassifyf (x) : __fpclassify (x))
# else
#  define ____fpclassify(x) \
     (sizeof (x) == sizeof (float)					      \
      ? __fpclassifyf (x)						      \
      : sizeof (x) == sizeof (double)					      \
      ? __fpclassify (x) : __fpclassifyl (x))
# endif

#define fpclassify(x) \
  (									      \
    (!__dfp_compatible(x)? (____fpclassify(x)) :			      \
      (sizeof (x) == sizeof (_Decimal128)? __fpclassifyd128(x):		      \
	(sizeof (x) == sizeof (_Decimal64)? __fpclassifyd64(x):		      \
	  __fpclassifyd32(x)						      \
	)								      \
      )									      \
    )									      \
  )

#ifdef signbit
# undef signbit
#endif

/* Return nonzero value if sign of X is negative.  */
# ifdef __NO_LONG_DOUBLE_MATH
#  define ____signbit(x) \
     (sizeof (x) == sizeof (float) ? __signbitf (x) : __signbit (x))
# else
#  define ____signbit(x) \
     (sizeof (x) == sizeof (float)					      \
      ? __signbitf (x)							      \
      : sizeof (x) == sizeof (double)					      \
      ? __signbit (x) : __signbitl (x))
# endif


#define signbit(x) \
  (									      \
    (!__dfp_compatible(x)? (____signbit(x)) :				      \
      (sizeof (x) == sizeof (_Decimal128)? __signbitd128(x):		      \
	(sizeof (x) == sizeof (_Decimal64)? __signbitd64(x):		      \
	  __signbitd32(x)						      \
	)								      \
      )									      \
    )									      \
  )

#ifdef isfinite
# undef isfinite
#endif

/* Return nonzero value if X is not +-Inf or NaN.  */
# ifdef __NO_LONG_DOUBLE_MATH
#  define ____isfinite(x) \
     (sizeof (x) == sizeof (float) ? __finitef (x) : __finite (x))
# else
#  define ____isfinite(x) \
     (sizeof (x) == sizeof (float)					      \
      ? __finitef (x)							      \
      : sizeof (x) == sizeof (double)					      \
      ? __finite (x) : __finitel (x))
# endif

#define isfinite(x)   \
  (									      \
    (!__dfp_compatible(x)? (____isfinite(x)) :				      \
      (sizeof (x) == sizeof (_Decimal128)? __finited128(x):		      \
	(sizeof (x) == sizeof (_Decimal64)? __finited64(x):		      \
	  __finited32(x)						      \
	)								      \
      )									      \
    )									      \
  )

/* We redefine this here in-case the math.h inclusion resulted in some
 * macro expansions.  */
#ifdef isnormal
# undef isnormal
#endif
/* Return nonzero value if X is neither zero, subnormal, Inf, nor NaN.  */
#define isnormal(x) (fpclassify (x) == FP_NORMAL)

#ifdef isnan
# undef isnan
#endif

/* Return nonzero value if X is a NaN.  We could use `fpclassify' but
   we already have this functions `__isnan' and it is faster.  */
# ifdef __NO_LONG_DOUBLE_MATH
#  define ____isnan(x) \
     (sizeof (x) == sizeof (float) ? __isnanf (x) : __isnan (x))
# else
#  define ____isnan(x) \
     (sizeof (x) == sizeof (float)		      \
      ? __isnanf (x)				      \
      : sizeof (x) == sizeof (double)		      \
      ? __isnan (x) : __isnanl (x))
# endif


#define isnan(x) \
  (!__dfp_compatible(x)						      \
    ? (____isnan(x))							      \
    : (sizeof (x) == sizeof (_Decimal128)				      \
      ? __isnand128(x)							      \
      : (sizeof (x) == sizeof (_Decimal64)				      \
	? __isnand64(x)							      \
	: __isnand32(x)))						      \
  )

#ifdef isinf
# undef isinf
#endif

/* Return nonzero value is X is positive or negative infinity.  */
# ifdef __NO_LONG_DOUBLE_MATH
#  define _____isinf(x) \
     (sizeof (x) == sizeof (float) ? __isinff (x) : __isinf (x))
# else
#  define ____isinf(x) \
     (sizeof (x) == sizeof (float)					      \
      ? __isinff (x)							      \
      : sizeof (x) == sizeof (double)					      \
      ? __isinf (x) : __isinfl (x))
# endif

#define isinf(x) \
  (									      \
    (!__dfp_compatible(x)? (____isinf(x)) :				      \
      (sizeof (x) == sizeof (_Decimal128)? __isinfd128(x):		      \
	(sizeof (x) == sizeof (_Decimal64)? __isinfd64(x):		      \
	  __isinfd32(x)							      \
	)								      \
      )									      \
    )									      \
  )

#ifdef fabs /* This is actually new in WG21 N2849.  */
# undef fabs
#endif
#define fabs(x) \
  (									      \
    (sizeof (x) == sizeof (_Decimal128)? __fabsd128(x):			      \
       (sizeof (x) == sizeof (_Decimal64)? __fabsd64(x):		      \
	  __fabsd32(x)							      \
       )								      \
    )									      \
  )

__END_DECLS

#endif /* __STDC_WANT_DEC_FP__  */

#endif /* _DFP_MATH_H  */
