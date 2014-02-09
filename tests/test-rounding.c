/* Test rounding functions: ceil, floor, nearbyint, round, trunc.

   Copyright (C) 2014 Free Software Foundation, Inc.

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

#ifndef __STDC_WANT_DEC_FP__
# define __STDC_WANT_DEC_FP__
#endif

#include <float.h>		/* DEC_NAN definition.  */
#include <stdio.h>
#include <math.h>

#define _WANT_VC 1
#include "scaffold.c"

typedef struct
{
  int line;
  _Decimal128 x;		/* Input.  */
  _Decimal128 e;		/* Expected should be this.  */
  const char *f;
} d128_type;

/* Test ceild128 function  */
static const d128_type d128_ceil[] = {
  {__LINE__, DEC_INFINITY, DEC_INFINITY, "%.34DDgDL"},
  {__LINE__, -DEC_INFINITY, -DEC_INFINITY, "%.34DDgDL"},
  {__LINE__, DEC_NAN, DEC_NAN, "%.34DDgDL"},
  {__LINE__, 0.0DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, -0.0DL, -0.0DL, "%.34DDgDL"},
  {__LINE__, M_PIdl, 4.0DL, "%.34DDgDL"},
  {__LINE__, -M_PIdl, -3.0DL, "%.34DDgDL"},
  {__LINE__, 0.1DL, 1.0DL, "%.34DDgDL"},
  {__LINE__, 0.25DL, 1.0DL, "%.34DDgDL"},
  {__LINE__, 0.625DL, 1.0DL, "%.34DDgDL"},
  {__LINE__, -0.1DL, -0.0DL, "%.34DDgDL"},
  {__LINE__, -0.25DL, -0.0DL, "%.34DDgDL"},
  {__LINE__, -0.625DL, -0.0DL, "%.34DDgDL"},
  {__LINE__, 2097152.5DL, 2097153.0DL, "%.34DDgDL"},
  {__LINE__, -2097152.5DL, -2097152.0DL, "%.34DDgDL"},
  {__LINE__, -4503599627370495.5DL, -4503599627370495.0DL, "%.34DDgDL"},
  {__LINE__, -4503599627370496.25DL, -4503599627370496.0DL, "%.34DDgDL"},
  {__LINE__, -4503599627370496.5DL, -4503599627370496.0DL, "%.34DDgDL"},
  {__LINE__, -4503599627370496.75DL, -4503599627370496.0DL, "%.34DDgDL"},
  {__LINE__, -4503599627370497.5DL, -4503599627370497.0DL, "%.34DDgDL"},
};
static const int d128_ceil_size =
  sizeof (d128_ceil) / sizeof (d128_ceil[0]);


/* Test floord128 function  */
static const d128_type d128_floor[] = {
  {__LINE__, DEC_INFINITY, DEC_INFINITY, "%.34DDgDL"},
  {__LINE__, -DEC_INFINITY, -DEC_INFINITY, "%.34DDgDL"},
  {__LINE__, DEC_NAN, DEC_NAN, "%.34DDgDL"},
  {__LINE__, 0.0DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, -0.0DL, -0.0DL, "%.34DDgDL"},
  {__LINE__, 0.2DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, -0.2DL, -1.0DL, "%.34DDgDL"},
  {__LINE__, 0.5DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, -0.5DL, -1.0DL, "%.34DDgDL"},
  {__LINE__, 0.8DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, -0.8DL, -1.0DL, "%.34DDgDL"},
  {__LINE__, 1.5DL, 1.0DL, "%.34DDgDL"},
  {__LINE__, -1.5DL, -2.0DL, "%.34DDgDL"},
  {__LINE__, 0.1DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, 0.25DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, 0.625DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, -0.1DL, -1.0DL, "%.34DDgDL"},
  {__LINE__, -0.25DL, -1.0DL, "%.34DDgDL"},
  {__LINE__, -0.625DL, -1.0DL, "%.34DDgDL"},
  {__LINE__, 2097152.5DL, 2097152.0DL, "%.34DDgDL"},
  {__LINE__, -2097152.5DL, -2097153.0DL, "%.34DDgDL"},
  {__LINE__, -4503599627370495.5DL, -4503599627370496.0DL, "%.34DDgDL"},
  {__LINE__, -4503599627370496.25DL, -4503599627370497.0DL, "%.34DDgDL"},
  {__LINE__, -4503599627370496.5DL, -4503599627370497.0DL, "%.34DDgDL"},
  {__LINE__, -4503599627370496.75DL, -4503599627370497.0DL, "%.34DDgDL"},
  {__LINE__, -4503599627370497.5DL, -4503599627370498.0DL, "%.34DDgDL"},
};
static const int d128_floor_size =
  sizeof (d128_floor) / sizeof (d128_floor[0]);

/* Test nearbyintd128 function  */
static const d128_type d128_nearbyint[] = {
  {__LINE__, DEC_INFINITY, DEC_INFINITY, "%.34DDgDL"},
  {__LINE__, -DEC_INFINITY, -DEC_INFINITY, "%.34DDgDL"},
  {__LINE__, DEC_NAN, DEC_NAN, "%.34DDgDL"},
  {__LINE__, 0.0DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, -0.0DL, -0.0DL, "%.34DDgDL"},
  {__LINE__, 0.2DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, -0.2DL, -0.0DL, "%.34DDgDL"},
  {__LINE__, 0.5DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, -0.5DL, -0.0DL, "%.34DDgDL"},
  {__LINE__, 0.8DL, 1.0DL, "%.34DDgDL"},
  {__LINE__, -0.8DL, -1.0DL, "%.34DDgDL"},
  {__LINE__, 1.5DL, 2.0DL, "%.34DDgDL"},
  {__LINE__, -1.5DL, -2.0DL, "%.34DDgDL"},
  {__LINE__, 0.1DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, 0.25DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, 0.625DL, 1.0DL, "%.34DDgDL"},
  {__LINE__, -0.1DL, -0.0DL, "%.34DDgDL"},
  {__LINE__, -0.25DL, -0.0DL, "%.34DDgDL"},
  {__LINE__, -0.625DL, -1.0DL, "%.34DDgDL"},
  {__LINE__, 1048576.75DL, 1048577.0DL, "%.34DDgDL"},
  {__LINE__, 2097152.75DL, 2097153.0DL, "%.34DDgDL"},
  {__LINE__, 2492472.75DL, 2492473.0DL, "%.34DDgDL"},
  {__LINE__, 2886220.75DL, 2886221.0DL, "%.34DDgDL"},
  {__LINE__, 3058792.75DL, 3058793.0DL, "%.34DDgDL"},
  {__LINE__, -1048576.75DL, -1048577.0DL, "%.34DDgDL"},
  {__LINE__, -2097152.75DL, -2097153.0DL, "%.34DDgDL"},
  {__LINE__, -2492472.75DL, -2492473.0DL, "%.34DDgDL"},
  {__LINE__, -2886220.75DL, -2886221.0DL, "%.34DDgDL"},
  {__LINE__, -3058792.75DL, -3058793.0DL, "%.34DDgDL"},
  {__LINE__, 70368744177664.75DL, 70368744177665.0DL, "%.34DDgDL"},
  {__LINE__, 140737488355328.75DL, 140737488355329.0DL, "%.34DDgDL"},
  {__LINE__, 281474976710656.75DL, 281474976710657.0DL, "%.34DDgDL"},
  {__LINE__, 562949953421312.75DL, 562949953421313.0DL, "%.34DDgDL"},
  {__LINE__, 1125899906842624.75DL, 1125899906842625.0DL, "%.34DDgDL"},
  {__LINE__, -70368744177664.75DL, -70368744177665.0DL, "%.34DDgDL"},
  {__LINE__, -140737488355328.75DL, -140737488355329.0DL, "%.34DDgDL"},
  {__LINE__, -281474976710656.75DL, -281474976710657.0DL, "%.34DDgDL"},
  {__LINE__, -562949953421312.75DL, -562949953421313.0DL, "%.34DDgDL"},
  {__LINE__, -1125899906842624.75DL, -1125899906842625.0DL, "%.34DDgDL"},
};
static const int d128_nearbyint_size =
  sizeof (d128_nearbyint) / sizeof (d128_nearbyint[0]);

/* Test roundd128 function  */
static const d128_type d128_round[] = {
  {__LINE__, DEC_INFINITY, DEC_INFINITY, "%.34DDgDL"},
  {__LINE__, -DEC_INFINITY, -DEC_INFINITY, "%.34DDgDL"},
  {__LINE__, DEC_NAN, DEC_NAN, "%.34DDgDL"},
  {__LINE__, 0.0DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, -0.0DL, -0.0DL, "%.34DDgDL"},
  {__LINE__, 0.2DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, -0.2DL, -0.0DL, "%.34DDgDL"},
  {__LINE__, 0.5DL, 1.0DL, "%.34DDgDL"},
  {__LINE__, -0.5DL, -1.0DL, "%.34DDgDL"},
  {__LINE__, 0.8DL, 1.0DL, "%.34DDgDL"},
  {__LINE__, -0.8DL, -1.0DL, "%.34DDgDL"},
  {__LINE__, 1.5DL, 2.0DL, "%.34DDgDL"},
  {__LINE__, -1.5DL, -2.0DL, "%.34DDgDL"},
  {__LINE__, 0.1DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, 0.25DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, 0.625DL, 1.0DL, "%.34DDgDL"},
  {__LINE__, -0.1DL, -0.0DL, "%.34DDgDL"},
  {__LINE__, -0.25DL, -0.0DL, "%.34DDgDL"},
  {__LINE__, -0.625DL, -1.0DL, "%.34DDgDL"},
  {__LINE__, 2097152.5DL, 2097153.0DL, "%.34DDgDL"},
  {__LINE__, -2097152.5DL, -2097153.0DL, "%.34DDgDL"},
  {__LINE__, -4503599627370495.5DL, -4503599627370496.0DL, "%.34DDgDL"},
  {__LINE__, -4503599627370496.25DL, -4503599627370496.0DL, "%.34DDgDL"},
  {__LINE__, -4503599627370496.5DL, -4503599627370497.0DL, "%.34DDgDL"},
  {__LINE__, -4503599627370496.75DL, -4503599627370497.0DL, "%.34DDgDL"},
  {__LINE__, -4503599627370497.5DL, -4503599627370498.0DL, "%.34DDgDL"},
};
static const int d128_round_size =
  sizeof (d128_round) / sizeof (d128_round[0]);

/* Test roundd128 function  */
static const d128_type d128_roundeven[] = {
  {__LINE__, DEC_INFINITY, DEC_INFINITY, "%.34DDgDL"},
  {__LINE__, -DEC_INFINITY, -DEC_INFINITY, "%.34DDgDL"},
  {__LINE__, DEC_NAN, DEC_NAN, "%.34DDgDL"},
  {__LINE__, 0.0DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, -0.0DL, -0.0DL, "%.34DDgDL"},
  {__LINE__, 0.2DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, -0.2DL, -0.0DL, "%.34DDgDL"},
  {__LINE__, 0.5DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, -0.5DL, -0.0DL, "%.34DDgDL"},
  {__LINE__, 0.8DL, 1.0DL, "%.34DDgDL"},
  {__LINE__, -0.8DL, -1.0DL, "%.34DDgDL"},
  {__LINE__, 1.5DL, 2.0DL, "%.34DDgDL"},
  {__LINE__, -1.5DL, -2.0DL, "%.34DDgDL"},
  {__LINE__, 0.1DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, 0.25DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, 0.625DL, 1.0DL, "%.34DDgDL"},
  {__LINE__, -0.1DL, -0.0DL, "%.34DDgDL"},
  {__LINE__, -0.25DL, -0.0DL, "%.34DDgDL"},
  {__LINE__, -0.625DL, -1.0DL, "%.34DDgDL"},
  {__LINE__, 2097152.5DL, 2097152.0DL, "%.34DDgDL"},
  {__LINE__, -2097152.5DL, -2097152.0DL, "%.34DDgDL"},
  {__LINE__, -4503599627370495.5DL, -4503599627370496.0DL, "%.34DDgDL"},
  {__LINE__, -4503599627370496.25DL, -4503599627370496.0DL, "%.34DDgDL"},
  {__LINE__, -4503599627370496.5DL, -4503599627370496.0DL, "%.34DDgDL"},
  {__LINE__, -4503599627370496.75DL, -4503599627370497.0DL, "%.34DDgDL"},
  {__LINE__, -4503599627370497.5DL, -4503599627370498.0DL, "%.34DDgDL"},
};
static const int d128_roundeven_size =
  sizeof (d128_roundeven) / sizeof (d128_roundeven[0]);


/* Test truncd128 function  */
static const d128_type d128_trunc[] = {
  {__LINE__, DEC_INFINITY, DEC_INFINITY, "%.34DDgDL"},
  {__LINE__, -DEC_INFINITY, -DEC_INFINITY, "%.34DDgDL"},
  {__LINE__, DEC_NAN, DEC_NAN, "%.34DDgDL"},
  {__LINE__, 0.0DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, -0.0DL, -0.0DL, "%.34DDgDL"},
  {__LINE__, 0.2DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, -0.2DL, -0.0DL, "%.34DDgDL"},
  {__LINE__, 0.5DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, -0.5DL, -0.0DL, "%.34DDgDL"},
  {__LINE__, 0.8DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, -0.8DL, -0.0DL, "%.34DDgDL"},
  {__LINE__, 1.5DL, 1.0DL, "%.34DDgDL"},
  {__LINE__, -1.5DL, -1.0DL, "%.34DDgDL"},
  {__LINE__, 0.1DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, 0.25DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, 0.625DL, 0.0DL, "%.34DDgDL"},
  {__LINE__, -0.1DL, -0.0DL, "%.34DDgDL"},
  {__LINE__, -0.25DL, -0.0DL, "%.34DDgDL"},
  {__LINE__, -0.625DL, -0.0DL, "%.34DDgDL"},
  {__LINE__, 2097152.5DL, 2097152.0DL, "%.34DDgDL"},
  {__LINE__, -2097152.5DL, -2097152.0DL, "%.34DDgDL"},
  {__LINE__, -4503599627370495.5DL, -4503599627370495.0DL, "%.34DDgDL"},
  {__LINE__, -4503599627370496.25DL, -4503599627370496.0DL, "%.34DDgDL"},
  {__LINE__, -4503599627370496.5DL, -4503599627370496.0DL, "%.34DDgDL"},
  {__LINE__, -4503599627370496.75DL, -4503599627370496.0DL, "%.34DDgDL"},
  {__LINE__, -4503599627370497.5DL, -4503599627370497.0DL, "%.34DDgDL"},
};
static const int d128_trunc_size =
  sizeof (d128_trunc) / sizeof (d128_trunc[0]);


typedef struct
{
  int line;
  _Decimal64 x;			/* Input.  */
  _Decimal64 e;			/* Expected should be this.  */
  const char *f;
} d64_type;

/* Test ceild64 function  */
static const d64_type d64_ceil[] = {
  {__LINE__, DEC_INFINITY, DEC_INFINITY, "%.16DgDD"},
  {__LINE__, -DEC_INFINITY, -DEC_INFINITY, "%.16DgDD"},
  {__LINE__, DEC_NAN, DEC_NAN, "%.16DgDD"},
  {__LINE__, 0.0DD, 0.0DD, "%.16DgDD"},
  {__LINE__, -0.0DD, -0.0DD, "%.16DgDD"},
  {__LINE__, M_PIdl, 4.0DD, "%.16DgDD"},
  {__LINE__, -M_PIdl, -3.0DD, "%.16DgDD"},
  {__LINE__, 0.1DD, 1.0DD, "%.16DgDD"},
  {__LINE__, 0.25DD, 1.0DD, "%.16DgDD"},
  {__LINE__, 0.625DD, 1.0DD, "%.16DgDD"},
  {__LINE__, -0.1DD, -0.0DD, "%.16DgDD"},
  {__LINE__, -0.25DD, -0.0DD, "%.16DgDD"},
  {__LINE__, -0.625DD, -0.0DD, "%.16DgDD"},
  {__LINE__, 2097152.5DD, 2097153.0DD, "%.16DgDD"},
  {__LINE__, -2097152.5DD, -2097152.0DD, "%.16DgDD"},
  {__LINE__, -3599627370495.5DD, -3599627370495.0DD, "%.16DgDD"},
  {__LINE__, -3599627370496.25DD, -3599627370496.0DD, "%.16DgDD"},
  {__LINE__, -3599627370496.5DD, -3599627370496.0DD, "%.16DgDD"},
  {__LINE__, -3599627370496.75DD, -3599627370496.0DD, "%.16DgDD"},
  {__LINE__, -3599627370497.5DD, -3599627370497.0DD, "%.16DgDD"},
};
static const int d64_ceil_size =
  sizeof (d64_ceil) / sizeof (d64_ceil[0]);


/* Test floord64 function  */
static const d64_type d64_floor[] = {
  {__LINE__, DEC_INFINITY, DEC_INFINITY, "%.16DgDD"},
  {__LINE__, -DEC_INFINITY, -DEC_INFINITY, "%.16DgDD"},
  {__LINE__, DEC_NAN, DEC_NAN, "%.16DgDD"},
  {__LINE__, 0.0DD, 0.0DD, "%.16DgDD"},
  {__LINE__, -0.0DD, -0.0DD, "%.16DgDD"},
  {__LINE__, 0.2DD, 0.0DD, "%.16DgDD"},
  {__LINE__, -0.2DD, -1.0DD, "%.16DgDD"},
  {__LINE__, 0.5DD, 0.0DD, "%.16DgDD"},
  {__LINE__, -0.5DD, -1.0DD, "%.16DgDD"},
  {__LINE__, 0.8DD, 0.0DD, "%.16DgDD"},
  {__LINE__, -0.8DD, -1.0DD, "%.16DgDD"},
  {__LINE__, 1.5DD, 1.0DD, "%.16DgDD"},
  {__LINE__, -1.5DD, -2.0DD, "%.16DgDD"},
  {__LINE__, 0.1DD, 0.0DD, "%.16DgDD"},
  {__LINE__, 0.25DD, 0.0DD, "%.16DgDD"},
  {__LINE__, 0.625DD, 0.0DD, "%.16DgDD"},
  {__LINE__, -0.1DD, -1.0DD, "%.16DgDD"},
  {__LINE__, -0.25DD, -1.0DD, "%.16DgDD"},
  {__LINE__, -0.625DD, -1.0DD, "%.16DgDD"},
  {__LINE__, 2097152.5DD, 2097152.0DD, "%.16DgDD"},
  {__LINE__, -2097152.5DD, -2097153.0DD, "%.16DgDD"},
  {__LINE__, -3599627370495.5DD, -3599627370496.0DD, "%.16DgDD"},
  {__LINE__, -3599627370496.25DD, -3599627370497.0DD, "%.16DgDD"},
  {__LINE__, -3599627370496.5DD, -3599627370497.0DD, "%.16DgDD"},
  {__LINE__, -3599627370496.75DD, -3599627370497.0DD, "%.16DgDD"},
  {__LINE__, -3599627370497.5DD, -3599627370498.0DD, "%.16DgDD"},
};
static const int d64_floor_size =
  sizeof (d64_floor) / sizeof (d64_floor[0]);

/* Test nearbyintd64 function  */
static const d64_type d64_nearbyint[] = {
  {__LINE__, DEC_INFINITY, DEC_INFINITY, "%.16DgDD"},
  {__LINE__, -DEC_INFINITY, -DEC_INFINITY, "%.16DgDD"},
  {__LINE__, DEC_NAN, DEC_NAN, "%.16DgDD"},
  {__LINE__, 0.0DD, 0.0DD, "%.16DgDD"},
  {__LINE__, -0.0DD, -0.0DD, "%.16DgDD"},
  {__LINE__, 0.2DD, 0.0DD, "%.16DgDD"},
  {__LINE__, -0.2DD, -0.0DD, "%.16DgDD"},
  {__LINE__, 0.5DD, 0.0DD, "%.16DgDD"},
  {__LINE__, -0.5DD, -0.0DD, "%.16DgDD"},
  {__LINE__, 0.8DD, 1.0DD, "%.16DgDD"},
  {__LINE__, -0.8DD, -1.0DD, "%.16DgDD"},
  {__LINE__, 1.5DD, 2.0DD, "%.16DgDD"},
  {__LINE__, -1.5DD, -2.0DD, "%.16DgDD"},
  {__LINE__, 0.1DD, 0.0DD, "%.16DgDD"},
  {__LINE__, 0.25DD, 0.0DD, "%.16DgDD"},
  {__LINE__, 0.625DD, 1.0DD, "%.16DgDD"},
  {__LINE__, -0.1DD, -0.0DD, "%.16DgDD"},
  {__LINE__, -0.25DD, -0.0DD, "%.16DgDD"},
  {__LINE__, -0.625DD, -1.0DD, "%.16DgDD"},
  {__LINE__, 1048576.75DD, 1048577.0DD, "%.16DgDD"},
  {__LINE__, 2097152.75DD, 2097153.0DD, "%.16DgDD"},
  {__LINE__, 2492472.75DD, 2492473.0DD, "%.16DgDD"},
  {__LINE__, 2886220.75DD, 2886221.0DD, "%.16DgDD"},
  {__LINE__, 3058792.75DD, 3058793.0DD, "%.16DgDD"},
  {__LINE__, -1048576.75DD, -1048577.0DD, "%.16DgDD"},
  {__LINE__, -2097152.75DD, -2097153.0DD, "%.16DgDD"},
  {__LINE__, -2492472.75DD, -2492473.0DD, "%.16DgDD"},
  {__LINE__, -2886220.75DD, -2886221.0DD, "%.16DgDD"},
  {__LINE__, -3058792.75DD, -3058793.0DD, "%.16DgDD"},
  {__LINE__, 68744177664.75DD, 68744177665.0DD, "%.16DgDD"},
  {__LINE__, 737488355328.75DD, 737488355329.0DD, "%.16DgDD"},
  {__LINE__, 474976710656.75DD, 474976710657.0DD, "%.16DgDD"},
  {__LINE__, 949953421312.75DD, 949953421313.0DD, "%.16DgDD"},
  {__LINE__, 5899906842624.75DD, 5899906842625.0DD, "%.16DgDD"},
  {__LINE__, -68744177664.75DD, -68744177665.0DD, "%.16DgDD"},
  {__LINE__, -737488355328.75DD, -737488355329.0DD, "%.16DgDD"},
  {__LINE__, -474976710656.75DD, -474976710657.0DD, "%.16DgDD"},
  {__LINE__, -949953421312.75DD, -949953421313.0DD, "%.16DgDD"},
  {__LINE__, -899906842624.75DD, -899906842625.0DD, "%.16DgDD"},
};
static const int d64_nearbyint_size =
  sizeof (d64_nearbyint) / sizeof (d64_nearbyint[0]);

/* Test round64 function  */
static const d64_type d64_round[] = {
  {__LINE__, DEC_INFINITY, DEC_INFINITY, "%.16DgDD"},
  {__LINE__, -DEC_INFINITY, -DEC_INFINITY, "%.16DgDD"},
  {__LINE__, DEC_NAN, DEC_NAN, "%.16DgDD"},
  {__LINE__, 0.0DD, 0.0DD, "%.16DgDD"},
  {__LINE__, -0.0DD, -0.0DD, "%.16DgDD"},
  {__LINE__, 0.2DD, 0.0DD, "%.16DgDD"},
  {__LINE__, -0.2DD, -0.0DD, "%.16DgDD"},
  {__LINE__, 0.5DD, 1.0DD, "%.16DgDD"},
  {__LINE__, -0.5DD, -1.0DD, "%.16DgDD"},
  {__LINE__, 0.8DD, 1.0DD, "%.16DgDD"},
  {__LINE__, -0.8DD, -1.0DD, "%.16DgDD"},
  {__LINE__, 1.5DD, 2.0DD, "%.16DgDD"},
  {__LINE__, -1.5DD, -2.0DD, "%.16DgDD"},
  {__LINE__, 0.1DD, 0.0DD, "%.16DgDD"},
  {__LINE__, 0.25DD, 0.0DD, "%.16DgDD"},
  {__LINE__, 0.625DD, 1.0DD, "%.16DgDD"},
  {__LINE__, -0.1DD, -0.0DD, "%.16DgDD"},
  {__LINE__, -0.25DD, -0.0DD, "%.16DgDD"},
  {__LINE__, -0.625DD, -1.0DD, "%.16DgDD"},
  {__LINE__, 2097152.5DD, 2097153.0DD, "%.16DgDD"},
  {__LINE__, -2097152.5DD, -2097153.0DD, "%.16DgDD"},
  {__LINE__, -3599627370495.5DD, -3599627370496.0DD, "%.16DgDD"},
  {__LINE__, -3599627370496.25DD, -3599627370496.0DD, "%.16DgDD"},
  {__LINE__, -3599627370496.5DD, -3599627370497.0DD, "%.16DgDD"},
  {__LINE__, -3599627370496.75DD, -3599627370497.0DD, "%.16DgDD"},
  {__LINE__, -3599627370497.5DD, -3599627370498.0DD, "%.16DgDD"},
};
static const int d64_round_size =
  sizeof (d64_round) / sizeof (d64_round[0]);

/* Test roundeven64 function  */
static const d64_type d64_roundeven[] = {
  {__LINE__, DEC_INFINITY, DEC_INFINITY, "%.16DgDD"},
  {__LINE__, -DEC_INFINITY, -DEC_INFINITY, "%.16DgDD"},
  {__LINE__, DEC_NAN, DEC_NAN, "%.16DgDD"},
  {__LINE__, 0.0DD, 0.0DD, "%.16DgDD"},
  {__LINE__, -0.0DD, -0.0DD, "%.16DgDD"},
  {__LINE__, 0.2DD, 0.0DD, "%.16DgDD"},
  {__LINE__, -0.2DD, -0.0DD, "%.16DgDD"},
  {__LINE__, 0.5DD, 0.0DD, "%.16DgDD"},
  {__LINE__, -0.5DD, -0.0DD, "%.16DgDD"},
  {__LINE__, 0.8DD, 1.0DD, "%.16DgDD"},
  {__LINE__, -0.8DD, -1.0DD, "%.16DgDD"},
  {__LINE__, 1.5DD, 2.0DD, "%.16DgDD"},
  {__LINE__, -1.5DD, -2.0DD, "%.16DgDD"},
  {__LINE__, 0.1DD, 0.0DD, "%.16DgDD"},
  {__LINE__, 0.25DD, 0.0DD, "%.16DgDD"},
  {__LINE__, 0.625DD, 1.0DD, "%.16DgDD"},
  {__LINE__, -0.1DD, -0.0DD, "%.16DgDD"},
  {__LINE__, -0.25DD, -0.0DD, "%.16DgDD"},
  {__LINE__, -0.625DD, -1.0DD, "%.16DgDD"},
  {__LINE__, 2097152.5DD, 2097152.0DD, "%.16DgDD"},
  {__LINE__, -2097152.5DD, -2097152.0DD, "%.16DgDD"},
  {__LINE__, -3599627370495.5DD, -3599627370496.0DD, "%.16DgDD"},
  {__LINE__, -3599627370496.25DD, -3599627370496.0DD, "%.16DgDD"},
  {__LINE__, -3599627370496.5DD, -3599627370496.0DD, "%.16DgDD"},
  {__LINE__, -3599627370496.75DD, -3599627370497.0DD, "%.16DgDD"},
  {__LINE__, -3599627370497.5DD, -3599627370498.0DD, "%.16DgDD"},
};
static const int d64_roundeven_size =
  sizeof (d64_roundeven) / sizeof (d64_roundeven[0]);

/* Test trunc64 function  */
static const d64_type d64_trunc[] = {
  {__LINE__, DEC_INFINITY, DEC_INFINITY, "%.16DgDD"},
  {__LINE__, -DEC_INFINITY, -DEC_INFINITY, "%.16DgDD"},
  {__LINE__, DEC_NAN, DEC_NAN, "%.16DgDD"},
  {__LINE__, 0.0DD, 0.0DD, "%.16DgDD"},
  {__LINE__, -0.0DD, -0.0DD, "%.16DgDD"},
  {__LINE__, 0.2DD, 0.0DD, "%.16DgDD"},
  {__LINE__, -0.2DD, -0.0DD, "%.16DgDD"},
  {__LINE__, 0.5DD, 0.0DD, "%.16DgDD"},
  {__LINE__, -0.5DD, -0.0DD, "%.16DgDD"},
  {__LINE__, 0.8DD, 0.0DD, "%.16DgDD"},
  {__LINE__, -0.8DD, -0.0DD, "%.16DgDD"},
  {__LINE__, 1.5DD, 1.0DD, "%.16DgDD"},
  {__LINE__, -1.5DD, -1.0DD, "%.16DgDD"},
  {__LINE__, 0.1DD, 0.0DD, "%.16DgDD"},
  {__LINE__, 0.25DD, 0.0DD, "%.16DgDD"},
  {__LINE__, 0.625DD, 0.0DD, "%.16DgDD"},
  {__LINE__, -0.1DD, -0.0DD, "%.16DgDD"},
  {__LINE__, -0.25DD, -0.0DD, "%.16DgDD"},
  {__LINE__, -0.625DD, -0.0DD, "%.16DgDD"},
  {__LINE__, 2097152.5DD, 2097152.0DD, "%.16DgDD"},
  {__LINE__, -2097152.5DD, -2097152.0DD, "%.16DgDD"},
  {__LINE__, -3599627370495.5DD, -3599627370495.0DD, "%.16DgDD"},
  {__LINE__, -3599627370496.25DD, -3599627370496.0DD, "%.16DgDD"},
  {__LINE__, -3599627370496.5DD, -3599627370496.0DD, "%.16DgDD"},
  {__LINE__, -3599627370496.75DD, -3599627370496.0DD, "%.16DgDD"},
  {__LINE__, -3599627370497.5DD, -3599627370497.0DD, "%.16DgDD"},
};
static const int d64_trunc_size =
  sizeof (d64_trunc) / sizeof (d64_trunc[0]);


typedef struct
{
  int line;
  _Decimal32 x;			/* Input.  */
  _Decimal32 e;			/* Expected should be this.  */
  const char *f;
} d32_type;

/* Test ceild32 function  */
static const d32_type d32_ceil[] = {
  {__LINE__, DEC_INFINITY, DEC_INFINITY, "%.7HgDF"},
  {__LINE__, -DEC_INFINITY, -DEC_INFINITY, "%.7HgDF"},
  {__LINE__, DEC_NAN, DEC_NAN, "%.7HgDF"},
  {__LINE__, 0.0DF, 0.0DF, "%.7HgDF"},
  {__LINE__, -0.0DF, -0.0DF, "%.7HgDF"},
  {__LINE__, M_PIdl, 4.0DF, "%.7HgDF"},
  {__LINE__, -M_PIdl, -3.0DF, "%.7HgDF"},
  {__LINE__, 0.1DF, 1.0DF, "%.7HgDF"},
  {__LINE__, 0.25DF, 1.0DF, "%.7HgDF"},
  {__LINE__, 0.625DF, 1.0DF, "%.7HgDF"},
  {__LINE__, -0.1DF, -0.0DF, "%.7HgDF"},
  {__LINE__, -0.25DF, -0.0DF, "%.7HgDF"},
  {__LINE__, -0.625DF, -0.0DF, "%.7HgDF"},
  {__LINE__, 97152.5DF, 97153.0DF, "%.7HgDF"},
  {__LINE__, -97152.5DF, -97152.0DF, "%.7HgDF"},
  {__LINE__, -370495.5DF, -370495.0DF, "%.7HgDF"},
  {__LINE__, -370496.25DF, -370496.0DF, "%.7HgDF"},
  {__LINE__, -370496.5DF, -370496.0DF, "%.7HgDF"},
  {__LINE__, -370496.75DF, -370496.0DF, "%.7HgDF"},
  {__LINE__, -370497.5DF, -370497.0DF, "%.7HgDF"},
};
static const int d32_ceil_size =
  sizeof (d32_ceil) / sizeof (d32_ceil[0]);

/* Test floord32 function  */
static const d32_type d32_floor[] = {
  {__LINE__, DEC_INFINITY, DEC_INFINITY, "%.7HgDF"},
  {__LINE__, -DEC_INFINITY, -DEC_INFINITY, "%.7HgDF"},
  {__LINE__, DEC_NAN, DEC_NAN, "%.7HgDF"},
  {__LINE__, 0.0DF, 0.0DF, "%.7HgDF"},
  {__LINE__, -0.0DF, -0.0DF, "%.7HgDF"},
  {__LINE__, 0.2DF, 0.0DF, "%.7HgDF"},
  {__LINE__, -0.2DF, -1.0DF, "%.7HgDF"},
  {__LINE__, 0.5DF, 0.0DF, "%.7HgDF"},
  {__LINE__, -0.5DF, -1.0DF, "%.7HgDF"},
  {__LINE__, 0.8DF, 0.0DF, "%.7HgDF"},
  {__LINE__, -0.8DF, -1.0DF, "%.7HgDF"},
  {__LINE__, 1.5DF, 1.0DF, "%.7HgDF"},
  {__LINE__, -1.5DF, -2.0DF, "%.7HgDF"},
  {__LINE__, 0.1DF, 0.0DF, "%.7HgDF"},
  {__LINE__, 0.25DF, 0.0DF, "%.7HgDF"},
  {__LINE__, 0.625DF, 0.0DF, "%.7HgDF"},
  {__LINE__, -0.1DF, -1.0DF, "%.7HgDF"},
  {__LINE__, -0.25DF, -1.0DF, "%.7HgDF"},
  {__LINE__, -0.625DF, -1.0DF, "%.7HgDF"},
  {__LINE__, 97152.5DF, 97152.0DF, "%.7HgDF"},
  {__LINE__, -97152.5DF, -97153.0DF, "%.7HgDF"},
  {__LINE__, -370495.5DF, -370496.0DF, "%.7HgDF"},
  {__LINE__, -370496.25DF, -370497.0DF, "%.7HgDF"},
  {__LINE__, -370496.5DF, -370497.0DF, "%.7HgDF"},
  {__LINE__, -370496.75DF, -370497.0DF, "%.7HgDF"},
  {__LINE__, -370497.5DF, -370498.0DF, "%.7HgDF"},
};
static const int d32_floor_size =
  sizeof (d32_floor) / sizeof (d32_floor[0]);

/* Test nearbyintd64 function  */
static const d32_type d32_nearbyint[] = {
  {__LINE__, DEC_INFINITY, DEC_INFINITY, "%.7HgDF"},
  {__LINE__, -DEC_INFINITY, -DEC_INFINITY, "%.7HgDF"},
  {__LINE__, DEC_NAN, DEC_NAN, "%.7HgDF"},
  {__LINE__, 0.0DF, 0.0DF, "%.7HgDF"},
  {__LINE__, -0.0DF, -0.0DF, "%.7HgDF"},
  {__LINE__, 0.2DF, 0.0DF, "%.7HgDF"},
  {__LINE__, -0.2DF, -0.0DF, "%.7HgDF"},
  {__LINE__, 0.5DF, 0.0DF, "%.7HgDF"},
  {__LINE__, -0.5DF, -0.0DF, "%.7HgDF"},
  {__LINE__, 0.8DF, 1.0DF, "%.7HgDF"},
  {__LINE__, -0.8DF, -1.0DF, "%.7HgDF"},
  {__LINE__, 1.5DF, 2.0DF, "%.7HgDF"},
  {__LINE__, -1.5DF, -2.0DF, "%.7HgDF"},
  {__LINE__, 0.1DF, 0.0DF, "%.7HgDF"},
  {__LINE__, 0.25DF, 0.0DF, "%.7HgDF"},
  {__LINE__, 0.625DF, 1.0DF, "%.7HgDF"},
  {__LINE__, -0.1DF, -0.0DF, "%.7HgDF"},
  {__LINE__, -0.25DF, -0.0DF, "%.7HgDF"},
  {__LINE__, -0.625DF, -1.0DF, "%.7HgDF"},
  {__LINE__, 48576.75DF, 48577.0DF, "%.7HgDF"},
  {__LINE__, 97152.75DF, 97153.0DF, "%.7HgDF"},
  {__LINE__, 92472.75DF, 92473.0DF, "%.7HgDF"},
  {__LINE__, 86220.75DF, 86221.0DF, "%.7HgDF"},
  {__LINE__, 58792.75DF, 58793.0DF, "%.7HgDF"},
  {__LINE__, -48576.75DF, -48577.0DF, "%.7HgDF"},
  {__LINE__, -97152.75DF, -97153.0DF, "%.7HgDF"},
  {__LINE__, -92472.75DF, -92473.0DF, "%.7HgDF"},
  {__LINE__, -86220.75DF, -86221.0DF, "%.7HgDF"},
  {__LINE__, -58792.75DF, -58793.0DF, "%.7HgDF"},
};
static const int d32_nearbyint_size =
  sizeof (d32_nearbyint) / sizeof (d32_nearbyint[0]);

/* Test roundd32 function  */
static const d32_type d32_round[] = {
  {__LINE__, DEC_INFINITY, DEC_INFINITY, "%.7HgDF"},
  {__LINE__, -DEC_INFINITY, -DEC_INFINITY, "%.7HgDF"},
  {__LINE__, DEC_NAN, DEC_NAN, "%.7HgDF"},
  {__LINE__, 0.0DF, 0.0DF, "%.7HgDF"},
  {__LINE__, -0.0DF, -0.0DF, "%.7HgDF"},
  {__LINE__, 0.2DF, 0.0DF, "%.7HgDF"},
  {__LINE__, -0.2DF, -0.0DF, "%.7HgDF"},
  {__LINE__, 0.5DF, 1.0DF, "%.7HgDF"},
  {__LINE__, -0.5DF, -1.0DF, "%.7HgDF"},
  {__LINE__, 0.8DF, 1.0DF, "%.7HgDF"},
  {__LINE__, -0.8DF, -1.0DF, "%.7HgDF"},
  {__LINE__, 1.5DF, 2.0DF, "%.7HgDF"},
  {__LINE__, -1.5DF, -2.0DF, "%.7HgDF"},
  {__LINE__, 0.1DF, 0.0DF, "%.7HgDF"},
  {__LINE__, 0.25DF, 0.0DF, "%.7HgDF"},
  {__LINE__, 0.625DF, 1.0DF, "%.7HgDF"},
  {__LINE__, -0.1DF, -0.0DF, "%.7HgDF"},
  {__LINE__, -0.25DF, -0.0DF, "%.7HgDF"},
  {__LINE__, -0.625DF, -1.0DF, "%.7HgDF"},
  {__LINE__, 97152.5DF, 97153.0DF, "%.7HgDF"},
  {__LINE__, -97152.5DF, -97153.0DF, "%.7HgDF"},
  {__LINE__, -370495.5DF, -370496.0DF, "%.7HgDF"},
  {__LINE__, -370496.25DF, -370496.0DF, "%.7HgDF"},
  {__LINE__, -370496.5DF, -370497.0DF, "%.7HgDF"},
  {__LINE__, -370496.75DF, -370497.0DF, "%.7HgDF"},
  {__LINE__, -370497.5DF, -370498.0DF, "%.7HgDF"},
};
static const int d32_round_size =
  sizeof (d32_round) / sizeof (d32_round[0]);

/* Test roundevend32 function  */
static const d32_type d32_roundeven[] = {
  {__LINE__, DEC_INFINITY, DEC_INFINITY, "%.7HgDF"},
  {__LINE__, -DEC_INFINITY, -DEC_INFINITY, "%.7HgDF"},
  {__LINE__, DEC_NAN, DEC_NAN, "%.7HgDF"},
  {__LINE__, 0.0DF, 0.0DF, "%.7HgDF"},
  {__LINE__, -0.0DF, -0.0DF, "%.7HgDF"},
  {__LINE__, 0.2DF, 0.0DF, "%.7HgDF"},
  {__LINE__, -0.2DF, -0.0DF, "%.7HgDF"},
  {__LINE__, 0.5DF, 0.0DF, "%.7HgDF"},
  {__LINE__, -0.5DF, -0.0DF, "%.7HgDF"},
  {__LINE__, 0.8DF, 1.0DF, "%.7HgDF"},
  {__LINE__, -0.8DF, -1.0DF, "%.7HgDF"},
  {__LINE__, 1.5DF, 2.0DF, "%.7HgDF"},
  {__LINE__, -1.5DF, -2.0DF, "%.7HgDF"},
  {__LINE__, 0.1DF, 0.0DF, "%.7HgDF"},
  {__LINE__, 0.25DF, 0.0DF, "%.7HgDF"},
  {__LINE__, 0.625DF, 1.0DF, "%.7HgDF"},
  {__LINE__, -0.1DF, -0.0DF, "%.7HgDF"},
  {__LINE__, -0.25DF, -0.0DF, "%.7HgDF"},
  {__LINE__, -0.625DF, -1.0DF, "%.7HgDF"},
  {__LINE__, 97152.5DF, 97152.0DF, "%.7HgDF"},
  {__LINE__, -97152.5DF, -97152.0DF, "%.7HgDF"},
  {__LINE__, -370495.5DF, -370496.0DF, "%.7HgDF"},
  {__LINE__, -370496.25DF, -370496.0DF, "%.7HgDF"},
  {__LINE__, -370496.5DF, -370496.0DF, "%.7HgDF"},
  {__LINE__, -370496.75DF, -370497.0DF, "%.7HgDF"},
  {__LINE__, -370497.5DF, -370498.0DF, "%.7HgDF"},
};
static const int d32_roundeven_size =
  sizeof (d32_roundeven) / sizeof (d32_roundeven[0]);

/* Test truncd32 function  */
static const d32_type d32_trunc[] = {
  {__LINE__, DEC_INFINITY, DEC_INFINITY, "%.7HgDF"},
  {__LINE__, -DEC_INFINITY, -DEC_INFINITY, "%.7HgDF"},
  {__LINE__, DEC_NAN, DEC_NAN, "%.7HgDF"},
  {__LINE__, 0.0DF, 0.0DF, "%.7HgDF"},
  {__LINE__, -0.0DF, -0.0DF, "%.7HgDF"},
  {__LINE__, 0.2DF, 0.0DF, "%.7HgDF"},
  {__LINE__, -0.2DF, -0.0DF, "%.7HgDF"},
  {__LINE__, 0.5DF, 0.0DF, "%.7HgDF"},
  {__LINE__, -0.5DF, -0.0DF, "%.7HgDF"},
  {__LINE__, 0.8DF, 0.0DF, "%.7HgDF"},
  {__LINE__, -0.8DF, -0.0DF, "%.7HgDF"},
  {__LINE__, 1.5DF, 1.0DF, "%.7HgDF"},
  {__LINE__, -1.5DF, -1.0DF, "%.7HgDF"},
  {__LINE__, 0.1DF, 0.0DF, "%.7HgDF"},
  {__LINE__, 0.25DF, 0.0DF, "%.7HgDF"},
  {__LINE__, 0.625DF, 0.0DF, "%.7HgDF"},
  {__LINE__, -0.1DF, -0.0DF, "%.7HgDF"},
  {__LINE__, -0.25DF, -0.0DF, "%.7HgDF"},
  {__LINE__, -0.625DF, -0.0DF, "%.7HgDF"},
  {__LINE__, 97152.5DF, 97152.0DF, "%.7HgDF"},
  {__LINE__, -97152.5DF, -97152.0DF, "%.7HgDF"},
  {__LINE__, -370495.5DF, -370495.0DF, "%.7HgDF"},
  {__LINE__, -370496.25DF, -370496.0DF, "%.7HgDF"},
  {__LINE__, -370496.5DF, -370496.0DF, "%.7HgDF"},
  {__LINE__, -370496.75DF, -370496.0DF, "%.7HgDF"},
  {__LINE__, -370497.5DF, -370497.0DF, "%.7HgDF"},
};
static const int d32_trunc_size =
  sizeof (d32_trunc) / sizeof (d32_trunc[0]);


int
main (void)
{
  int i;

  /* _Decimal128 tests.  */
  for (i = 0; i < d128_ceil_size; ++i)
    {
      _Decimal128 r = ceild128 (d128_ceil[i].x);
      printf ("ceil (%.34DDfDL) = %.34DDfDL) in %s:%d\n", d128_ceil[i].x, r,
	      __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d128_ceil[i].line, d128_ceil[i].e, r,
	     d128_ceil[i].f);
    }
  for (i = 0; i < d128_floor_size; ++i)
    {
      _Decimal128 r = floord128 (d128_floor[i].x);
      printf ("floor (%.34DDfDL) = %.34DDfDL) in %s:%d\n", d128_floor[i].x, r,
	      __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d128_floor[i].line, d128_floor[i].e, r,
	     d128_floor[i].f);
    }
  for (i = 0; i < d128_nearbyint_size; ++i)
    {
      _Decimal128 r = nearbyintd128 (d128_nearbyint[i].x);
      printf ("nearbyint (%.34DDfDL) = %.34DDfDL) in %s:%d\n", d128_nearbyint[i].x, r,
	      __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d128_nearbyint[i].line, d128_nearbyint[i].e, r,
	     d128_nearbyint[i].f);
    }
  for (i = 0; i < d128_round_size; ++i)
    {
      _Decimal128 r = roundd128 (d128_round[i].x);
      printf ("round (%.34DDfDL) = %.34DDfDL) in %s:%d\n", d128_round[i].x, r,
	      __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d128_round[i].line, d128_round[i].e, r,
	     d128_round[i].f);
    }
  for (i = 0; i < d128_roundeven_size; ++i)
    {
      _Decimal128 r = roundevend128 (d128_roundeven[i].x);
      printf ("roundeven (%.34DDfDL) = %.34DDfDL) in %s:%d\n", d128_roundeven[i].x, r,
	      __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d128_roundeven[i].line, d128_roundeven[i].e, r,
	     d128_roundeven[i].f);
    }
  for (i = 0; i < d128_trunc_size; ++i)
    {
      _Decimal128 r = truncd128 (d128_trunc[i].x);
      printf ("trunc (%.34DDfDL) = %.34DDfDL) in %s:%d\n", d128_trunc[i].x, r,
	      __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d128_trunc[i].line, d128_trunc[i].e, r,
	     d128_trunc[i].f);
    }

  /* _Decimal64 tests.  */
  for (i = 0; i < d64_ceil_size; ++i)
    {
      _Decimal64 r = ceild64 (d64_ceil[i].x);
      printf ("ceil (%.16DfDD) = %.16DfDD) in %s:%d\n", d64_ceil[i].x, r,
	      __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d64_ceil[i].line, d64_ceil[i].e, r,
	     d64_ceil[i].f);
    }
  for (i = 0; i < d64_floor_size; ++i)
    {
      _Decimal64 r = floord64 (d64_floor[i].x);
      printf ("floor (%.16DfDD) = %.16DfDD) in %s:%d\n", d64_floor[i].x, r,
	      __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d64_floor[i].line, d64_floor[i].e, r,
	     d64_floor[i].f);
    }
  for (i = 0; i < d64_nearbyint_size; ++i)
    {
      _Decimal64 r = nearbyintd64 (d64_nearbyint[i].x);
      printf ("nearbyint (%.16DfDD) = %.16DfDD) in %s:%d\n", d64_nearbyint[i].x, r,
	      __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d64_nearbyint[i].line, d64_nearbyint[i].e, r,
	     d64_nearbyint[i].f);
    }
  for (i = 0; i < d64_round_size; ++i)
    {
      _Decimal64 r = roundd64 (d64_round[i].x);
      printf ("round (%.16DfDD) = %.16DfDD) in %s:%d\n", d64_round[i].x, r,
	      __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d64_round[i].line, d64_round[i].e, r,
	     d64_round[i].f);
    }
  for (i = 0; i < d64_roundeven_size; ++i)
    {
      _Decimal64 r = roundevend64 (d64_roundeven[i].x);
      printf ("roundeven (%.16DfDD) = %.16DfDD) in %s:%d\n", d64_roundeven[i].x, r,
	      __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d64_roundeven[i].line, d64_roundeven[i].e, r,
	     d64_roundeven[i].f);
    }
  for (i = 0; i < d64_trunc_size; ++i)
    {
      _Decimal64 r = truncd64 (d64_trunc[i].x);
      printf ("trunc (%.16DfDD) = %.16DfDD) in %s:%d\n", d64_trunc[i].x, r,
	      __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d64_trunc[i].line, d64_trunc[i].e, r,
	     d64_trunc[i].f);
    }

  /* _Decimal32 tests.  */
  for (i = 0; i < d32_ceil_size; ++i)
    {
      _Decimal32 r = ceild32 (d32_ceil[i].x);
      printf ("ceil (%.9HfDF) = %.9HfDF) in %s:%d\n", d32_ceil[i].x, r,
	      __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d32_ceil[i].line, d32_ceil[i].e, r,
	     d32_ceil[i].f);
    }
  for (i = 0; i < d32_floor_size; ++i)
    {
      _Decimal32 r = floord32 (d32_floor[i].x);
      printf ("floor (%.9HfDF) = %.9HfDF) in %s:%d\n", d32_floor[i].x, r,
	      __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d32_floor[i].line, d32_floor[i].e, r,
	     d32_floor[i].f);
    }
  for (i = 0; i < d32_nearbyint_size; ++i)
    {
      _Decimal32 r = nearbyintd32 (d32_nearbyint[i].x);
      printf ("nearbyint (%.9HfDF) = %.9HfDF) in %s:%d\n", d32_nearbyint[i].x, r,
	      __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d32_nearbyint[i].line, d32_nearbyint[i].e, r,
	     d32_nearbyint[i].f);
    }
  for (i = 0; i < d32_round_size; ++i)
    {
      _Decimal32 r = roundd32 (d32_round[i].x);
      printf ("round (%.9HfDF) = %.9HfDF) in %s:%d\n", d32_round[i].x, r,
	      __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d32_round[i].line, d32_round[i].e, r,
	     d32_round[i].f);
    }
  for (i = 0; i < d32_roundeven_size; ++i)
    {
      _Decimal32 r = roundevend32 (d32_roundeven[i].x);
      printf ("roundeven (%.9HfDF) = %.9HfDF) in %s:%d\n", d32_roundeven[i].x, r,
	      __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d32_roundeven[i].line, d32_roundeven[i].e, r,
	     d32_roundeven[i].f);
    }
  for (i = 0; i < d32_trunc_size; ++i)
    {
      _Decimal32 r = truncd32 (d32_trunc[i].x);
      printf ("trunc (%.9HfDF) = %.9HfDF) in %s:%d\n", d32_trunc[i].x, r,
	      __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d32_trunc[i].line, d32_trunc[i].e, r,
	     d32_trunc[i].f);
    }

  _REPORT ();

  /* fail comes from scaffold.c  */
  return fail;
}
