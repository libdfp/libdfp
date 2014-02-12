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
  _Decimal128 x;		/* Input y  */
  _Decimal128 y;		/* Input x  */
  _Bool e;			/* Expected should be this.  */
  const char *f;
} d128_type;

/* Test quantumd128 function  */
static const d128_type d128_quantum[] = {
  {__LINE__, DEC_INFINITY, DEC_INFINITY, 1, "%d"},
  {__LINE__, -DEC_INFINITY, DEC_INFINITY, 1, "%d"},
  {__LINE__, DEC_INFINITY, DEC_NAN, 0, "%d"},
  {__LINE__, DEC_INFINITY, 0.0DL, 0, "%d"},
  {__LINE__, DEC_NAN, DEC_NAN, 1, "%d"},
  {__LINE__, DEC_NAN, DEC_INFINITY, 0, "%d"},
  {__LINE__, DEC_NAN, 0.0DL, 0, "%d"},
  {__LINE__, __DEC128_SUBNORMAL_MIN__, 1e-6176DL, 1, "%d"},
  {__LINE__, __DEC128_MIN__, 1e-6143DL, 1, "%d"},
  {__LINE__, __DEC128_MIN__, 1e-6142DL, 0, "%d"},
  {__LINE__, __DEC128_MAX__, 1e6144DL, 1, "%d"},
  {__LINE__, __DEC128_MAX__, 1e6110DL, 0, "%d"},
  {__LINE__, 1e1DL, 10e1DL, 1, "%d"},
  {__LINE__, 1e0DL, 10e0DL, 1, "%d"},
  {__LINE__, -0.25DL, 1e-2DL, 1, "%d"},
  {__LINE__, 0.625DL, 0.001DL, 1, "%d"},
  {__LINE__, 2097152.5DL, 0.1DL, 1, "%d"},
  {__LINE__, -2097152.5DL, 0.1DL, 1, "%d"},
  {__LINE__, -4503599627370495.5DL, 0.1DL, 1, "%d"},
  {__LINE__, 4503599627370.49625DL, 0.00001DL, 1, "%d"},
};
static const int d128_quantum_size =
  sizeof (d128_quantum) / sizeof (d128_quantum[0]);


typedef struct
{
  int line;
  _Decimal64 x;			/* Input y  */
  _Decimal64 y;			/* Input x  */
  _Bool e;			/* Expected should be this.  */
  const char *f;
} d64_type;

/* Test quantumd64 function  */
static const d64_type d64_quantum[] = {
  {__LINE__, DEC_INFINITY, DEC_INFINITY, 1, "%d"},
  {__LINE__, -DEC_INFINITY, DEC_INFINITY, 1, "%d"},
  {__LINE__, DEC_INFINITY, DEC_NAN, 0, "%d"},
  {__LINE__, DEC_INFINITY, 0.0DD, 0, "%d"},
  {__LINE__, DEC_NAN, DEC_NAN, 1, "%d"},
  {__LINE__, DEC_NAN, DEC_INFINITY, 0, "%d"},
  {__LINE__, DEC_NAN, 0.0DD, 0, "%d"},
  {__LINE__, __DEC64_SUBNORMAL_MIN__, 1e-398DD, 1, "%d"},
  {__LINE__, __DEC64_MIN__, 1e-383DD, 1, "%d"},
  {__LINE__, __DEC64_MIN__, 1e-382DD, 0, "%d"},
  {__LINE__, __DEC64_MAX__, 1e369DD, 1, "%d"},
  {__LINE__, __DEC64_MAX__, 1e367DD, 0, "%d"},
  {__LINE__, 1e1DD, 10e1DD, 1, "%d"},
  {__LINE__, 1e0DD, 10e0DD, 1, "%d"},
  {__LINE__, -0.25DD, 1e-2DD, 1, "%d"},
  {__LINE__, 0.625DD, 0.001DD, 1, "%d"},
  {__LINE__, 2097152.5DD, 0.1DD, 1, "%d"},
  {__LINE__, -2097152.5DD, 0.1DD, 1, "%d"},
  {__LINE__, -3599627370495.5DD, 0.1DD, 1, "%d"},
  {__LINE__, 3599627370.49625DD, 10.00001DD, 1, "%d"},
};
static const int d64_quantum_size =
  sizeof (d64_quantum) / sizeof (d64_quantum[0]);


typedef struct
{
  int line;
  _Decimal32 x;			/* Input y  */
  _Decimal32 y;			/* Input x  */
  _Bool e;			/* Expected should be this.  */
  const char *f;
} d32_type;

/* Test quantumd32 function  */
static const d32_type d32_quantum[] = {
  {__LINE__, DEC_INFINITY, DEC_INFINITY, 1, "%d"},
  {__LINE__, -DEC_INFINITY, DEC_INFINITY, 1, "%d"},
  {__LINE__, DEC_INFINITY, DEC_NAN, 0, "%d"},
  {__LINE__, DEC_INFINITY, 0.0DF, 0, "%d"},
  {__LINE__, DEC_NAN, DEC_NAN, 1, "%d"},
  {__LINE__, DEC_NAN, DEC_INFINITY, 0, "%d"},
  {__LINE__, DEC_NAN, 0.0DF, 0, "%d"},
  {__LINE__, __DEC32_SUBNORMAL_MIN__, 1e-101DF, 1, "%d"},
  {__LINE__, __DEC32_MIN__, 1e-95DF, 1, "%d"},
  {__LINE__, __DEC32_MIN__, 1e-94DF, 0, "%d"},
  {__LINE__, __DEC32_MAX__, 1e90DF, 1, "%d"},
  {__LINE__, __DEC32_MAX__, 1e89DF, 0, "%d"},
  {__LINE__, 1e1DF, 10e1DF, 1, "%d"},
  {__LINE__, 1e0DF, 10e0DF, 1, "%d"},
  {__LINE__, -0.25DF, 1e-2DF, 1, "%d"},
  {__LINE__, 0.625DF, 0.001DF, 1, "%d"},
  {__LINE__, 7152.5DF, 0.1DF, 1, "%d"},
  {__LINE__, -7152.5DF, 0.1DF, 1, "%d"},
  {__LINE__, -70495.5DF, 0.1DF, 1, "%d"},
  {__LINE__, 70.49625DF, 10.00001DF, 1, "%d"},
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
      _Bool r = samequantumd128 (d128_quantum[i].x, d128_quantum[i].y);
      printf ("samequantum (%.34DDgDL, %.34DDgDL) = %i) in %s:%d\n",
	d128_quantum[i].x, d128_quantum[i].y, r, __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d128_quantum[i].line, d128_quantum[i].e, r,
	     d128_quantum[i].f);
    }

  /* _Decimal64 tests.  */
  for (i = 0; i < d64_quantum_size; ++i)
    {
      _Bool r = samequantumd64 (d64_quantum[i].x, d64_quantum[i].y);
      printf ("samequantum (%.16DgDD, %.16DgDD) = %i) in %s:%d\n",
	d64_quantum[i].x, d64_quantum[i].y, r, __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d64_quantum[i].line, d64_quantum[i].e, r,
	     d64_quantum[i].f);
    }

  /* _Decimal32 tests.  */
  for (i = 0; i < d32_quantum_size; ++i)
    {
      _Bool r = samequantumd32 (d32_quantum[i].x, d32_quantum[i].y);
      printf ("samequantum (%.7HgDF, %.7HgDF) = %i) in %s:%d\n",
	d32_quantum[i].x, d32_quantum[i].y, r, __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d32_quantum[i].line, d32_quantum[i].e, r,
	     d32_quantum[i].f);
    }

  _REPORT ();

  /* fail comes from scaffold.c  */
  return fail;
}
