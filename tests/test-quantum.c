/* Test rounding functions: quantumd[32|64|128].

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

/* Test quantumd128 function  */
static const d128_type d128_quantum[] = {
  {__LINE__, DEC_INFINITY, DEC_INFINITY, "%.34DDgDL"},
  {__LINE__, -DEC_INFINITY, DEC_INFINITY, "%.34DDgDL"},
  {__LINE__, DEC_NAN, DEC_NAN, "%.34DDgDL"},
  {__LINE__, __DEC128_SUBNORMAL_MIN__, 1e-6176DL, "%.34DDgDD"},
  {__LINE__, __DEC128_MIN__, 1e-6143DL, "%.34DDgDD"},
  {__LINE__, __DEC128_MAX__, 1e6111DL, "%.34DgDD"},
  {__LINE__, 1e1DL, 10.0DL, "%.32DDgDL"},
  {__LINE__, 1e0DL, 1.0DL, "%.32DDgDL"},
  {__LINE__, -0.25DL, 0.01DL, "%.34DDgDL"},
  {__LINE__, 0.625DL, 0.001DL, "%.34DDgDL"},
  {__LINE__, 2097152.5DL, 0.1DL, "%.34DDgDL"},
  {__LINE__, -2097152.5DL, 0.1DL, "%.34DDgDL"},
  {__LINE__, -4503599627370495.5DL, 0.1DL, "%.34DDgDL"},
  {__LINE__, 4503599627370.49625DL, 0.00001DL, "%.34DDgDL"},
  {__LINE__, -45035996.273704955DL, 1e-9DL, "%.34DDgDL"},
};
static const int d128_quantum_size =
  sizeof (d128_quantum) / sizeof (d128_quantum[0]);


typedef struct
{
  int line;
  _Decimal64 x;			/* Input.  */
  _Decimal64 e;			/* Expected should be this.  */
  const char *f;
} d64_type;

/* Test quantumd64 function  */
static const d64_type d64_quantum[] = {
  {__LINE__, DEC_INFINITY, DEC_INFINITY, "%.16DgDD"},
  {__LINE__, -DEC_INFINITY, DEC_INFINITY, "%.16DgDD"},
  {__LINE__, DEC_NAN, DEC_NAN, "%.16DgDD"},
  {__LINE__, __DEC64_SUBNORMAL_MIN__, 1e-398DD, "%.16DgDD"},
  {__LINE__, __DEC64_MIN__, 1e-383DD, "%.16DgDD"},
  {__LINE__, __DEC64_MAX__, 1e369DD, "%.16gDD"},
  {__LINE__, 1e1DD, 10.0DD, "%.16DgDD"},
  {__LINE__, 1e0DD, 1.0DD, "%.16DgDD"},
  {__LINE__, -0.1DD, 0.1DD, "%.16DgDD"},
  {__LINE__, -0.25DD, 0.01DD, "%.16DgDD"},
  {__LINE__, -0.625DD, 0.001DD, "%.16DgDD"},
  {__LINE__, 2097.1525DD, 0.0001DD, "%.16DgDD"},
  {__LINE__, -20.971525DD, 0.000001DD, "%.16DgDD"},
  {__LINE__, -3599.6273704955DD, 1e-10DD, "%.16DgDD"},
  {__LINE__, 359.962737049625DD, 1e-12DD, "%.16DgDD"},
};
static const int d64_quantum_size =
  sizeof (d64_quantum) / sizeof (d64_quantum[0]);


typedef struct
{
  int line;
  _Decimal32 x;			/* Input.  */
  _Decimal32 e;			/* Expected should be this.  */
  const char *f;
} d32_type;

/* Test quantumd32 function  */
static const d32_type d32_quantum[] = {
  {__LINE__, DEC_INFINITY, DEC_INFINITY, "%.7HgDF"},
  {__LINE__, -DEC_INFINITY, DEC_INFINITY, "%.7HgDF"},
  {__LINE__, DEC_NAN, DEC_NAN, "%.7HgDF"},
  {__LINE__, __DEC32_SUBNORMAL_MIN__, 1e-101DF, "%.7HgDF"},
  {__LINE__, __DEC32_MIN__, 1e-95DF, "%.7HgDF"},
  {__LINE__, __DEC32_MAX__, 1e90DF, "%.7HgDF"},
  {__LINE__, 1e1DF, 10.0DF, "%.16DgDD"},
  {__LINE__, 1e0DF, 1.0DF, "%.16DgDD"},
  {__LINE__, -0.1DF, 0.1DF, "%.7HgDF"},
  {__LINE__, -0.25DF, 0.01DF, "%.7HgDF"},
  {__LINE__, -0.625DF, 0.001DF, "%.7HgDF"},
  {__LINE__, 97.1525DF, 0.0001DF, "%.7HgDF"},
};
static const int d32_quantum_size =
  sizeof (d32_quantum) / sizeof (d32_quantum[0]);


int
main (void)
{
  int i;

  /* _Decimal128 tests.  */
  for (i = 0; i < d128_quantum_size; ++i)
    {
      _Decimal128 r = quantumd128 (d128_quantum[i].x);
      printf ("quantum (%.34DDgDL) = %.34DDgDL) in %s:%d\n", d128_quantum[i].x, r,
	      __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d128_quantum[i].line, d128_quantum[i].e, r,
	     d128_quantum[i].f);
    }

  /* _Decimal64 tests.  */
  for (i = 0; i < d64_quantum_size; ++i)
    {
      _Decimal64 r = quantumd64 (d64_quantum[i].x);
      printf ("quantum (%.16DgDD) = %.16DgDD) in %s:%d\n", d64_quantum[i].x, r,
	      __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d64_quantum[i].line, d64_quantum[i].e, r,
	     d64_quantum[i].f);
    }

  /* _Decimal32 tests.  */
  for (i = 0; i < d32_quantum_size; ++i)
    {
      _Decimal32 r = quantumd32 (d32_quantum[i].x);
      printf ("quantum (%.9HgDF) = %.9HgDF) in %s:%d\n", d32_quantum[i].x, r,
	      __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d32_quantum[i].line, d32_quantum[i].e, r,
	     d32_quantum[i].f);
    }

  _REPORT ();

  /* fail comes from scaffold.c  */
  return fail;
}
