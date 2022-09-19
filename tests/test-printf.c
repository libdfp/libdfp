/* Test printf_dfp facility.

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
#define __STDC_WANT_DEC_FP__ 1
#endif

#include <fenv.h>
#include <float.h>
#include <stdio.h>
#include <wchar.h> /* This should pick up the libdfp wchar in dfp/wchar.h.  */
#include <stdlib.h>

#define _WANT_PC 1 /* Pick up the _PC(x,y,...) macro.  */
#include "scaffold.c"

#include "decode.h"

/* Inspired by GLIBC stdio-common/tfformat.c  */
typedef struct{
  int line;
  _Decimal32 d;
  const char *expect;
  const char *format;
  const char *sf_format;
} d32_type;

d32_type printf_d32s[] =
{
  /* Default precision.  */
  {__LINE__, 0.0004E-4DF, "0.00000004", "%.8Hf", "%.8f"},
  {__LINE__, 123.456E-6DF, "0.000123", "%Hf", "%f"},
  {__LINE__, 123.456E-6DF, "1.234560e-04", "%He", "%e"},
  {__LINE__, 12.0E-15DF, "0.000000", "%Hf", "%f"},
  {__LINE__, 12.0E-5DF, "0.000120", "%Hf", "%f"},
  {__LINE__, 12.0E-15DF, "1.2e-14", "%Hg", "%g"},
  {__LINE__, 0.E+0DF, "0.000000", "%Hf", "%f"},
  {__LINE__, 1.E+2DF, "100.000000", "%Hf", "%f"},
  {__LINE__, 12.0E+3DF, "1.200000e+04", "%He", "%e"},
  {__LINE__, 12.000E+3DF, "1.200000e+04", "%He", "%e"},
  {__LINE__, 12.0E+3DF, "12000.000000", "%Hf", "%f"},
  {__LINE__, 1900000.E+2DF, "190000000.000000", "%Hf", "%f"},
  {__LINE__, 1900000.E+2DF, "190000000", "%.Hf", "%.f"},
  {__LINE__, 19.E+2DF, "1900.000000", "%Hf", "%f"},
  /* Default precision.  */
  {__LINE__, 1.0E+2DF, "100.000000", "%Hf", "%f"},
  /* Default precision.  */
  {__LINE__, 1.00E+2DF, "100.000000", "%Hf", "%f"},
  {__LINE__, 23456.0E-3DF, "2.35e+01", "%.2He", "%.2e"},
  {__LINE__, 23456.0E-3DF, "23.46", "%.2Hf", "%.2f"},
  {__LINE__, 23456.E-3DF, "23.46", "%.2Hf", "%.2f"},
  {__LINE__, 1234567.E0DF, "1234567.00", "%.2Hf", "%.2f"},
  {__LINE__, 1234567.E0DF, "1234567.000000", "%Hf", "%f"},
  /* Precision of '3'.  */
  {__LINE__, 123.456E+0DF, "123.456000", "%Hf", "%f"},
  {__LINE__, 123.456E+0DF, "123.456", "%.3Hf", "%.3f"},
  /* Implict precision of '1' since there is one explicit zero in the mantissa
   * right of the decimal place after it is adjusted for the exponent.  */
  {__LINE__, 1.000E+2DF, "100.000000", "%Hf", "%f"},
  /* Implict precision of '1' since there is one explicit zero in the mantissa
   * right of the decimal place after it is adjusted for the exponent.  */
  {__LINE__, 100.0E+0DF, "100.000000", "%Hf", "%f"},
  /* Implict precision of '1' since there is one explicit zero in the mantissa
   * right of the decimal place after it is adjusted for the exponent.  */
  {__LINE__, 0.0001000E+6DF, "100.000000", "%Hf", "%f"},
  /* Implict precision of '4' since there is one explicit zero in the mantissa
   * right of the decimal place after it is adjusted for the exponent.  */
  {__LINE__, 1.000000E+2DF, "100.000000", "%Hf", "%f"},
  /* Because of the 'e' output precision is '5'.  */
  {__LINE__, 123.456E+0DF, "1.234560e+02", "%He", "%e"},
  {__LINE__, 123.456E+0DF, "1.23456e+02", "%.5He", "%.5e"},
  {__LINE__, 123.456E+0DF, "1.234560E+02", "%HE", "%E"},
  {__LINE__, 19000E+15DF, "19000000000000000000.000000", "%Hf", "%f"},
  {__LINE__, 190001E+45DF, "190001000000000000000000000000000000000000000000000.000000", "%Hf", "%f"},
  {__LINE__, -123.456E+0DF, "-1.234560e+02", "%He", "%e"},
  /* Encoded as 1234567E1, prec 5*/
  {__LINE__, 12.34567E+6DF, "12345670.000000", "%Hf", "%f"},

  {__LINE__, 123.456E-5DF, "1.234560e-03", "%He", "%e"},
  {__LINE__, 123.456E-5DF, "0.001235", "%Hf", "%f"},
  {__LINE__, 123.456E-5DF, "0.00123456", "%Hg", "%g"},

  {__LINE__, 123.456E-6DF, "1.234560e-04", "%He", "%e"},
  {__LINE__, 123.456E-6DF, "0.000123", "%Hf", "%f"},
  {__LINE__, 123.456E-6DF, "0.000123456", "%Hg", "%g"},

  {__LINE__, 123.456E-7DF, "1.234560e-05", "%He", "%e"},
  {__LINE__, 123.456E-7DF, "0.000012", "%Hf", "%f"},
  {__LINE__, 123.456E-7DF, "1.23456e-05", "%Hg", "%g"},
  {__LINE__, 123.456E-7DF, "1.235e-05", "%.4Hg", "%.4g"},
  {__LINE__, 123.456E-9DF, "1.234560e-07", "%He", "%e"},

  {__LINE__, 6.5DF, "6.5e+00", "%.1He", "%.1e"},
  {__LINE__, 6.5DF, "6e+00", "%.0He", "%.0e"},
  {__LINE__, 6.6DF, "7e+00", "%.0He", "%.0e"},

  {__LINE__, 123.456E-9DF, "1.23456e-7", "%Ha", "%a"},
  {__LINE__, 0.00000E-101DF, "0.000000e+00", "%He", "%e"},

  /* Erroneously prints "1e+6" due to the '.' with no explicit precision. FIX
   * ME.  */
  {__LINE__, 6.0E5DF, "6e+5", "%.Ha", "%.a"},
  {__LINE__, 6.0E5DF, "6e+5", "%.0Ha", "%.0a"},
  {__LINE__, 6.E5DF, "6e+5", "%.0Ha", "%.0a"},

  {__LINE__, 0.0e10DF, "0.000000e+00", "%He", "%e"},
  {__LINE__, 0.0e10DF, "0.000000", "%Hf", "%f"},

  {__LINE__, 0.0e10DF, "0", "%Hg", "%g"},
  /* For 'g' when '#' is present the spec says that trailing zeros are NOT
   * removed.  And YES, this SHOULD be five trailing zeros because this falls
   * into the category where 'g' is converted to 'e' and precision equals P - 1.
   * Additionally, the C-spec indicates that.  This is congruent with how this
   * works for binary float.  */
  {__LINE__, 0.0e10DF, "0.00000", "%#Hg", NULL},
  {__LINE__, 0.0e10DF, "0", "%.4Hg", "%.4g"},

  /* (1,0,-1)  */
  {__LINE__, 0.0DF, "0.0", "%Ha", "%a"},
  /* (1,0,0)  */
  {__LINE__, 0e0DF, "0", "%Ha", "%a"},

  /* (1,0,0)  */
  {__LINE__, -0e0DF, "-0", "%Ha", "%a"},
  /* (-1,0,-1)  */
  {__LINE__, -0.0DF, "-0.0", "%Ha", "%a"},

  /* ( 1, 123, 0)  */
  {__LINE__, 123.DF, "123", "%Ha", "%a"},

  /* (-1, 123, 0) -123  */
  {__LINE__, -123.DF, "-123", "%Ha", "%a"},

  /* ( 1, 123, -2) 1.23  */
  {__LINE__, 1.23DF, "1.23", "%Ha", "%a"},
  {__LINE__, 123E-2DF, "1.23", "%Ha", "%a"},
  {__LINE__, 0.123E1DF, "1.23", "%Ha", "%a"},

  /* ( 1, 123, 1) 1.23e+3  */
  {__LINE__, 123e1DF, "1.23e+3", "%Ha", "%a"},

  /* ( 1, 1230, 0) 1230  */
  {__LINE__, 123.0e1DF, "1230", "%Ha", "%a"},

  /* (-1, 123, 1) -1.23e+3  */
  {__LINE__, -123e1DF, "-1.23e+3", "%Ha", "%a"},

  /* ( 1, 123, -8) 0.00000123  */
  {__LINE__, 123e-8DF, "0.00000123", "%Ha", "%a"},
  {__LINE__, 123.e-8DF, "0.00000123", "%Ha", "%a"},

  /* ( 1, 123, -9) 1.23e-7  */
  {__LINE__, 123e-9DF, "1.23e-7", "%Ha", "%a"},
  {__LINE__, 123.e-9DF, "1.23e-7", "%Ha", "%a"},

  /* Can't represent the following in _Decimal32  */
  /* ( 1, 1234567890123456, 0) 1234567890123456  */
  /* ( 1, 1234567890123456, 1) 1.234567890123456e+16  */
  /* ( 1, 1234567890123456, -1) 123456789012345.6  */
  /* ( 1, 1234567890123456, -21) 0.000001234567890123456  */
  /* ( 1, 1234567890123456, -22) 1.234567890123456e-7  */

  /* ( 1, 0, 0) 0  */
  {__LINE__, 0e0DF, "0", "%Ha", "%a"},
  {__LINE__, 0.DF, "0", "%Ha", "%a"},
  {__LINE__, 0.e0DF, "0", "%Ha", "%a"},

  /* (-1, 0, 0) -0  */
  {__LINE__, -0e0DF, "-0", "%Ha", "%a"},
  {__LINE__, -0.DF, "-0", "%Ha", "%a"},
  {__LINE__, -0.e0DF, "-0", "%Ha", "%a"},


  /* ( 1, 5, -6) 0.000005  */
  {__LINE__, 5e-6DF, "0.000005", "%Ha", "%a"},

  /* ( 1, 50, -7) 0.0000050  */
  {__LINE__, 50e-7DF, "0.0000050", "%Ha", "%a"},

  /* ( 1, 5, -7) 5e-7  */
  {__LINE__, 5e-7DF, "5e-7", "%Ha", "%a"},

  /* Yes, goofy, but it is encoded 00000000e0.  */
  {__LINE__, 0.00e2DF, "0", "%Ha", "%a"},

  /* Yes, goofy, but it is encoded 00000000e0.  */
  {__LINE__, 0.00e2DF, "0", "%.2Ha", "%.2a"},

  /* Notice the '#'.  The DFP spec doesn't speak about this and doesn't indicate
   * that this is supposed to work like it does for 'g' so we still expect to
   * see trailing zeros removed and no exponent.  */
  {__LINE__, 0.00e2DF, "0.", "%#Ha", NULL},

  /* ( 1, 5, -7)  */
  {__LINE__, 5e-7DF, "5.e-7", "%#Ha", NULL},

  {__LINE__, 6543.E-11DF, "6.543e-8", "%Ha", "%a"},

  /* Continued DPF spec examples.  */
  {__LINE__, 6543.00DF, "6543.00", "%Ha", "%a"},
  {__LINE__, 6543.00DF, "6543.00", "%.6Ha", "%.6a"},
  {__LINE__, 6543.00DF, "6543.0", "%.5Ha", "%.5a"},
  {__LINE__, 6543.09DF, "6543.1", "%.5Ha", "%.5a"},
  {__LINE__, 6543.00DF, "6543", "%.4Ha", "%.4a"},
  {__LINE__, 6543.00DF, "6.54e+3", "%.3Ha", "%.3a"},
  {__LINE__, 6543.00DF, "6.5e+3", "%.2Ha", "%.2a"},
  {__LINE__, 6543.00DF, "7e+3", "%.1Ha", "%.1a"},
  {__LINE__, 6543.00DF, "7E+3", "%.1HA", "%.1A"},

  {__LINE__, 6543.00DF, " 6543.00", "%8Ha", NULL},
  /* The following have shown inconsistent results between soft-dfp and
   * hard-dfp.  */

  /* In reality this exceeds the ability of _Decimal32 to represent the exponent
   * so while it SHOULD be 0e-106 it really ends up being encoded as 0e-101
   * because that is the min exponent.  */
  {__LINE__, 0.00000E-101DF, "0e-101", "%Ha", "%a"},
  /* Likewise, this still exceeds.  */
  {__LINE__, 0.00000E-97DF, "0e-101", "%Ha", "%a"},

  /* As shown, this is 0e-101 for real.  */
  {__LINE__, 0.00000E-96DF, "0e-101", "%Ha", "%a"},

  {__LINE__, 0.00000E-95DF, "0e-100", "%Ha", "%a"},

  {__LINE__, 0e10DF, "0e+10", "%Ha", "%a"},

  {__LINE__, 0.0e10DF, "0e+9", "%.1Ha", "%.1a"},

  {__LINE__, 0.0e-09DF, "0e-10", "%.0Ha", "%.0a"},

  /* ( 1, 0, -6) 0.000000  */
  {__LINE__, 0e-6DF, "0.000000", "%Ha", "%a"},

  /* ( 1, 0, -7) 0e-7  */
  {__LINE__, 0e-7DF, "0e-7", "%Ha", "%a"},
  {__LINE__, 0.e-10DF, "0e-10", "%.1Ha", "%.1a"},
  {__LINE__, 0.e-10DF, "0e-10", "%.3Ha", "%.3a"},

  {__LINE__, 0.e-10DF, "0e-10", "%.4Ha", "%.4a"},

  /* ( 1, 0, 2) 0e+2 encoded as 00000000e2.  */
  {__LINE__, 0e2DF, "0e+2", "%Ha", "%a"},

  /* Yes, goofy, but it is encoded 00000000e1.  */
  {__LINE__, 0.0e2DF, "0e+1", "%Ha", "%a"},
  {__LINE__, 0.0e10DF, "0e+9", "%Ha", "%a"},

  {0,0,0,0,0 }
};

typedef struct{
  int line;
  _Decimal64 d;
  const char *expect;
  const char *format;
  const char *sf_format;
} d64_type;

d64_type printf_d64s[] =
{
  /* Default precision.  */
  {__LINE__, 123.456E-6DD, "0.000123", "%Df", "%f"},
  {__LINE__, 123.456E-6DD, "1.234560e-04", "%De", "%e"},
  {__LINE__, 12.0E-15DD, "0.000000", "%Df", "%f"},
  {__LINE__, 12.0E-5DD, "0.000120", "%Df", "%f"},
  {__LINE__, 12.0E-15DD, "1.2e-14", "%Dg", "%g"},
  {__LINE__, 0.E+0DD, "0.000000", "%Df", "%f"},
  {__LINE__, 1.E+2DD, "100.000000", "%Df", "%f"},
  {__LINE__, 12.0E+3DD, "1.200000e+04", "%De", "%e"},
  {__LINE__, 12.000E+3DD, "1.200000e+04", "%De", "%e"},
  {__LINE__, 12.0E+3DD, "12000.000000", "%Df", "%f"},
  {__LINE__, 1900000.E+2DD, "190000000.000000", "%Df", "%f"},
  {__LINE__, 1900000.E+2DD, "190000000", "%.Df", "%.f"},
  {__LINE__, 19.E+2DD, "1900.000000", "%Df", "%f"},
  /* Default precision.  */
  {__LINE__, 1.0E+2DD, "100.000000", "%Df", "%f"},
  /* Default precision.  */
  {__LINE__, 1.00E+2DD, "100.000000", "%Df", "%f"},
  {__LINE__, 23456.0E-3DD, "2.35e+01", "%.2De", "%.2e"},
  {__LINE__, 23456.0E-3DD, "23.46", "%.2Df", "%.2f"},
  {__LINE__, 23456.E-3DD, "23.46", "%.2Df", "%.2f"},
  {__LINE__, 1234567.E0DD, "1234567.00", "%.2Df", "%.2f"},
  {__LINE__, 1234567.E0DD, "1234567.000000", "%Df", "%f"},
  /* Precision of '3'.  */
  {__LINE__, 123.456E+0DD, "123.456000", "%Df", "%f"},
  {__LINE__, 123.456E+0DD, "123.456", "%.3Df", "%.3f"},
  /* Implict precision of '1' since there is one explicit zero in the mantissa
   * right of the decimal place after it is adjusted for the exponent.  */
  {__LINE__, 1.000E+2DD, "100.000000", "%Df", "%f"},
  /* Implict precision of '1' since there is one explicit zero in the mantissa
   * right of the decimal place after it is adjusted for the exponent.  */
  {__LINE__, 100.0E+0DD, "100.000000", "%Df", "%f"},
  /* Implict precision of '1' since there is one explicit zero in the mantissa
   * right of the decimal place after it is adjusted for the exponent.  */
  {__LINE__, 0.0001000E+6DD, "100.000000", "%Df", "%f"},
  /* Implict precision of '4' since there is one explicit zero in the mantissa
   * right of the decimal place after it is adjusted for the exponent.  */
  {__LINE__, 1.000000E+2DD, "100.000000", "%Df", "%f"},
  /* Because of the 'e' output precision is '5'.  */
  {__LINE__, 123.456E+0DD, "1.234560e+02", "%De", "%e"},
  {__LINE__, 123.456E+0DD, "1.23456e+02", "%.5De", "%.5e"},
  {__LINE__, 123.456E+0DD, "1.234560E+02", "%DE", "%E"},
  {__LINE__, 19000E+15DD, "19000000000000000000.000000", "%Df", "%f"},
  {__LINE__, 190001E+45DD, "190001000000000000000000000000000000000000000000000.000000", "%Df", "%f"},
  {__LINE__, -123.456E+0DD, "-1.234560e+02", "%De", "%e"},
  /* Encoded as 1234567E1, prec 5*/
  {__LINE__, 12.34567E+6DD, "12345670.000000", "%Df", "%f"},

  {__LINE__, 123.456E-5DD, "1.234560e-03", "%De", "%e"},
  {__LINE__, 123.456E-5DD, "0.001235", "%Df", "%f"},
  {__LINE__, 123.456E-5DD, "0.00123456", "%Dg", "%g"},

  {__LINE__, 123.456E-6DD, "1.234560e-04", "%De", "%e"},
  {__LINE__, 123.456E-6DD, "0.000123", "%Df", "%f"},
  {__LINE__, 123.456E-6DD, "0.000123456", "%Dg", "%g"},

  {__LINE__, 123.456E-7DD, "1.234560e-05", "%De", "%e"},
  {__LINE__, 123.456E-7DD, "0.000012", "%Df", "%f"},
  {__LINE__, 123.456E-7DD, "1.23456e-05", "%Dg", "%g"},
  {__LINE__, 123.456E-7DD, "1.235e-05", "%.4Dg", "%.4g"},
  {__LINE__, 123.456E-9DD, "1.234560e-07", "%De", "%e"},

  {__LINE__, 6.5DD, "6.5e+00", "%.1De", "%.1e"},
  {__LINE__, 6.5DD, "6e+00", "%.0De", "%.0e"},
  {__LINE__, 6.6DD, "7e+00", "%.0De", "%.0e"},

  /* Erroneously prints "1e+6" due to the '.' with no explicit precision. Fix
   * ME.  */
  {__LINE__, 6.0E5DD, "6e+5", "%.Da", "%.a"},

  {__LINE__, 123.456E-9DD, "1.23456e-7", "%Da", "%a"},
  {__LINE__, 0.00000E-101DD, "0.000000e+00", "%De", "%e"},

  {__LINE__, 0.e-101DD, "0e-101", "%Da", "%a"},

  /* This returns 0e-106 because the zeros are significant.  */
  {__LINE__, 0.00000E-101DD, "0e-106", "%Da", "%a"},

  {__LINE__, 0.0e10DD, "0.000000e+00", "%De", "%e"},
  {__LINE__, 0.0e10DD, "0.000000", "%Df", "%f"},

  {__LINE__, 0.0e10DD, "0", "%Dg", "%g"},
  /* For 'g' when '#' is present the spec says that trailing zeros are NOT
   * removed.  And YES, this SHOULD be five trailing zeros because this falls
   * into the category where 'g' is converted to 'e' and precision equals P - 1.
   * Additionally, the C-spec indicates that.  This is congruent with how this
   * works for binary float.  */
  {__LINE__, 0.0e10DD, "0.00000", "%#Dg", NULL},
  {__LINE__, 0.0e10DD, "0", "%.4Dg", "%.4g"},

  /* (1,0,-1)  */
  {__LINE__, 0.0DD, "0.0", "%Da", "%a"},
  {__LINE__, 0e10DD, "0e+10", "%Da", "%a"},

  {__LINE__, 0.0e10DD, "0e+9", "%.1Da", "%.1a"},
  {__LINE__, 0.0e-09DD, "0e-10", "%.0Da", "%.0a"},
  /* (1,0,0)  */
  {__LINE__, 0e0DD, "0", "%Da", "%a"},

  /* (1,0,0)  */
  {__LINE__, -0e0DD, "-0", "%Da", "%a"},
  /* (-1,0,-1)  */
  {__LINE__, -0.0DD, "-0.0", "%Da", "%a"},

  /* ( 1, 123, 0)  */
  {__LINE__, 123.DD, "123", "%Da", "%a"},

  /* (-1, 123, 0) -123  */
  {__LINE__, -123.DD, "-123", "%Da", "%a"},

  /* ( 1, 123, -2) 1.23  */
  {__LINE__, 1.23DD, "1.23", "%Da", "%a"},
  {__LINE__, 123E-2DD, "1.23", "%Da", "%a"},
  {__LINE__, 0.123E1DD, "1.23", "%Da", "%a"},

  /* ( 1, 123, 1) 1.23e+3  */
  {__LINE__, 123e1DD, "1.23e+3", "%Da", "%a"},

  /* ( 1, 1230, 0) 1230  */
  {__LINE__, 123.0e1DD, "1230", "%Da", "%a"},

  /* (-1, 123, 1) -1.23e+3  */
  {__LINE__, -123e1DD, "-1.23e+3", "%Da", "%a"},

  /* ( 1, 123, -8) 0.00000123  */
  {__LINE__, 123e-8DD, "0.00000123", "%Da", "%a"},
  {__LINE__, 123.e-8DD, "0.00000123", "%Da", "%a"},

  /* ( 1, 123, -9) 1.23e-7  */
  {__LINE__, 123e-9DD, "1.23e-7", "%Da", "%a"},
  {__LINE__, 123.e-9DD, "1.23e-7", "%Da", "%a"},

  /* Can represent the following in _Decimal64  */
  /* ( 1, 1234567890123456, 0) 1234567890123456  */
  {__LINE__, 1234567890123456e0DD, "1234567890123456", "%Da", "%a"},

  /* ( 1, 1234567890123456, 1) 1.234567890123456e+16  */
  {__LINE__, 1234567890123456e1DD, "1.234567890123456e+16", "%Da", "%a"},

  /* ( 1, 1234567890123456, -1) 123456789012345.6  */
  {__LINE__, 1234567890123456e-1DD, "123456789012345.6", "%Da", "%a"},

  /* ( 1, 1234567890123456, -21)    0.000001234567890123456  */
  {__LINE__, 1234567890123456e-21DD, "0.000001234567890123456", "%Da", "%a"},

  /* ( 1, 1234567890123456, -22)    1.234567890123456e-7  */
  {__LINE__, 1234567890123456e-22DD, "1.234567890123456e-7", "%Da", "%a"},

  /* ( 1, 0, 0) 0  */
  {__LINE__, 0e0DD, "0", "%Da", "%a"},
  {__LINE__, 0.DD, "0", "%Da", "%a"},
  {__LINE__, 0.e0DD, "0", "%Da", "%a"},

  /* (-1, 0, 0) -0  */
  {__LINE__, -0e0DD, "-0", "%Da", "%a"},
  {__LINE__, -0.DD, "-0", "%Da", "%a"},
  {__LINE__, -0.e0DD, "-0", "%Da", "%a"},

  /* ( 1, 0, -6) 0.000000  */
  {__LINE__, 0e-6DD, "0.000000", "%Da", "%a"},

  /* ( 1, 0, -7) 0e-7  */
  {__LINE__, 0e-7DD, "0e-7", "%Da", "%a"},
  {__LINE__, 0.e-10DD, "0e-10", "%.1Da", "%.1a"},
  {__LINE__, 0.e-10DD, "0e-10", "%.3Da", "%.3a"},
  {__LINE__, 0.e-10DD, "0e-10", "%.4Da", "%.4a"},


  /* ( 1, 5, -6) 0.000005  */
  {__LINE__, 5e-6DD, "0.000005", "%Da", "%a"},

  /* ( 1, 50, -7) 0.0000050  */
  {__LINE__, 50e-7DD, "0.0000050", "%Da", "%a"},

  /* ( 1, 5, -7) 5e-7  */
  {__LINE__, 5e-7DD, "5e-7", "%Da", "%a"},

  /* ( 1, 0, 2) 0e+2 encoded as 00000000e2.  */
  {__LINE__, 0e2DD, "0e+2", "%Da", "%a"},

  /* Yes, goofy, but it is encoded 00000000e1.  */
  {__LINE__, 0.0e2DD, "0e+1", "%Da", "%a"},

  {__LINE__, 0.0e10DD, "0e+9", "%Da", "%a"},

  /* Yes, goofy, but it is encoded 00000000e0.  */
  {__LINE__, 0.00e2DD, "0", "%Da", "%a"},

  /* Yes, goofy, but it is encoded 00000000e0.  */
  {__LINE__, 0.00e2DD, "0", "%.2Da", "%.2a"},

  /* Notice the '#'.  The DDP spec doesn't speak about this and doesn't indicate
   * that this is supposed to work like it does for 'g' so we still expect to
   * see trailing zeros removed and no exponent.  */
  {__LINE__, 0.00e2DD, "0.", "%#Da", NULL},

  /* ( 1, 5, -7)  */
  {__LINE__, 5e-7DD, "5.e-7", "%#Da", NULL},

  {__LINE__, 6543.E-11DD, "6.543e-8", "%Da", "%a"},

  /* Continued DPF spec examples.  */
  {__LINE__, 6543.00DD, "6543.00", "%Da", "%a"},
  {__LINE__, 6543.00DD, "6543.00", "%.6Da", "%.6a"},
  {__LINE__, 6543.00DD, "6543.0", "%.5Da", "%.5a"},
  {__LINE__, 6543.00DD, "6543", "%.4Da", "%.4a"},
  {__LINE__, 6543.00DD, "6.54e+3", "%.3Da", "%.3a"},
  {__LINE__, 6543.00DD, "6.5e+3", "%.2Da", "%.2a"},
  {__LINE__, 6543.00DD, "7e+3", "%.1Da", "%.1a"},
  {__LINE__, 6543.00DD, "7E+3", "%.1DA", "%.1A"},

  {__LINE__, 6543.00DD, " 6543.00", "%8Da", NULL},

  {__LINE__, __DEC64_MAX__, "9.999999999999999e+384", "%.17Dg", "%.17g" },
  {__LINE__, __DEC64_MAX__, "9.999999999999999e+384", "%Da", "%a" },
  {__LINE__, __DEC64_MAX__, "1.000000e+385", "%De", "%e" },
  {__LINE__, __DEC64_MAX__, "9.999999999999999e+384", "%.16Da", "%.16a" },
  {__LINE__, __DEC64_MAX__, "1.00e+385", "%.3Da", "%.3a" },
  {__LINE__, 9.999999999999999E16DD, "9.999999999999999e+16", "%.16Dg", "%.16g" },
  {__LINE__, 9.999999999999999E23DD, "9.999999999999999e+23", "%.16Dg", "%.16g" },
  {__LINE__, 9.999999999999999E300DD, "9.999999999999999e+300", "%.16Dg", "%.16g" },
  {__LINE__, 9.999999999999999E360DD, "9.999999999999999e+360", "%.16Dg", "%.16g" },
  {__LINE__, 9.999999999999999E369DD, "9.999999999999999e+369", "%.16Dg", "%.16g" },
  {__LINE__, 9.999999999999999E370DD, "9.999999999999999e+370", "%.16Dg", "%.16g" },
  {__LINE__, 9.999999999999999E379DD, "9.999999999999999e+379", "%.16Dg", "%.16g" },
  {__LINE__, 9.999999999999999E380DD, "9.999999999999999e+380", "%.16Dg", "%.16g" },
  {__LINE__, 9999999999999999E369DD, "9.999999999999999e+384", "%.16Dg", "%.16g" },
  {__LINE__, 4E384DD, "4e+384", "%Dg", "%g" },
  {__LINE__, 4E384DD, "4e+384", "%.Dg", "%.g" },


  {0,0,0,0,0 }
};

typedef struct{
  int line;
  _Decimal128 d;
  const char *expect;
  const char *format;
  const char *sf_format;
} d128_type;

d128_type printf_d128s[] =
{
  /* Default precision.  */
  {__LINE__, 123.456E-6DL, "0.000123", "%DDf", "%f"},
  {__LINE__, 123.456E-6DL, "1.234560e-04", "%DDe", "%e"},
  {__LINE__, 12.0E-15DL, "0.000000", "%DDf", "%f"},
  {__LINE__, 12.0E-5DL, "0.000120", "%DDf", "%f"},
  {__LINE__, 12.0E-15DL, "1.2e-14", "%DDg", "%g"},
  {__LINE__, 0.E+0DL, "0.000000", "%DDf", "%f"},
  {__LINE__, 1.E+2DL, "100.000000", "%DDf", "%f"},
  {__LINE__, 12.0E+3DL, "1.200000e+04", "%DDe", "%e"},
  {__LINE__, 12.000E+3DL, "1.200000e+04", "%DDe", "%e"},
  {__LINE__, 12.0E+3DL, "12000.000000", "%DDf", "%f"},
  {__LINE__, 1900000.E+2DL, "190000000.000000", "%DDf", "%f"},
  {__LINE__, 1900000.E+2DL, "190000000", "%.DDf", "%.f"},
  {__LINE__, 19.E+2DL, "1900.000000", "%DDf", "%f"},
  /* Default precision.  */
  {__LINE__, 1.0E+2DL, "100.000000", "%DDf", "%f"},
  /* Default precision.  */
  {__LINE__, 1.00E+2DL, "100.000000", "%DDf", "%f"},
  {__LINE__, 23456.0E-3DL, "2.35e+01", "%.2DDe", "%.2e"},
  {__LINE__, 23456.0E-3DL, "23.46", "%.2DDf", "%.2f"},
  {__LINE__, 23456.E-3DL, "23.46", "%.2DDf", "%.2f"},
  {__LINE__, 1234567.E0DL, "1234567.00", "%.2DDf", "%.2f"},
  {__LINE__, 1234567.E0DL, "1234567.000000", "%DDf", "%f"},
  /* Precision of '3'.  */
  {__LINE__, 123.456E+0DL, "123.456000", "%DDf", "%f"},
  {__LINE__, 123.456E+0DL, "123.456", "%.3DDf", "%.3f"},
  /* Implict precision of '1' since there is one explicit zero in the mantissa
   * right of the decimal place after it is adjusted for the exponent.  */
  {__LINE__, 1.000E+2DL, "100.000000", "%DDf", "%f"},
  /* Implict precision of '1' since there is one explicit zero in the mantissa
   * right of the decimal place after it is adjusted for the exponent.  */
  {__LINE__, 100.0E+0DL, "100.000000", "%DDf", "%f"},
  /* Implict precision of '1' since there is one explicit zero in the mantissa
   * right of the decimal place after it is adjusted for the exponent.  */
  {__LINE__, 0.0001000E+6DL, "100.000000", "%DDf", "%f"},
  /* Implict precision of '4' since there is one explicit zero in the mantissa
   * right of the decimal place after it is adjusted for the exponent.  */
  {__LINE__, 1.000000E+2DL, "100.000000", "%DDf", "%f"},
  /* Because of the 'e' output precision is '5'.  */
  {__LINE__, 123.456E+0DL, "1.234560e+02", "%DDe", "%e"},
  {__LINE__, 123.456E+0DL, "1.23456e+02", "%.5DDe", "%.5e"},
  {__LINE__, 123.456E+0DL, "1.234560E+02", "%DDE", "%E"},
  {__LINE__, 19000E+15DL, "19000000000000000000.000000", "%DDf", "%f"},
  {__LINE__, 190001E+45DL, "190001000000000000000000000000000000000000000000000.000000", "%DDf", "%f"},
  {__LINE__, -123.456E+0DL, "-1.234560e+02", "%DDe", "%e"},
  /* Encoded as 1234567E1, prec 5*/
  {__LINE__, 12.34567E+6DL, "12345670.000000", "%DDf", "%f"},

  {__LINE__, 123.456E-5DL, "1.234560e-03", "%DDe", "%e"},
  {__LINE__, 123.456E-5DL, "0.001235", "%DDf", "%f"},
  {__LINE__, 123.456E-5DL, "0.00123456", "%DDg", "%g"},

  {__LINE__, 123.456E-6DL, "1.234560e-04", "%DDe", "%e"},
  {__LINE__, 123.456E-6DL, "0.000123", "%DDf", "%f"},
  {__LINE__, 123.456E-6DL, "0.000123456", "%DDg", "%g"},

  {__LINE__, 123.456E-7DL, "1.234560e-05", "%DDe", "%e"},
  {__LINE__, 123.456E-7DL, "0.000012", "%DDf", "%f"},
  {__LINE__, 123.456E-7DL, "1.23456e-05", "%DDg", "%g"},
  {__LINE__, 123.456E-7DL, "1.235e-05", "%.4DDg", "%.4g"},
  {__LINE__, 123.456E-9DL, "1.234560e-07", "%DDe", "%e"},

  {__LINE__, 6.5DL, "6.5e+00", "%.1DDe", "%.1e"},
  {__LINE__, 6.5DL, "6e+00", "%.0DDe", "%.0e"},
  {__LINE__, 6.6DL, "7e+00", "%.0DDe", "%.0e"},

  /* Erroneously prints "1e+6" due to the '.' with no explicit precision. FIX
   * ME.  */
  {__LINE__, 6.0E5DL, "6e+5", "%.DDa", "%.a"},

  {__LINE__, 123.456E-9DL, "1.23456e-7", "%DDa", "%a"},
  {__LINE__, 0.00000E-101DL, "0.000000e+00", "%DDe", "%e"},

  {__LINE__, 0.e-101DL, "0e-101", "%DDa", "%a"},

  /* This returns 0e-106 because the zeros are significant.  */
  {__LINE__, 0.00000E-101DL, "0e-106", "%DDa", "%a"},

  {__LINE__, 0.0e10DL, "0.000000e+00", "%DDe", "%e"},
  {__LINE__, 0.0e10DL, "0.000000", "%DDf", "%f"},

  {__LINE__, 0.0e10DL, "0", "%DDg", "%g"},
  /* For 'g' when '#' is present the spec says that trailing zeros are NOT
   * removed.  And YES, this SHOULD be five trailing zeros because this falls
   * into the category where 'g' is converted to 'e' and precision equals P - 1.
   * Additionally, the C-spec indicates that.  This is congruent with how this
   * works for binary float.  */
  {__LINE__, 0.0e10DL, "0.00000", "%#DDg", NULL},
  {__LINE__, 0.0e10DL, "0", "%.4DDg", "%.4g"},

  /* (1,0,-1)  */
  {__LINE__, 0.0DL, "0.0", "%DDa", "%a"},
  {__LINE__, 0e10DL, "0e+10", "%DDa", "%a"},

  {__LINE__, 0.0e10DL, "0e+9", "%.1DDa", "%.1a"},
  {__LINE__, 0.0e-09DL, "0e-10", "%.0DDa", "%.0a"},
  /* (1,0,0)  */
  {__LINE__, 0e0DL, "0", "%DDa", "%a"},

  /* (1,0,0)  */
  {__LINE__, -0e0DL, "-0", "%DDa", "%a"},
  /* (-1,0,-1)  */
  {__LINE__, -0.0DL, "-0.0", "%DDa", "%a"},

  /* ( 1, 123, 0)  */
  {__LINE__, 123.DL, "123", "%DDa", "%a"},

  /* (-1, 123, 0) -123  */
  {__LINE__, -123.DL, "-123", "%DDa", "%a"},

  /* ( 1, 123, -2) 1.23  */
  {__LINE__, 1.23DL, "1.23", "%DDa", "%a"},
  {__LINE__, 123E-2DL, "1.23", "%DDa", "%a"},
  {__LINE__, 0.123E1DL, "1.23", "%DDa", "%a"},

  /* ( 1, 123, 1) 1.23e+3  */
  {__LINE__, 123e1DL, "1.23e+3", "%DDa", "%a"},

  /* ( 1, 1230, 0) 1230  */
  {__LINE__, 123.0e1DL, "1230", "%DDa", "%a"},

  /* (-1, 123, 1) -1.23e+3  */
  {__LINE__, -123e1DL, "-1.23e+3", "%DDa", "%a"},

  /* ( 1, 123, -8) 0.00000123  */
  {__LINE__, 123e-8DL, "0.00000123", "%DDa", "%a"},
  {__LINE__, 123.e-8DL, "0.00000123", "%DDa", "%a"},

  /* ( 1, 123, -9) 1.23e-7  */
  {__LINE__, 123e-9DL, "1.23e-7", "%DDa", "%a"},
  {__LINE__, 123.e-9DL, "1.23e-7", "%DDa", "%a"},

  /* Can represent the following in _Decimal128  */
  /* ( 1, 1234567890123456, 0) 1234567890123456  */
  {__LINE__, 1234567890123456e0DL, "1234567890123456", "%DDa", "%a"},

  /* ( 1, 1234567890123456, 1) 1.234567890123456e+16  */
  {__LINE__, 1234567890123456e1DL, "1.234567890123456e+16", "%DDa", "%a"},

  /* ( 1, 1234567890123456, -1) 123456789012345.6  */
  {__LINE__, 1234567890123456e-1DL, "123456789012345.6", "%DDa", "%a"},

  /* ( 1, 1234567890123456, -21)    0.000001234567890123456  */
  {__LINE__, 1234567890123456e-21DL, "0.000001234567890123456", "%DDa", "%a"},

  /* ( 1, 1234567890123456, -22)    1.234567890123456e-7  */
  {__LINE__, 1234567890123456e-22DL, "1.234567890123456e-7", "%DDa", "%a"},

  /* ( 1, 0, 0) 0  */
  {__LINE__, 0e0DL, "0", "%DDa", "%a"},
  {__LINE__, 0.DL, "0", "%DDa", "%a"},
  {__LINE__, 0.e0DL, "0", "%DDa", "%a"},

  /* (-1, 0, 0) -0  */
  {__LINE__, -0e0DL, "-0", "%DDa", "%a"},
  {__LINE__, -0.DL, "-0", "%DDa", "%a"},
  {__LINE__, -0.e0DL, "-0", "%DDa", "%a"},

  /* ( 1, 0, -6) 0.000000  */
  {__LINE__, 0e-6DL, "0.000000", "%DDa", "%a"},

  /* ( 1, 0, -7) 0e-7  */
  {__LINE__, 0e-7DL, "0e-7", "%DDa", "%a"},
  {__LINE__, 0.e-10DL, "0e-10", "%.1DDa", "%.1a"},
  {__LINE__, 0.e-10DL, "0e-10", "%.3DDa", "%.3a"},
  {__LINE__, 0.e-10DL, "0e-10", "%.4DDa", "%.4a"},


  /* ( 1, 5, -6) 0.000005  */
  {__LINE__, 5e-6DL, "0.000005", "%DDa", "%a"},

  /* ( 1, 50, -7) 0.0000050  */
  {__LINE__, 50e-7DL, "0.0000050", "%DDa", "%a"},

  /* ( 1, 5, -7) 5e-7  */
  {__LINE__, 5e-7DL, "5e-7", "%DDa", "%a"},

  /* ( 1, 0, 2) 0e+2 encoded as 00000000e2.  */
  {__LINE__, 0e2DL, "0e+2", "%DDa", "%a"},

  /* Yes, goofy, but it is encoded 00000000e1.  */
  {__LINE__, 0.0e2DL, "0e+1", "%DDa", "%a"},

  {__LINE__, 0.0e10DL, "0e+9", "%DDa", "%a"},

  /* Yes, goofy, but it is encoded 00000000e0.  */
  {__LINE__, 0.00e2DL, "0", "%DDa", "%a"},

  /* Yes, goofy, but it is encoded 00000000e0.  */
  {__LINE__, 0.00e2DL, "0", "%.2DDa", "%.2a"},

  /* Notice the '#'.  The DLP spec doesn't speak about this and doesn't indicate
   * that this is supposed to work like it does for 'g' so we still expect to
   * see trailing zeros removed and no exponent.  */
  {__LINE__, 0.00e2DL, "0.", "%#DDa", NULL},

  /* ( 1, 5, -7)  */
  {__LINE__, 5e-7DL, "5.e-7", "%#DDa", NULL},

  {__LINE__, 6543.E-11DL, "6.543e-8", "%DDa", "%a"},

  /* Continued DPF spec examples.  */
  {__LINE__, 6543.00DL, "6543.00", "%DDa", "%a"},
  {__LINE__, 6543.00DL, "6543.00", "%.6DDa", "%.6a"},
  {__LINE__, 6543.00DL, "6543.0", "%.5DDa", "%.5a"},
  {__LINE__, 6543.00DL, "6543", "%.4DDa", "%.4a"},
  {__LINE__, 6543.00DL, "6.54e+3", "%.3DDa", "%.3a"},
  {__LINE__, 6543.00DL, "6.5e+3", "%.2DDa", "%.2a"},
  {__LINE__, 6543.00DL, "7e+3", "%.1DDa", "%.1a"},
  {__LINE__, 6543.00DL, "7E+3", "%.1DDA", "%.1A"},

  {__LINE__, 6543.00DL, "6543.00", "%.8DDa", "%.8a"},

  /* Fixed by Andreas Schwab on 2010-20-04 */
  {__LINE__, 0.9999999999DL, "1.000000", "%DDf", "%f"},

  /* Caused a segv due to a the negative exponent generating a negative 'index'.
   * It was fixed by Andreas Krebbel on 2011-01-18 */
  {__LINE__, (1.0DL / 1.000000e-123DL), "1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000.000000", "%DDf", "%f"},

  {__LINE__, -1234.56789123456789123455678DL, "-1234.56789123", "%.8DDf", "%.8f"},
  /* This prints like f/F but g/G computes precision based on all digits in
   * the mantissa, not just those to the right of the decimal place like
   * regular f/F.  */
  {__LINE__, -1234.56789123456789123455678DL, "-1234.5679", "%.8DDg", "%.8g"},
  {__LINE__, -1234.56789123456789123455678DL, "-1234.56789123", "%.12DDg", "%.12g"},
  /* This isn't properly rounding after the truncated.  FIX ME. */
  {__LINE__, -1234.56789123456789123455678DL, "-1234.57", "%DDg", "%g"},
  {__LINE__, -1234.56789123456789123455678DL, "-1234.5679", "%.8DDa", "%.8a"},
  {__LINE__, -1234.56789123456789123455678DL, "-1.234568e+03", "%.6DDe", "%.6e"},
  {__LINE__, -1234.56789123456789123455678DL, "-1.23456789e+03", "%.8DDe", "%.8e"},
  {0,0,0,0,0 }
};

typedef struct{
  int line;
  float f;
  _Decimal32 d;
  const char *expect;
  const char *format;
} m32_type;

m32_type printf_m32s[] =
{
  {__LINE__, 1.0, 123.456E-6DL, "1.000000 0.000123", "%f %Hf"},

  {0,0,0,0,0 }
};

int main (void)
{
  d32_type *d32ptr;
  d64_type *d64ptr;
  d128_type *d128ptr;
  m32_type *m32ptr;

  for (d32ptr = printf_d32s; d32ptr->line; d32ptr++)
    {
      _PC_P(__FILE__, d32ptr->line, d32ptr->expect, d32ptr->format, d32ptr->d);
      if (d32ptr->sf_format)
        _SFC_P(__FILE__, d32ptr->line, d32ptr->expect, d32ptr->sf_format, strfromd32, d32ptr->d);
    }

  for (d64ptr = printf_d64s; d64ptr->line; d64ptr++)
    {
      _PC_P(__FILE__,d64ptr->line, d64ptr->expect,d64ptr->format,d64ptr->d);
      if (d64ptr->sf_format)
        _SFC_P(__FILE__, d64ptr->line, d64ptr->expect, d64ptr->sf_format, strfromd64, d64ptr->d);
    }

  for (d128ptr = printf_d128s; d128ptr->line; d128ptr++)
    {
      _PC_P(__FILE__,d128ptr->line, d128ptr->expect,d128ptr->format,d128ptr->d);
      if (d128ptr->sf_format)
        _SFC_P(__FILE__, d128ptr->line, d128ptr->expect, d128ptr->sf_format, strfromd128, d128ptr->d);
    }

  for (m32ptr = printf_m32s; m32ptr->line; m32ptr++)
    {
      _PC_P(__FILE__, m32ptr->line, m32ptr->expect, m32ptr->format, \
	    m32ptr->f, m32ptr->d);
    }

  _REPORT();

  /* fail comes from scaffold.c  */
  return fail;
}
