/* Test dfp/math.h overrides

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
   if not, write to the Free Software Foundation, Inc., 51 Franklin
   Street, Fifth Floor, Boston, MA 02110-1301 USA.

   Please see libdfp/COPYING.txt for more information.  */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* Hacky, but there isn't otherwise a good test today.  Note presumably
   unique integer types are used to verify function selection, otherwise
   you will find most compilers get upset when mixing _DecimalN and _FloatN
   arithmetic. */
extern uint8_t __isnanf_test(float);
#define __isnanf __isnanf_test
extern uint16_t __isnan_test(double);
#define __isnan __isnan_test
extern uint32_t __isnanl_test(long double);
#define __isnanl __isnanl_test
extern int16_t __isnand32_test(_Decimal32);
#define __isnand32 __isnand32_test
extern int32_t __isnand64_test(_Decimal64);
#define __isnand64 __isnand64_test
extern int64_t __isnand128_test(_Decimal128);
#define __isnand128 __isnand128_test

extern uint8_t __isfinitef_test(float);
#define __isfinitef __isfinitef_test
extern uint16_t __isfinite_test(double);
#define __isfinite __isfinite_test
extern uint32_t __isfinitel_test(long double);
#define __isfinitel __isfinitel_test
extern int16_t __isfinited32_test(_Decimal32);
#define __isfinited32 __isfinited32_test
extern int32_t __isfinited64_test(_Decimal64);
#define __isfinited64 __isfinited64_test
extern int64_t __isfinited128_test(_Decimal128);
#define __isfinited128 __isfinited128_test

extern uint8_t __signbitf_test(float);
#define __signbitf __signbitf_test
extern uint16_t __signbit_test(double);
#define __signbit __signbit_test
extern uint32_t __signbitl_test(long double);
#define __signbitl __signbitl_test
extern int16_t __signbitd32_test(_Decimal32);
#define __signbitd32 __signbitd32_test
extern int32_t __signbitd64_test(_Decimal64);
#define __signbitd64 __signbitd64_test
extern int64_t __signbitd128_test(_Decimal128);
#define __signbitd128 __signbitd128_test

extern uint8_t __fpclassifyf_test(float);
#define __fpclassifyf __fpclassifyf_test
extern uint16_t __fpclassify_test(double);
#define __fpclassify __fpclassify_test
extern uint32_t __fpclassifyl_test(long double);
#define __fpclassifyl __fpclassifyl_test
extern int16_t __fpclassifyd32_test(_Decimal32);
#define __fpclassifyd32 __fpclassifyd32_test
extern int32_t __fpclassifyd64_test(_Decimal64);
#define __fpclassifyd64 __fpclassifyd64_test
extern int64_t __fpclassifyd128_test(_Decimal128);
#define __fpclassifyd128 __fpclassifyd128_test

extern uint8_t __isinff_test(float);
#define __isinff __isinff_test
extern uint16_t __isinf_test(double);
#define __isinf __isinf_test
extern uint32_t __isinfl_test(long double);
#define __isinfl __isinfl_test
extern int16_t __isinfd32_test(_Decimal32);
#define __isinfd32 __isinfd32_test
extern int32_t __isinfd64_test(_Decimal64);
#define __isinfd64 __isinfd64_test
extern int64_t __isinfd128_test(_Decimal128);
#define __isinfd128 __isinfd128_test

extern uint8_t __issignalingf_test(float);
#define __issignalingf __issignalingf_test
extern uint16_t __issignaling_test(double);
#define __issignaling __issignaling_test
extern uint32_t __issignalingl_test(long double);
#define __issignalingl __issignalingl_test
extern int16_t __issignalingd32_test(_Decimal32);
#define __issignalingd32 __issignalingd32_test
extern int32_t __issignalingd64_test(_Decimal64);
#define __issignalingd64 __issignalingd64_test
extern int64_t __issignalingd128_test(_Decimal128);
#define __issignalingd128 __issignalingd128_test

#define typenum(x) (_Generic(x, int8_t : 1, int16_t : 2, int32_t : 3, uint8_t : 4, uint16_t : 5, uint32_t : 6, default: 0 ))

#define test(macro,val,__type) \
    type = typenum(macro(val)); \
    etype = typenum((__type)0); \
    if (type != typenum((__type)0)) \
      { printf( "Failed: " #macro " (" #val ") not typed as " #__type " (%d != %d)\n", type, etype); status++; }

#define do_test(macro) \
   test (macro, 2.0f, float); \
   test (macro, 2.0, double); \
   test (macro, 2.0l, long double); \
   test (macro, 2.0D, _Decimal32); \
   test (macro, 2.0DD, _Decimal64); \
   test (macro, 2.0DL, _Decimal128);

int
main(int a, char ** v)
{
#if __GNUC__ > 4 || ((__GNUC__ == 4 ) && (__GNUC_MINOR > 8))
  int status = 0;
  int type;
  int etype;

  (void) a;
  (void) v;

  do_test (fpclassify);
  do_test (signbit);
  do_test (isfinite);
  /* do_test (isnormal); The macro is really a test of fpclassify today */
  do_test (isinf);
  do_test (isnan);
  do_test (issignaling);
  return status ? EXIT_FAILURE : EXIT_SUCCESS;
#else
  fprintf (stderr, "This test requires C11 _Generic (aka >= GCC 4.9)\n");
  return EXIT_SUCCESS;
#endif
}
