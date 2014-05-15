/* Unit test the internal getexpd[32|64|128]() functions.

   Copyright (C) 2012 Free Software Foundation, Inc.

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
  const char *format;
} d128_type;

#define RET128_MAX                     6111
#define RET128_MIN                    -6143
#define RET128_MIN_SUB                -6176

static const
d128_type d128[] =
{
  {__LINE__, __DEC128_MAX__,                           RET128_MAX, "%d"},
  {__LINE__, __DEC128_MIN__,                           RET128_MIN, "%d"},
  {__LINE__, __DEC128_SUBNORMAL_MIN__,             RET128_MIN_SUB, "%d"},
  {__LINE__, 1E200DL,                                         200, "%d"},
  {__LINE__, 12131E2000DL,                                   2000, "%d"},
  {__LINE__, 1E-200DL,                                       -200, "%d"},
  {__LINE__, 12131E-2000DL,                                 -2000, "%d"},
  {__LINE__, 9999999999999999999999999999999999E0DL,            0, "%d"},
  {__LINE__, 9999999999999999999999999999999999E10DL,          10, "%d"},
  {__LINE__, 9999999999999999999999999999999999E100DL,        100, "%d"},
  {__LINE__, 9999999999999999999999999999999999E1000DL,      1000, "%d"},
};
static const int d128_len = sizeof (d128) / sizeof (d128[0]);


typedef struct {
  int line;
  _Decimal64 x;
  int e;
  const char *format;
} d64_type;


#define RET64_MAX                       369
#define RET64_MIN                      -383
#define RET64_MIN_SUB                  -398

d64_type d64[] =
{
  {__LINE__, __DEC64_MAX__,                 RET64_MAX, "%d"},
  {__LINE__, __DEC64_MIN__,                 RET64_MIN, "%d"},
  {__LINE__, __DEC64_SUBNORMAL_MIN__,   RET64_MIN_SUB, "%d"},
  {__LINE__, 1E20DD,                               20, "%d"},
  {__LINE__, 12131E200DD,                         200, "%d"},
  {__LINE__, 1E-20DD,                             -20, "%d"},
  {__LINE__, 12131E-200DD,                       -200, "%d"},
  {__LINE__, 9999999999999999E0DD,                  0, "%d"},
  {__LINE__, 9999999999999999E10DD,                10, "%d"},
  {__LINE__, 9999999999999999E100DD,              100, "%d"},

};
static const int d64_len = sizeof (d64) / sizeof (d64[0]);


typedef struct {
  int line;
  _Decimal32 x;
  int e;
  const char *format;
} d32_type;


#define RET32_MAX                        90
#define RET32_MIN                       -95
#define RET32_MIN_SUB                  -101

d32_type d32[] =
{
  {__LINE__, __DEC32_MAX__,                 RET32_MAX, "%d"},
  {__LINE__, __DEC32_MIN__,                 RET32_MIN, "%d"},
  {__LINE__, __DEC32_SUBNORMAL_MIN__,   RET32_MIN_SUB, "%d"},
  {__LINE__, 1E2DF,                                 2, "%d"},
  {__LINE__, 12131E20DF,                           20, "%d"},
  {__LINE__, 1E-2DF,                               -2, "%d"},
  {__LINE__, 12131E-20DF,                         -20, "%d"},
  {__LINE__, 9999999E0DF,                           0, "%d"},
  {__LINE__, 9999999E10DF,                         10, "%d"},
};
static const int d32_len = sizeof (d32) / sizeof (d32[0]);


int main (void)
{
  int i;

  for (i=0; i<d128_len; ++i)
    {
      int retval = getexpd128 (d128[i].x);
      fprintf (stdout, "getexpd128 (%DDeDL) in: %s:%d\n", d128[i].x,
	       __FILE__,__LINE__-1);
      _VC_P (__FILE__, d128[i].line, d128[i].e, retval, d128[i].format);
    }

  for (i=0; i<d64_len; ++i)
    {
      int retval = getexpd64 (d64[i].x);
      fprintf (stdout, "getexpd64 (%DeDD) in: %s:%d\n", d64[i].x,
	       __FILE__,__LINE__-1);
      _VC_P (__FILE__, d64[i].line, d64[i].e, retval, d64[i].format);
    }

  for (i=0; i<d32_len; ++i)
    {
      int retval = getexpd32 (d32[i].x);
      fprintf (stdout, "getexpd32 (%HeDF) in: %s:%d\n", d32[i].x,
	       __FILE__,__LINE__-1);
      _VC_P (__FILE__, d32[i].line, d32[i].e, retval, d32[i].format);
    }

  _REPORT();

  /* fail comes from scaffold.c  */
  return fail;
}


