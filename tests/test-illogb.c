/* Test [i|l]logb[32|64|128]

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
#define __STDC_WANT_DEC_FP__
#endif

#include <math.h>
#include <float.h>
#include <stdio.h>

#define _WANT_VC 1
#include "scaffold.c"

typedef struct {
  int line;
  _Decimal128 x;        /* Value to test  */
  int e;                /* Result should be this.  */
  const char *format;   /* Value form to printf (%i) */
} id128_type;

static id128_type id128[] =
{
  { __LINE__, 1.0DL,                                 0, "%d" },
  { __LINE__, 10.71284590452353602878DL,             1, "%d" },
  { __LINE__, 1024.0DL,                              3, "%d" },
  { __LINE__, -200000.0DL,                           5, "%d" },

  { __LINE__, 0.0DL,                         FP_ILOGB0, "%d" },
  { __LINE__, DEC_NAN,                     FP_ILOGBNAN, "%d" },
  { __LINE__, DEC_INFINITY,                    INT_MAX, "%d" },
  { __LINE__, -DEC_INFINITY,                   INT_MIN, "%d" },
};
static const int id128_size = sizeof (id128) / sizeof (id128[0]);


typedef struct {
  int line;
  _Decimal64 x;         /* Value to test  */
  int e;                /* Result should be this.  */
  const char *format;   /* Value form to printf (%i) */
} id64_type;

static id64_type id64[] =
{
  { __LINE__, 1.0DL,                                 0, "%d" },
  { __LINE__, 10.7644590452353602878DL,              1, "%d" },
  { __LINE__, 1024.0DL,                              3, "%d" },
  { __LINE__, -200000.0DL,                           5, "%d" },

  { __LINE__, 0.0DL,                         FP_ILOGB0, "%d" },
  { __LINE__, DEC_NAN,                     FP_ILOGBNAN, "%d" },
  { __LINE__, DEC_INFINITY,                    INT_MAX, "%d" },
  { __LINE__, -DEC_INFINITY,                   INT_MIN, "%d" },
};
static const int id64_size = sizeof (id64) / sizeof (id64[0]);


typedef struct {
  int line;
  _Decimal32 x;         /* Value to test  */
  int e;                /* Result should be this.  */
  const char *format;   /* Value form to printf (%i) */
} id32_type;

static id32_type id32[] =
{
  { __LINE__, 1.0DL,                                 0, "%d" },
  { __LINE__, 10.7324590452353602878DL,              1, "%d" },
  { __LINE__, 1024.0DL,                              3, "%d" },
  { __LINE__, -200000.0DL,                           5, "%d" },

  { __LINE__, 0.0DL,                         FP_ILOGB0, "%d" },
  { __LINE__, DEC_NAN,                     FP_ILOGBNAN, "%d" },
  { __LINE__, DEC_INFINITY,                    INT_MAX, "%d" },
  { __LINE__, -DEC_INFINITY,                   INT_MIN, "%d" },
};
static const int id32_size = sizeof (id32) / sizeof (id32[0]);


typedef struct {
  int line;
  _Decimal128 x;        /* Value to test  */
  long int e;           /* Result should be this.  */
  const char *format;   /* Value form to printf (%i) */
} ld128_type;

static ld128_type ld128[] =
{
  { __LINE__, 1.0DL,                                 0, "%ld" },
  { __LINE__, 10.71284590452353602878DL,             1, "%ld" },
  { __LINE__, 1024.0DL,                              3, "%ld" },
  { __LINE__, -200000.0DL,                           5, "%ld" },

  { __LINE__, 0.0DL,                         FP_ILOGB0, "%ld" },
  { __LINE__, DEC_NAN,                     FP_ILOGBNAN, "%ld" },
  { __LINE__, DEC_INFINITY,                   LONG_MAX, "%ld" },
  { __LINE__, -DEC_INFINITY,                  LONG_MIN, "%ld" },
};
static const int ld128_size = sizeof (ld128) / sizeof (ld128[0]);


typedef struct {
  int line;
  _Decimal64 x;         /* Value to test  */
  long int e;           /* Result should be this.  */
  const char *format;   /* Value form to printf (%i) */
} ld64_type;

static ld64_type ld64[] =
{
  { __LINE__, 1.0DL,                                 0, "%ld" },
  { __LINE__, 10.7644590452353602878DL,              1, "%ld" },
  { __LINE__, 1024.0DL,                              3, "%ld" },
  { __LINE__, -200000.0DL,                           5, "%ld" },

  { __LINE__, 0.0DL,                         FP_ILOGB0, "%ld" },
  { __LINE__, DEC_NAN,                     FP_ILOGBNAN, "%ld" },
  { __LINE__, DEC_INFINITY,                   LONG_MAX, "%ld" },
  { __LINE__, -DEC_INFINITY,                  LONG_MIN, "%ld" },
};
static const int ld64_size = sizeof (ld64) / sizeof (ld64[0]);


typedef struct {
  int line;
  _Decimal32 x;         /* Value to test  */
  long int e;           /* Result should be this.  */
  const char *format;   /* Value form to printf (%i) */
} ld32_type;

static ld32_type ld32[] =
{
  { __LINE__, 1.0DL,                                 0, "%ld" },
  { __LINE__, 10.7324590452353602878DL,              1, "%ld" },
  { __LINE__, 1024.0DL,                              3, "%ld" },
  { __LINE__, -200000.0DL,                           5, "%ld" },

  { __LINE__, 0.0DL,                         FP_ILOGB0, "%ld" },
  { __LINE__, DEC_NAN,                     FP_ILOGBNAN, "%ld" },
  { __LINE__, DEC_INFINITY,                   LONG_MAX, "%ld" },
  { __LINE__, -DEC_INFINITY,                  LONG_MIN, "%ld" },
};
static const int ld32_size = sizeof (ld32) / sizeof (ld32[0]);


int main (void)
{
  int i;

  for (i=0; i<id128_size; ++i)
    {
      int retval = ilogbd128 (id128[i].x);
      printf ("ilogbd128 (%.34DDeDL) = %i in: %s:%d\n", id128[i].x, retval,
	__FILE__, __LINE__-1);
      _VC_P (__FILE__, id128[i].line, id128[i].e, retval, id128[i].format);
    }
  for (i=0; i<id64_size; ++i)
    {
      int retval = ilogbd64 (id64[i].x);
      printf ("ilogbd64 (%.34DeDL) = %i in: %s:%d\n", id64[i].x, retval,
	__FILE__, __LINE__-1);
      _VC_P (__FILE__, id64[i].line, id64[i].e, retval, id64[i].format);
    }
  for (i=0; i<id32_size; ++i)
    {
      int retval = ilogbd32 (id32[i].x);
      printf ("ilogbd32 (%.34HeDL) = %i in: %s:%d\n", id32[i].x, retval,
	__FILE__, __LINE__-1);
      _VC_P (__FILE__, id32[i].line, id32[i].e, retval, id32[i].format);
    }

  for (i=0; i<ld128_size; ++i)
    {
      long int retval = llogbd128 (ld128[i].x);
      printf ("ilogbd128 (%.34DDeDL) = %li in: %s:%d\n", ld128[i].x, retval,
	__FILE__, __LINE__-1);
      _VC_P (__FILE__, ld128[i].line, ld128[i].e, retval, ld128[i].format);
    }
  for (i=0; i<ld64_size; ++i)
    {
      long int retval = llogbd64 (ld64[i].x);
      printf ("ilogbd64 (%.34DeDL) = %li in: %s:%d\n", ld64[i].x, retval,
	__FILE__, __LINE__-1);
      _VC_P (__FILE__, ld64[i].line, ld64[i].e, retval, ld64[i].format);
    }
  for (i=0; i<ld32_size; ++i)
    {
      long int retval = llogbd32 (ld32[i].x);
      printf ("ilogbd32 (%.34HeDL) = %li in: %s:%d\n", ld32[i].x, retval,
	__FILE__, __LINE__-1);
      _VC_P (__FILE__, ld32[i].line, ld32[i].e, retval, ld32[i].format);
    }

  _REPORT ();

  /* fail comes from scaffold.c  */
  return fail;
}
