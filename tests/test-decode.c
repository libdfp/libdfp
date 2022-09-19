/* Test decoded facility.

   Copyright (C) 2009-2015 Free Software Foundation, Inc.

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
   if not, write to the Free Software Foundation, Inc., 51 Franklin
   Street, Fifth Floor, Boston, MA 02110-1301 USA.

   Please see libdfp/COPYING.txt for more information.  */

#ifndef __STDC_WANT_DEC_FP__
# define __STDC_WANT_DEC_FP__
#endif

#include <float.h>
#include <stdio.h>
#include <wchar.h>

#define _WANT_DC 1
#include "scaffold.c"		/* Pick up the _DC_P(x,y,...) macro.  */

typedef struct
{
  int line;
  _Decimal128 d;
  const char *expect;
} d128_type;

static const
d128_type d128[] =
{
  {__LINE__,                        __DEC128_MIN__,
             "+0,000,000,000,000,000,000,000,000,000,000,001E-6143" },
  {__LINE__,                        __DEC128_MAX__,
             "+9,999,999,999,999,999,999,999,999,999,999,999E+6111"},
  {__LINE__,               -__builtin_nand128 (""),
             "-0,000,000,000,000,000,000,000,000,000,000,000E-6176"},
  {__LINE__,                  __builtin_infd128 (),
             "+0,000,000,000,000,000,000,000,000,000,000,000E-6176"},
  {__LINE__,                        __DEC128_MIN__,
             "+0,000,000,000,000,000,000,000,000,000,000,001E-6143"},
  {__LINE__,              __DEC128_SUBNORMAL_MIN__,
             "+0,000,000,000,000,000,000,000,000,000,000,001E-6176"},
  {__LINE__,                                9E70DL,
             "+0,000,000,000,000,000,000,000,000,000,000,009E+70"},
  {__LINE__,                                4E84DL,
             "+0,000,000,000,000,000,000,000,000,000,000,004E+84"},
  {__LINE__,                           1234567E84DL,
	     "+0,000,000,000,000,000,000,000,000,001,234,567E+84"},
  {__LINE__,                                 1.00DL,
	     "+0,000,000,000,000,000,000,000,000,000,000,100E-2"   },
  {__LINE__, -8.958607314841774959249800028756976DL,
             "-8,958,607,314,841,774,959,249,800,028,756,976E-33"  },
};
static const int d128s = sizeof (d128) / sizeof (d128[0]);


typedef struct
{
  int line;
  _Decimal64 d;
  const char *expect;
} d64_type;

static const
d64_type d64[] =
{
  {__LINE__, 1.00DD,                  "+0,000,000,000,000,100E-2"},
  {__LINE__, __DEC64_MIN__,           "+0,000,000,000,000,001E-383"},
  {__LINE__, __DEC64_MAX__,           "+9,999,999,999,999,999E+369"},
  {__LINE__, -__builtin_nand64(""),   "-0,000,000,000,000,000E-398"},
  {__LINE__,  __builtin_infd64(),     "+0,000,000,000,000,000E-398"},
  {__LINE__, 9.999999999999999E369DD, "+9,999,999,999,999,999E+354"},
  {__LINE__, 9.999999999999999E370DD, "+9,999,999,999,999,999E+355"},
  {__LINE__, 9.999999999999999E384DD, "+9,999,999,999,999,999E+369"},
  {__LINE__, __DEC64_MIN__,           "+0,000,000,000,000,001E-383"},
  {__LINE__, __DEC64_SUBNORMAL_MIN__, "+0,000,000,000,000,001E-398"},
  {__LINE__, 1E-398DD,                "+0,000,000,000,000,001E-398"},
  {__LINE__, 9E370DD,                 "+0,000,000,000,000,090E+369"},
  {__LINE__, 4E384DD,                 "+4,000,000,000,000,000E+369"},
};
static const int d64s = sizeof (d64) / sizeof (d64[0]);


typedef struct
{
  int line;
  _Decimal32 d;
  const char *expect;
} d32_type;

static const
d32_type d32[] =
{
  {__LINE__, 1.00DF,                  "+0,000,100E-2"},
  {__LINE__, __DEC32_MIN__,           "+0,000,001E-95"},
  {__LINE__, __DEC32_MAX__,           "+9,999,999E+90"},
  {__LINE__, -__builtin_nand32(""),   "-0,000,000E-101"},
  {__LINE__,  __builtin_infd32(),     "+0,000,000E-101"},
  {__LINE__, __DEC32_MIN__,           "+0,000,001E-95"},
  {__LINE__, __DEC32_SUBNORMAL_MIN__, "+0,000,001E-101"},
  {__LINE__, 9E70DF,                  "+0,000,009E+70"},
  {__LINE__, 4E84DF,                  "+0,000,004E+84"},
  {__LINE__, 1234567E84DF,            "+1,234,567E+84"},
};
static const int d32s = sizeof (d32) / sizeof (d32[0]);

int
main (void)
{
  int i;

  for (i=0; i<d128s; ++i)
    {
      _DC_P (__FILE__, d128[i].line, d128[i].expect, d128[i].d);
    }

  for (i=0; i<d64s; ++i)
    {
      _DC_P (__FILE__, d64[i].line, d64[i].expect, d64[i].d);
    }

  for (i=0; i<d32s; ++i)
    {
      _DC_P (__FILE__, d32[i].line, d32[i].expect, d32[i].d);
    }

  _REPORT ();

  /* fail comes from scaffold.c  */
  return fail;
}
