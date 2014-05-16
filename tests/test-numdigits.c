/* Unit test the internal numdigitsd[32|64|128]() functions.

   Copyright (C) 2010-2014 Free Software Foundation, Inc.

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

#ifndef __STDC_WANT_DEC_FP__
#define __STDC_WANT_DEC_FP__
#endif

#include <float.h> /* DEC_NAN definition.  */
#include <stdio.h>
#include <math.h>

#include <get_digits.h>

#define _DECIMAL_SIZE 32
#define DEC_TYPE _Decimal32
#include <numdigits.h>
#undef _DECIMAL_SIZE
#undef DEC_TYPE

#define _DECIMAL_SIZE 64
#define DEC_TYPE _Decimal64
#include <numdigits.h>
#undef _DECIMAL_SIZE
#undef DEC_TYPE

#define _DECIMAL_SIZE 128
#define DEC_TYPE _Decimal128
#include <numdigits.h>
#undef _DECIMAL_SIZE
#undef DEC_TYPE

#define _WANT_VC 1 /* Pick up the _VC_P(x,y,fmt) macro.  */
#include "scaffold.c" /* Pick up the _VC_P(x,y,fmt) macro.  */

/* We're going to be comparing fields so we need to extract the data.  This is a
 * sneaky way to get around the fact that get_digits_d* isn't exported from
 * libdfp.  */
#ifdef __DECIMAL_BID_FORMAT__
# include "../sysdeps/bid/bid-private.c"
#else
# include "../sysdeps/dpd/dpd-private.c"
#endif

typedef struct
{
  int line;
  _Decimal128 x;  /* Value to test  */
  int e;  /* Result should be this.  */
  const char *format; /* printf %d */
} d128_type;

static
d128_type d128[] =
{
  {__LINE__, 0.02E-2DL, 1,  "%d"},
  {__LINE__, 0.0200E-2DL, 3,  "%d"},
  {__LINE__, 1.0E-2DL, 2,  "%d"},
  {__LINE__, 0.0000345E-2DL, 3,  "%d"},
  {__LINE__, 0.0000345E-10DL, 3,  "%d"},
  {__LINE__, 123456.0E-2DL, 7,  "%d"},
  {__LINE__, 123456.000E-2DL, 9,  "%d"},
  {__LINE__, 123456.000E-4DL, 9,  "%d"},
  {__LINE__, 123456.000E-6DL, 9,  "%d"},
  {__LINE__, 123456.00000000000000E-18DL, 20,  "%d"},
  {__LINE__, 123.456E-6DL, 6, "%d"},
};
static int d128_s = sizeof (d128) / sizeof (d128[0]);

typedef struct{
  int line;
  _Decimal64 x;  /* Value to test  */
  int e;  /* Result should be this.  */
  const char *format; /* printf %d */
} d64_type;

static
d64_type d64[] =
{
  {__LINE__, 0.02E-2DD, 1,  "%d"},
  {__LINE__, 0.0200E-2DD, 3,  "%d"},
  {__LINE__, 1.0E-2DD, 2,  "%d"},
  {__LINE__, 0.0000345E-2DD, 3,  "%d"},
  {__LINE__, 0.0000345E-10DD, 3,  "%d"},
  {__LINE__, 123456.0E-2DD, 7,  "%d"},
  {__LINE__, 123456.000E-2DD, 9,  "%d"},
  {__LINE__, 123456.000E-4DD, 9,  "%d"},
  {__LINE__, 123456.000E-6DD, 9,  "%d"},

  /* Can't exceed __DEC64_MANT_DIG__.  This test has 17 digits but the
   * compiler will truncate it (or round) before it's passed   */
  {__LINE__, 9.9999999999999999E300DD, 16, "%d"},

  /* There was a known bug with any value where the normalized exponent
   * exceeded 369.  This caused __DEC64_MAX__ to return incorrect results.  */
  {__LINE__, __DEC64_MAX__, 16, "%d"},

  /* The normalized exponent is '369' so the right justified encoding has two
   * digits, '90', in the mantissa.  */
  {__LINE__, 9E370DD, 2, "%d"},

  /* Fails the same way: the absolute value of the exponent exceeds 369. */
  {__LINE__, __DEC64_MIN__, 1, "%d"},
  {__LINE__, 1E-398DD, 1, "%d"},

  /* 0.000000000000001E-383DD  */
  {__LINE__, __DEC64_SUBNORMAL_MIN__, 1, "%d"},
};
static int d64_s = sizeof (d64) / sizeof (d64[0]);

typedef struct{
  int line;
  _Decimal32 x;  /* Value to test  */
  int e;  /* Result should be this.  */
  const char *format; /* printf %d */
} d32_type;

d32_type d32[] =
{
  {__LINE__, 0.02E-2DF, 1,  "%d"},
  {__LINE__, 0.0200E-2DF, 3,  "%d"},
  {__LINE__, 1.0E-2DF, 2,  "%d"},
  {__LINE__, 0.0000345E-2DF, 3,  "%d"},
  {__LINE__, 0.0000345E-10DF, 3,  "%d"},
  {__LINE__, 123456.0E-2DF, 7,  "%d"},
  {__LINE__, 123456.000E-2DF, 7,  "%d"}, /* can't exceed __DEC32_MANT_DIG__  */
  {__LINE__, 123456.000E-4DF, 7,  "%d"}, /* can't exceed __DEC32_MANT_DIG__  */
  {__LINE__, 123456.000E-6DF, 7,  "%d"}, /* can't exceed __DEC32_MANT_DIG__  */
  /* can't exceed __DEC32_MANT_DIG__  */
  {__LINE__, 123456.00000000000000E-18DD, 7,  "%d"},

   /* Problem numbers.  These should all return '1'.  At some point they were
    * returning '0' for soft-dfp and '1' for hard-dfp.  */
  {__LINE__, 0.00000E-101DF, 1,  "%d"},
  {__LINE__, 0.00000E-97DF, 1, "%d"},
  {__LINE__, 0.00000E-96DF, 1, "%d"},
  {__LINE__, 0.00000E-95DF, 1, "%d"},
  {__LINE__, 0e10DF, 1, "%d"},
  {__LINE__, 0.0e10DF, 1, "%d"},
  {__LINE__, 0.0e-09DF, 1, "%d"},
  {__LINE__, 0e-6DF,1, "%d"},
  {__LINE__, 0e-7DF, 1, "%d"},
  {__LINE__, 0.e-10DF, 1, "%d"},
  {__LINE__, 0.e-10DF, 1, "%d"},
  {__LINE__, 0.e-10DF, 1, "%d"},
  {__LINE__, 0e2DF, 1, "%d"},
  {__LINE__, 0.0e2DF, 1, "%d"},
  {__LINE__, 0.0e10DF, 1, "%d"},
};
static int d32_s = sizeof (d32) / sizeof (d32[0]);

int main (void)
{
  int i;

  for (i=0; i<d128_s; ++i)
    {
      int retval = numdigitsd128 (d128[i].x);
      fprintf(stdout,"numdigitsd128 (%DDgDL) in: %s:%d\n",
		      d128[i].x, __FILE__, __LINE__-1);
      _VC_P(__FILE__, d128[i].line, d128[i].e, retval, d128[i].format);
    }

  for (i=0; i<d64_s; ++i)
    {
      int retval = numdigitsd64 (d64[i].x);
      fprintf(stdout,"numdigitsd64 (%DgDD) in: %s:%d\n",
		      d64[i].x, __FILE__, __LINE__-1);
      _VC_P(__FILE__, d64[i].line, d64[i].e, retval, d64[i].format);
    }

  for (i=0; i<d32_s; ++i)
    {
      int retval = numdigitsd32 (d32[i].x);
      fprintf (stdout,"numdigitsd32 (%HgDF) in: %s:%d\n",
		       d32[i].x, __FILE__, __LINE__-1);
      _VC_P (__FILE__, d32[i].line, d32[i].e, retval, d32[i].format);
    }

  _REPORT();

  /* fail comes from scaffold.c  */
  return fail;
}


