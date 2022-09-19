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
   if not, write to the Free Software Foundation, Inc., 51 Franklin
   Street, Fifth Floor, Boston, MA 02110-1301 USA.

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

#include "scaffold.c"
#include "dfpacc.h"

union tidi
{
  INT128 ti;
  unsigned long long int di[2];
};

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#  define INT128_MIN ({union tidi d = { .di = {0x8000000000000000LL, 0LL}}; d.ti; })
#else
#  define INT128_MIN ({union tidi d = { .di = {0x0LL, 0x8000000000000000LL}}; d.ti; })
#endif

/* 128b integer lacks printf support. */
#define DECLARE_TESTS(_t,_n) \
  struct                     \
  {                          \
    int line;                \
    _Decimal##_t x;          \
    char const *e;           \
  } d##_t##_n[]

DECLARE_TESTS(128,) = {
  {__LINE__, 0.DL, "0"},
  {__LINE__, 1.DL, "1"},
  {__LINE__, -1.DL, "-1"},
  {__LINE__, -1.12DL, "-1"},
  {__LINE__, 170141183460469231731687303715884100000.DL, "170141183460469231731687303715884100000"},
  {__LINE__, -170141183460469231731687303715884100000.DL, "-170141183460469231731687303715884100000"},
  {__LINE__, 170141183460469231731687303715884200000.DL, "170141183460469231731687303715884105727"},
  {__LINE__, -170141183460469231731687303715884200000.DL, "−170141183460469231731687303715884105728"},
  {__LINE__, -9999999999999999999999999999999999.DL, "-9999999999999999999999999999999999"},
  {__LINE__, -1e17DL, "-100000000000000000"},
  {__LINE__, 1e17DL, "100000000000000000"},
  {__LINE__, 1e20DL, "100000000000000000000"},
  {__LINE__, DEC_INFINITY, "170141183460469231731687303715884105727"},
  {__LINE__, -DEC_INFINITY, "−170141183460469231731687303715884105728"},
  {__LINE__, __builtin_nand128 (""), "−170141183460469231731687303715884105728"},
};

DECLARE_TESTS(128,uns) = {
  {__LINE__, 0.DL, "0"},
  {__LINE__, 1.DL, "1"},
  {__LINE__, -1.DL, "0"},
  {__LINE__, -.1DL, "0"},
  {__LINE__, 340282366920938463463374607431768200000.DL, "340282366920938463463374607431768200000"},
  {__LINE__, 340282366920938463463374607431768300000.DL, "340282366920938463463374607431768211455"},
  {__LINE__, 1e5DL, "100000"},
  {__LINE__, 1e17DL, "100000000000000000"},
  {__LINE__, 1e22DL, "10000000000000000000000"},
  {__LINE__, -1e17DL, "0"},
  {__LINE__, __builtin_nand128 (""), "0"},
  {__LINE__, DEC_INFINITY, "340282366920938463463374607431768211455"},
};

DECLARE_TESTS(64,) = {
  {__LINE__, 0.DD, "0"},
  {__LINE__, 1.DD, "1"},
  {__LINE__, -1.DD, "-1"},
  {__LINE__, -123.DD, "-123"},
  {__LINE__, 123.DD, "123"},
  {__LINE__, -1.12DD, "-1"},
  {__LINE__, -9999999999999999.DD, "-9999999999999999"},
  {__LINE__, 170141183460469200000000000000000000000.DD, "170141183460469200000000000000000000000"},
  {__LINE__,-170141183460469200000000000000000000000.DD,"-170141183460469200000000000000000000000"},
  {__LINE__, 170141183460469300000000000000000000000.DD, "170141183460469231731687303715884105727"},
  {__LINE__,-170141183460469300000000000000000000000.DD,"−170141183460469231731687303715884105728"},
  {__LINE__, -1e17DD, "-100000000000000000"},
  {__LINE__, 1e17DD, "100000000000000000"},
  {__LINE__, 1e20DD, "100000000000000000000"},
  {__LINE__, DEC_INFINITY, "170141183460469231731687303715884105727"},
  {__LINE__, -DEC_INFINITY, "−170141183460469231731687303715884105728"},
  {__LINE__, __builtin_nand64(""), "−170141183460469231731687303715884105728"},
};

DECLARE_TESTS(64,uns) = {
  {__LINE__, 0.DD, "0"},
  {__LINE__, 1.DD, "1"},
  {__LINE__, -1.DD, "0"},
  {__LINE__, -.1DD, "0"},
  {__LINE__, 340282366920938400000000000000000000000.DD, "340282366920938400000000000000000000000"},
  {__LINE__, 340282366920938500000000000000000000000.DD, "340282366920938463463374607431768211455"},
  {__LINE__, 1e5DD, "100000"},
  {__LINE__, 1e17DD, "100000000000000000"},
  {__LINE__, 1e22DD, "10000000000000000000000"},
  {__LINE__, -1e17DD, "0"},
  {__LINE__, __builtin_nand64 (""), "0"},
  {__LINE__, DEC_INFINITY, "340282366920938463463374607431768211455"},
};

DECLARE_TESTS(32,) = {
  {__LINE__, 0.DF, "0"},
  {__LINE__, 1.DF, "1"},
  {__LINE__, -1.DF, "-1"},
  {__LINE__, -123.DF, "-123"},
  {__LINE__, 123.DF, "123"},
  {__LINE__, -1.12DF, "-1"},
  {__LINE__, -9999999.DF, "-9999999"},
  {__LINE__, 170141100000000000000000000000000000000.DF, "170141100000000000000000000000000000000"},
  {__LINE__,-170141100000000000000000000000000000000.DF,"-170141100000000000000000000000000000000"},
  {__LINE__, 170141200000000000000000000000000000000.DF, "170141183460469231731687303715884105727"},
  {__LINE__,-170141200000000000000000000000000000000.DF,"−170141183460469231731687303715884105728"},
  {__LINE__, -1e17DF, "-100000000000000000"},
  {__LINE__, 1e17DF, "100000000000000000"},
  {__LINE__, 1e20DF, "100000000000000000000"},
  {__LINE__, DEC_INFINITY, "170141183460469231731687303715884105727"},
  {__LINE__, -DEC_INFINITY, "−170141183460469231731687303715884105728"},
  {__LINE__, __builtin_nand32(""), "−170141183460469231731687303715884105728"},
};

DECLARE_TESTS(32,uns) = {
  {__LINE__, 0.DF, "0"},
  {__LINE__, 1.DF, "1"},
  {__LINE__, -1.DF, "0"},
  {__LINE__, -.1DF, "0"},
  {__LINE__, 340282300000000000000000000000000000000.DF, "340282300000000000000000000000000000000"},
  {__LINE__, 340282400000000000000000000000000000000.DF, "340282366920938463463374607431768211455"},
  {__LINE__, 1e5DF, "100000"},
  {__LINE__, 1e17DF, "100000000000000000"},
  {__LINE__, 1e22DF, "10000000000000000000000"},
  {__LINE__, -1e17DF, "0"},
  {__LINE__, __builtin_nand32 (""), "0"},
  {__LINE__, DEC_INFINITY, "340282366920938463463374607431768211455"},
};

/* Mediocre stringify routing for all valid td conversions to ti/unsti. */
static char const *
stringifyti (INT128 val)
{
  static char s128str[60];
  memset (s128str, ' ', sizeof (s128str));
  long i = sizeof (s128str) - 2, neg = 0;
  s128str[sizeof (s128str) - 1] = 0;
  s128str[sizeof (s128str) - 2] = '0';
  if (val == 0)
    i--;
  else if(val == INT128_MIN)
      return "−170141183460469231731687303715884105728";
  if (val < 0)
    {
      neg = 1;
      val = -val;
    }
  while (val)
    {
      INT128 rem = val % (INT128) 10;
      val /= (INT128) 10;
      s128str[i--] = '0' + (char) rem;
    }
  if (neg)
    s128str[i--] = '-';
  return &s128str[i + 1];
}

/* Likewise for unsigned.  */
static char const *
stringifyunsti (UINT128 val)
{
  static char s128str[60];
  memset (s128str, ' ', sizeof (s128str));
  long i = sizeof (s128str) - 2;
  s128str[sizeof (s128str) - 1] = 0;
  s128str[sizeof (s128str) - 2] = '0';
  if (val == 0)
    i--;
  while (val)
    {
      INT128 rem = val % (INT128) 10;
      val /= (INT128) 10;
      s128str[i--] = '0' + (char) rem;
    }
  return &s128str[i + 1];
}

typedef INT128 ti;
typedef UINT128 unsti;

#define RUN_TEST_SERIES(_t, _type, _name)                          \
  for (i = 0; i < (int)(sizeof(_t)/sizeof(_t[0])); ++i)            \
    {                                                              \
      _type r = _t[i].x;                                           \
      fprintf (stdout, #_name " (%DDgDL) in: %s:%d\n",             \
	       (_Decimal128) _t[i].x, __FILE__, __LINE__ - 1);     \
      _SC_P (__FILE__, _t[i].line, _t[i].e, stringify##_type (r)); \
    }

int
main (void)
{
  int i;

  RUN_TEST_SERIES (d128, ti, fixtdti);
  RUN_TEST_SERIES (d128uns, unsti, fixunstdti);
  RUN_TEST_SERIES (d64, ti, fixddti);
  RUN_TEST_SERIES (d64uns, unsti, fixunsddti);
  RUN_TEST_SERIES (d32, ti, fixsdti);
  RUN_TEST_SERIES (d32uns, unsti, fixunssdti);

  _REPORT ();

  /* fail comes from scaffold.c  */
  return fail;
}
