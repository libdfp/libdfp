/* Test istream operator in cpp compat header <dfp/decimal/decimal>.

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

/* This is a CPP compatibility testcase.  Pick up the _Decimal[32|64|128]
 * definitions.  */
#include <float.h>

#include <decimal/decimal>

#include <iomanip>

/* For cout support.  */
#include <iostream>

using namespace std;
using namespace std::decimal;

 /* Pick up the _OSC(x,y,tmp,fmt) macro.  */
#define _WANT_ISC 1
#include "scaffold.c"

/* Inspired by GLIBC stdio-common/tfformat.c  */
typedef struct{
  int line;
  decimal32 expect;
  const char *d;
  const char *fmt;
  decimal32 tmp;
} d32_type;

d32_type istream_d32s[] =
{
  {__LINE__, 0.0000006E-90DF, "6e-97", "%Ha", -1.0DF},
  {__LINE__, 0.0000006E-90DF, "6E-97", "%Ha", -1.0DF },
  /* Test where specified precision '10' exceeds __DEC32_MANT_DIG__.
   * This should reset precision to __DEC32_MANT_DIG__.  */
  {__LINE__, 0.6666666666E-90DF, "6.666667E-91", "%Ha", -1.0DF },
  {0,0,0,0,0}
};

typedef struct{
  int line;
  decimal64 expect;
  const char *d;
  const char *fmt;
  decimal64 tmp;
} d64_type;

d64_type istream_d64s[] =
{
  {__LINE__, -9.999E-3DD, "-0.009999", "%Da", -1.0DD},
  {__LINE__, -9.999E-3DD, "-9.999000e-03", "%Da", -1.0DD},
  {__LINE__, -9.999E-3DD, "-9.999E-03", "%Da", -1.0DD},
  {__LINE__, -9.999E-3DD, "-0.009999", "%Da", -1.0DD},
  {__LINE__, __builtin_infd64(), "inf", "%Da", -1.0DD},
  {__LINE__, __builtin_infd64(), "INF", "%Da", -1.0DD},
  {__LINE__, (0.0DD * __builtin_infd64()), "NAN", "%Da", -1.0DD},
  {__LINE__, (0.0DD * __builtin_infd64()), "nan", "%Da", -1.0DD},
  {0,0,0,0,0}
};

typedef struct{
  int line;
  decimal128 expect;
  const char *d;
  const char *fmt;
  decimal128 tmp;
} d128_type;

d128_type istream_d128s[] =
{
  {__LINE__, -1234.57DL, "-1234.57", "%DDa", -1.0DL},
  {__LINE__, -1234.57DL, "-1234.57", "%DDa", 0.0DL},
  {__LINE__, -1234.57DL, "-1234.57000", "%DDa", -1.0DL},
  {__LINE__, -1234.5679DL, "-1234.5679", "%DDa", -1.0DL},
  {__LINE__, -12345678912345678.9123455678DL, "-12345678912345678.9123455678","%DDa", -1.0DL},
  {0,0,0,0,0}
};

int main(void)
{
  d32_type *d32ptr;
  d64_type *d64ptr;
  d128_type *d128ptr;
  int fail_save = 0;
  for (d32ptr = istream_d32s; d32ptr->line; d32ptr++)
    {
      _ISC_P(__FILE__,d32ptr->line, d32ptr->expect,d32ptr->d,d32ptr->tmp,d32ptr->fmt);
    }

  for (d64ptr = istream_d64s; d64ptr->line; d64ptr++)
    {
      _ISC_P(__FILE__,d64ptr->line, d64ptr->expect,d64ptr->d,d64ptr->tmp,d64ptr->fmt);
    }

  for (d128ptr = istream_d128s; d128ptr->line; d128ptr++)
    {
      _ISC_P(__FILE__,d128ptr->line, d128ptr->expect,d128ptr->d,d128ptr->tmp, d128ptr->fmt);
    }

  _REPORT();

  /* fail comes from scaffold.c  */
  return fail;

}
