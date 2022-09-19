/* Unit test the internal getexpd[32|64|128]() functions.

   Copyright (C) 2014-2015 Free Software Foundation, Inc.

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

#ifndef __STDC_WANT_DEC_FP__
#define __STDC_WANT_DEC_FP__
#endif

#include <stdio.h>
#include <math.h>
#include <float.h> /* DEC_NAN definition.  */

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

typedef struct{
  int line;
  _Decimal128 x;
  int e;
  _Decimal128 r;
  const char *format;
} d128_type;

#define RET128_MAX                     6111
#define RET128_MIN                    -6143
#define RET128_MIN_SUB                -6176

#define DEC128_TEST(__x, __e, __r) \
  { __LINE__, __x, __e, __r, "%DDeDL" }

static const
d128_type d128[] =
{
  DEC128_TEST (1E0DL,     1,   1E1DL),
  DEC128_TEST (1E0DL,    10,  1E10DL),
  DEC128_TEST (1E0DL,   100, 1E100DL),
  DEC128_TEST (1E100DL,   0,   1E0DL),
  DEC128_TEST (1E10DL,    0,   1E0DL),
  DEC128_TEST (1E1DL,     0,   1E0DL),
  DEC128_TEST (9999999999999999999999999999999999E0DL,      1,
	       9999999999999999999999999999999999E1DL),
  DEC128_TEST (9999999999999999999999999999999999E0DL,     10,
	       9999999999999999999999999999999999E10DL),
  DEC128_TEST (9999999999999999999999999999999999E0DL,    100,
	       9999999999999999999999999999999999E100DL)
};
static const int d128_len = sizeof (d128) / sizeof (d128[0]);


typedef struct {
  int line;
  _Decimal64 x;
  int e;
  _Decimal64 r;
  const char *format;
} d64_type;


#define RET64_MAX                       369
#define RET64_MIN                      -383
#define RET64_MIN_SUB                  -398

#define DEC64_TEST(__x, __e, __r) \
  { __LINE__, __x, __e, __r, "%DeDD" }

static const
d64_type d64[] =
{
  DEC64_TEST (1E0DD,     1,   1E1DD),
  DEC64_TEST (1E0DD,    10,  1E10DD),
  DEC64_TEST (1E0DD,   100, 1E100DD),
  DEC64_TEST (1E100DD,   0,   1E0DD),
  DEC64_TEST (1E10DD,    0,   1E0DD),
  DEC64_TEST (1E1DD,     0,   1E0DD),
  DEC64_TEST (9999999999999999E0DD,      1,
	       9999999999999999E1DD),
  DEC64_TEST (9999999999999999E0DD,     10,
	       9999999999999999E10DD),
  DEC64_TEST (9999999999999999E0DD,    100,
	       9999999999999999E100DD)
};
static const int d64_len = sizeof (d64) / sizeof (d64[0]);


typedef struct {
  int line;
  _Decimal32 x;
  int e;
  _Decimal32 r;
  const char *format;
} d32_type;


#define RET32_MAX                        90
#define RET32_MIN                       -95
#define RET32_MIN_SUB                  -101

#define DEC32_TEST(__x, __e, __r) \
  { __LINE__, __x, __e, __r, "%HeDF" }

static const
d32_type d32[] =
{
  DEC32_TEST (1E0DF,     1,   1E1DF),
  DEC32_TEST (1E0DF,    10,  1E10DF),
  DEC32_TEST (1E0DF,    90,  1E90DF),
  DEC32_TEST (1E90DF,    0,   1E0DF),
  DEC32_TEST (1E10DF,    0,   1E0DF),
  DEC32_TEST (1E1DF,     0,   1E0DF),
  DEC32_TEST (9999999E0DF,      1,
	       9999999E1DF),
  DEC32_TEST (9999999E0DF,     10,
	       9999999E10DF),
  DEC32_TEST (9999999E0DF,     90,
	       9999999E90DF)
};
static const int d32_len = sizeof (d32) / sizeof (d32[0]);


int main (void)
{
  int i;

  for (i=0; i<d128_len; ++i)
    {
      _Decimal128 retval = setexpd128 (d128[i].x, d128[i].e);
      fprintf (stdout, "setexpd128 (%DDeDL, %i) in: %s:%d\n", d128[i].x,
	       d128[i].e, __FILE__,__LINE__-1);
      _VC_P (__FILE__, d128[i].line, d128[i].r, retval, d128[i].format);
    }

  for (i=0; i<d64_len; ++i)
    {
      _Decimal64 retval = setexpd64 (d64[i].x, d64[i].e);
      fprintf (stdout, "setexpd64 (%DeDD, %i) in: %s:%d\n", d64[i].x,
	       d64[i].e, __FILE__,__LINE__-1);
      _VC_P (__FILE__, d64[i].line, d64[i].r, retval, d64[i].format);
    }

  for (i=0; i<d32_len; ++i)
    {
      _Decimal32 retval = setexpd32 (d32[i].x, d32[i].e);
      fprintf (stdout, "setexpd32 (%HeDF, %i) in: %s:%d\n", d32[i].x,
	       d32[i].e, __FILE__,__LINE__-1);
      _VC_P (__FILE__, d32[i].line, d32[i].r, retval, d32[i].format);
    }

  _REPORT();

  /* fail comes from scaffold.c  */
  return fail;
}
