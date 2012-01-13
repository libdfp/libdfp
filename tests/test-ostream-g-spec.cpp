/* Test ostream operator 'g/G' spec conv in cpp compat header <dfp/decimal/decimal>.

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

/* This testcase sets _LIBDFP_G_CONV_SPEC which causes decimal/decimal's
 * operator<< to default to the equivalent of an printf g/G spec conv rather
 * than the DFP SPEC printf a/A spec conv.  */
#define _LIBDFP_G_CONV_SPEC
#include <decimal/decimal>

#include <iomanip>

/* For cout support.  */
#include <iostream>

using namespace std;
using namespace std::decimal;

 /* Pick up the _OSC(x,y,precision,upper,spec) macro.  */
#define _WANT_OSC 1
#include "scaffold.c"

/* Inspired by GLIBC stdio-common/tfformat.c  */
typedef struct{
  int line;
  decimal32 d;
  const char *expect;
  int precision; /* -1 means unspecified.  */
  char upper; /* l or u */
  char spec; /* a, e, f */
} d32_type;

d32_type ostream_d32s[] =
{
  {__LINE__, 0.0000006E-90DF, "6e-97", -1, 'l', 'g' },
  {__LINE__, 0.0000006E-90DF, "6E-97", -1, 'u', 'g' },
  /* Test where specified precision '10' exceeds __DEC32_MANT_DIG__.
   * This should reset precision to __DEC32_MANT_DIG__.  */
  {__LINE__, 0.6666666666E-90DF, "6.666667E-91", 10, 'u', 'g' },
  {__LINE__, 0.6666666666E-90DF, "6.666667e-91", 10, 'l', 'g' },
  {0,0,0,0,0,0}
};

typedef struct{
  int line;
  decimal64 d;
  const char *expect;
  int precision; /* -1 means unspecified.  */
  char upper; /* l or u */
  char spec; /* a, e, f */
} d64_type;

d64_type ostream_d64s[] =
{
  {__LINE__, -9.999E-3DD, "-0.009999", -1, 'l', 'g' },
  {__LINE__, -9.999E-3DD, "-0.009999", -1, 'u', 'g' },
  {__LINE__, -9.999992E-3DD, "-0.00999999", -1, 'l', 'g' },
  {__LINE__, __builtin_infd64(), "inf", -1, 'l', 'g' },
  {__LINE__, __builtin_infd64(), "INF", -1, 'u', 'g' },
  {__LINE__, (0.0DD * __builtin_infd64()), "NAN", -1, 'u', 'g' },
  {__LINE__, (0.0DD * __builtin_infd64()), "nan", -1, 'l', 'g' },
  {0,0,0,0,0,0}
};

typedef struct{
  int line;
  decimal128 d;
  const char *expect;
  int precision; /* -1 means unspecified.  */
  char upper; /* l or u */
  char spec; /* a, e, f */
} d128_type;

d128_type ostream_d128s[] =
{
  /* Known bug in printf (not truncating).  */
  {__LINE__, -1234.56789123456789123455678DL, "-1234.57", -1, 'l', 'g' },
  /* Known bug in printf (not truncating).  */
  {__LINE__, -1234.56789123456789123455678DL, "-1234.57", 6, 'l', 'g' },
  /* Known bug in printf (not truncating).  */
  {__LINE__, -1234.56789123456789123455678DL, "-1234.5679", 8, 'l', 'g' },
  {__LINE__, -12345678912345678.9123455678DL, "-1.234567891E+16", 10, 'u', 'g' },
  {__LINE__, -12345678912345678.9123455678DL, "-1.234567891e+16", 10, 'l', 'g' },
  {0,0,0,0,0,0}
};

int main(void)
{

  d32_type *d32ptr;
  d64_type *d64ptr;
  d128_type *d128ptr;
  for (d32ptr = ostream_d32s; d32ptr->line; d32ptr++)
    {
      _OSC_P(__FILE__,d32ptr->line, d32ptr->expect,d32ptr->d,d32ptr->precision,d32ptr->upper,d32ptr->spec);
    }

  for (d64ptr = ostream_d64s; d64ptr->line; d64ptr++)
    {
      _OSC_P(__FILE__,d64ptr->line, d64ptr->expect,d64ptr->d,d64ptr->precision,d64ptr->upper,d64ptr->spec);
    }

  for (d128ptr = ostream_d128s; d128ptr->line; d128ptr++)
    {
      _OSC_P(__FILE__,d128ptr->line, d128ptr->expect,d128ptr->d,d128ptr->precision,d128ptr->upper,d128ptr->spec);
    }

  _REPORT();

  /* fail comes from scaffold.c  */
  return fail;

}
