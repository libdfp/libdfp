/* Test rounding functions: llquantexpd[32|64|128].

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
  long long int e;		/* Expected should be this.  */
  const char *f;
} d128_type;

/* Test llquantexpd128 function  */
static const d128_type d128_llquantexp[] = {
  {__LINE__, DEC_INFINITY, LLONG_MIN, "%lld"},
  {__LINE__, -DEC_INFINITY, LLONG_MIN, "%lld"},
  {__LINE__, DEC_NAN, LLONG_MIN, "%lld"},
  {__LINE__, __DEC128_SUBNORMAL_MIN__, -6176, "%lld"},
  {__LINE__, __DEC128_MIN__, -6143, "%lld"},
  {__LINE__, __DEC128_MAX__, 6111, "%lld"},
  {__LINE__, -0.25DL, -2, "%lld"},
  {__LINE__, 0.625DL, -3, "%lld"},
  {__LINE__, 2097152.5DL, -1, "%lld"},
  {__LINE__, -2097152.5DL, -1, "%lld"},
  {__LINE__, -4503599627370495.5DL, -1, "%lld"},
  {__LINE__, 4503599627370496.25DL, -2, "%lld"},
  {__LINE__, -45035996.273704955DL, -9, "%lld"},
};

static const int d128_llquantexp_size =
  sizeof (d128_llquantexp) / sizeof (d128_llquantexp[0]);


typedef struct
{
  int line;
  _Decimal64 x;			/* Input.  */
  long long int e;		/* Expected should be this.  */
  const char *f;
} d64_type;

/* Test llquantexpd64 function  */
static const d64_type d64_llquantexp[] = {
  {__LINE__, DEC_INFINITY, LLONG_MIN, "%lld"},
  {__LINE__, -DEC_INFINITY, LLONG_MIN, "%lld"},
  {__LINE__, DEC_NAN, LLONG_MIN, "%lld"},
  {__LINE__, __DEC64_SUBNORMAL_MIN__, -398, "%lld"},
  {__LINE__, __DEC64_MIN__, -383, "%lld"},
  {__LINE__, __DEC64_MAX__, 369, "%lld"},
  {__LINE__, -0.1DD, -1, "%lld"},
  {__LINE__, -0.25DD, -2, "%lld"},
  {__LINE__, -0.625DD, -3, "%lld"},
  {__LINE__, 2097.1525DD, -4, "%lld"},
  {__LINE__, -20.971525DD, -6, "%lld"},
  {__LINE__, -3599.6273704955DD, -10, "%lld"},
  {__LINE__, 359.962737049625DD, -12, "%lld"},
};

static const int d64_llquantexp_size =
  sizeof (d64_llquantexp) / sizeof (d64_llquantexp[0]);


typedef struct
{
  int line;
  _Decimal32 x;			/* Input.  */
  long long int e;		/* Expected should be this.  */
  const char *f;
} d32_type;

/* Test llquantexpd32 function  */
static const d32_type d32_llquantexp[] = {
  {__LINE__, DEC_INFINITY, LLONG_MIN, "%lld"},
  {__LINE__, -DEC_INFINITY, LLONG_MIN, "%lld"},
  {__LINE__, DEC_NAN, LLONG_MIN, "%lld"},
  {__LINE__, __DEC32_SUBNORMAL_MIN__, -101, "%lld"},
  {__LINE__, __DEC32_MIN__, -95, "%lld"},
  {__LINE__, __DEC32_MAX__, 90, "%lld"},
  {__LINE__, -0.1DF, -1, "%lld"},
  {__LINE__, -0.25DF, -2, "%lld"},
  {__LINE__, -0.625DF, -3, "%lld"},
  {__LINE__, 97.1525DF, -4, "%lld"},
};

static const int d32_llquantexp_size =
  sizeof (d32_llquantexp) / sizeof (d32_llquantexp[0]);


int
main (void)
{
  int i;

  /* _Decimal128 tests.  */
  for (i = 0; i < d128_llquantexp_size; ++i)
    {
      long long int r = llquantexpd128 (d128_llquantexp[i].x);
      printf ("llquantexp (%.34DDgDL) = %lld) in %s:%d\n",
	      d128_llquantexp[i].x, r, __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d128_llquantexp[i].line, d128_llquantexp[i].e, r,
	     d128_llquantexp[i].f);
    }

  /* _Decimal64 tests.  */
  for (i = 0; i < d64_llquantexp_size; ++i)
    {
      long long int r = llquantexpd64 (d64_llquantexp[i].x);
      printf ("llquantexp (%.16DgDD) = %lld) in %s:%d\n", d64_llquantexp[i].x,
	      r, __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d64_llquantexp[i].line, d64_llquantexp[i].e, r,
	     d64_llquantexp[i].f);
    }

  /* _Decimal32 tests.  */
  for (i = 0; i < d32_llquantexp_size; ++i)
    {
      long long int r = llquantexpd32 (d32_llquantexp[i].x);
      printf ("llquantexp (%.9HgDF) = %lld) in %s:%d\n", d32_llquantexp[i].x,
	      r, __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d32_llquantexp[i].line, d32_llquantexp[i].e, r,
	     d32_llquantexp[i].f);
    }

  _REPORT ();

  /* fail comes from scaffold.c  */
  return fail;
}
