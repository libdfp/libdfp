/* Unit test the ti (int128) conversion routines.

   Copyright (C) 2015 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Paul E. Murphy <murphyp@linux.vnet.ibm.com>

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

#include <float.h>		/* DEC_NAN definition.  */
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

/* TODO: For now, BID does not have dpd conversions.  */
#ifndef __DECIMAL_BID_FORMAT__
#define HAS_TI 1
#endif

#define _WANT_VC 1		/* Pick up the _VC_P(x,y,fmt) macro.  */
#include "scaffold.c"		/* Pick up the _VC_P(x,y,fmt) macro.  */

/* We're going to be comparing fields so we need to extract the data.  This is a
 * sneaky way to get around the fact that get_digits_d* isn't exported from
 * libdfp.  */
#ifdef __DECIMAL_BID_FORMAT__
#include "../sysdeps/bid/bid-private.c"
#else
#include "../sysdeps/dpd/dpd-private.c"
#endif

typedef struct
{
  const char *format;		/* Expected value printf format.  */
  int uns;			/* Signed or unsigned conversion.  */
  int line;
  char const *x;		/* Stringified __int128 input value.  */
  _Decimal128 e;		/* Expected result.  */
} d128_type;

static d128_type d128[] = {
#if HAS_TI
  {"%DDe", 0, __LINE__, "0", 0.DL},
  {"%DDe", 0, __LINE__, "1", 1.DL},
  {"%DDe", 0, __LINE__, "-1", -1.DL},
  {"%DDe", 0, __LINE__, "170141183460469231731687303715884100000",
   170141183460469231731687303715884100000.DL},
  {"%DDe", 0, __LINE__, "170141183460469231731687303715884100001",
   170141183460469231731687303715884100000.DL},
  {"%DDe", 0, __LINE__, "-170141183460469231731687303715884100000",
   -170141183460469231731687303715884100000.DL},
  {"%DDe", 0, __LINE__, "-100000000000000000", -1e17DL},
  {"%DDe", 0, __LINE__, "100000000000000000", 1e17DL},

  /* Unsigned conversions.  */
  {"%DDe", 1, __LINE__, "340282366920938463463374607431768211455",
   340282366920938463463374607431768200000.DL},
  {"%DDe", 1, __LINE__, "1000000", 1000000.DL},
  {"%DDe", 1, __LINE__, "100000000000000000", 100000000000000000.DL},
  {"%DDe", 1, __LINE__, "0", 0.DL},
#endif /* HAS_TI */
};

static int d128_s = sizeof (d128) / sizeof (d128[0]);

/* Mediocre string to ti/unsti function. */
static __int128
intifyti (char const *str)
{
  unsigned __int128 val = 0;
  long neg = 0;
  if (*str == '-')
    {
      neg = 1;
      str++;
    }
  while (*str)
    {
      val *= (__int128) 10;
      val += (*str - '0');
      str++;
    }
  if (neg)
    return -((__int128) val);
  return val;
}

int
main (void)
{
  int i;

  for (i = 0; i < d128_s; ++i)
    {
      _Decimal128 x;
      char const *fmt;
      if (d128[i].uns)
	{
	  fmt = "floatunstitd (%s) in: %s:%d\n";
	  unsigned __int128 x_ti = intifyti (d128[i].x);
	  x = x_ti;
	}
      else
	{
	  fmt = "floattitd (%s) in: %s:%d\n";
	  __int128 x_ti = intifyti (d128[i].x);
	  x = x_ti;
	}
      fprintf (stdout, fmt, d128[i].x, __FILE__, __LINE__);
      _VC_P (__FILE__, d128[i].line, d128[i].e, x, d128[i].format);
    }

  _REPORT ();

  /* fail comes from scaffold.c  */
  return fail;
}
