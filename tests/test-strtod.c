/* Test strtod[32|64|128] facility.

   Copyright (C) 2010-2015 Free Software Foundation, Inc.

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
# define __STDC_WANT_DEC_FP__ 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <wchar.h>

#include "decode.h"

#define _WANT_VC 1
#define _WANT_DC 1

#include "scaffold.c"

#include <errno.h>
#include <float.h>

/* Inspired by GLIBC stdio-common/tfformat.c  */
typedef struct{
  int line;
  const char *input;
  _Decimal32 d32;
  int qexp32;
  _Decimal64 d64;
  int qexp64;
  _Decimal128 d128;
  int qexp128;
  size_t rem;
} d_type;

d_type strtods[] =
{
  {__LINE__, "12.04", 12.04DF, -2, 12.04DD, -2, 12.04DL, -2, 0},
  {__LINE__, "1.0", 1.0DF, -1, 1.0DD, -1, 1.0DL, -1, 0},
  {__LINE__, "1", 1.0DF, 0, 1.0DD, 0, 1.0DL, 0, 0},
  {__LINE__, "0", 0.0DF, 0, 0.0DD, 0, 0.0DL, 0, 0},
  {__LINE__, "0.0", 0.0DF, -1, 0.0DD, -1, 0.0DL, -1, 0},
  {__LINE__, "-0.0001", -0.0001DF, -4, -0.0001DD, -4, -0.0001DL, -4, 0},
  {__LINE__, "inf", DEC_INFINITY, 0, DEC_INFINITY, 0, DEC_INFINITY, 0, 0},
  {__LINE__, "INFINITY", DEC_INFINITY, 0, DEC_INFINITY, 0, DEC_INFINITY, 0, 0},
  {__LINE__, "0.0E+100", 0.0DF, 90, 0.0DD, 99, 0.0DL, 99, 0},
  {__LINE__, "0e+97", 0.0DF, 90, 0.0DD, 97, 0.0DL, 97, 0},
  {__LINE__, "0e+395", 0.0DF, 90, 0.0DD, 369, 0.0DL, 395, 0},
  {__LINE__, "0e+6145", 0.0DF, 90, 0.0DD, 369, 0.0DL, 6111, 0},
  {__LINE__, "-0e+97", -0.0DF, 90, -0.0DD, 97, -0.0DL, 97, 0},
  {__LINE__, "-0e+92", -0.0DF, 90, -0.0DD, 92, -0.0DL, 92, 0},
  {__LINE__, "-0e+90", -0.0DF, 90, -0.0DD, 90, -0.0DL, 90, 0},
  {__LINE__, "-0e+89", -0.0DF, 89, -0.0DD, 89, -0.0DL, 89, 0},
  {__LINE__, "0e-102", 0.0DF, -101, 0.0DD, -102, 0.0DL, -102, 0},
  {__LINE__, "-0e-102", -0.0DF, -101, -0.0DD, -102, -0.0DL, -102, 0},
  {__LINE__, "0e-399", 0.0DF, -101, 0.0DD, -398, 0.0DL, -399, 0},
  {__LINE__, "0e-6177", 0.0DF, -101, 0.0DD, -398, 0.0DL, -6176, 0},
  {__LINE__, "0.01", 0.01DF, -2, 0.01DD, -2, 0.01DL, -2, 0},
  {__LINE__, "0.1", 0.1DF, -1, 0.1DD, -1, 0.1DL, -1, 0},
  {__LINE__, "0.11", 0.11DF, -2, 0.11DD, -2, 0.11DL, -2, 0},
  {__LINE__, "0.21", 0.21DF, -2, 0.21DD, -2, 0.21DL, -2, 0},
  {__LINE__, "0.999999",     0.999999DF, -6, 0.999999DD, -6, 0.999999DL, -6, 0},
  {__LINE__, "0.9999999",    0.9999999DF, -7, 0.9999999DD, -7, 0.9999999DL, -7, 0},
  {__LINE__, "0.99999999",   1.000000DF, -6, 0.99999999DD, -8, 0.99999999DL, -8, 0},
  {__LINE__, "0.999999999",  1.000000DF, -6, 0.999999999DD, -9, 0.999999999DL, -9, 0},
  {__LINE__, "19e9", 19000000000.0DF, 9, 19000000000.0DD, 9, 19000000000.0DL, 9, 0},
  {__LINE__, "3.14", 3.140000DF, -2, 3.140000DD, -2, 3.140000DL, -2, 0},
  {__LINE__, "3.14e-2", 0.031400DF, -4, 0.031400DD, -4, 0.031400DL, -4, 0},
  {__LINE__, "1234.5678910111213e-5", 0.01234568DF, -8, 0.01234567891011121DD, -17, 0.012345678910111213DL, -18, 0},
  {__LINE__, "-1234.57", -1234.57DF, -2, -1234.57DD, -2, -1234.57DL, -2, 0},
  {__LINE__, "bogus", 0.DF, 0, 0.DD, 0, 0.DL, 0, 5},

  /* TS 18661-2 strtodN accepts only base-10 strings. */
  {__LINE__, "0x123", 0.DF, 0, 0.DD, 0, 0.DL, 0, 4},
  {__LINE__, "0X123", 0.DF, 0, 0.DD, 0, 0.DL, 0, 4},
  {__LINE__, "1x123", 1.DF, 0, 1.DD, 0, 1.DL, 0, 4},
  {__LINE__, "1X123", 1.DF, 0, 1.DD, 0, 1.DL, 0, 4},
  {__LINE__, "10x123", 10.DF, 0, 10.DD, 0, 10.DL, 0, 4},
  {__LINE__, "10X123", 10.DF, 0, 10.DD, 0, 10.DL, 0, 4},

  /* Whitespace handling edge cases */
  {__LINE__, "   123.45", 123.45DF, -2, 123.45DD, -2, 123.45DL, -2, 0},
  {__LINE__, "\t\t123.45", 123.45DF, -2, 123.45DD, -2, 123.45DL, -2, 0},
  {__LINE__, "\n123.45", 123.45DF, -2, 123.45DD, -2, 123.45DL, -2, 0},
  {__LINE__, " \t\n\r123.45", 123.45DF, -2, 123.45DD, -2, 123.45DL, -2, 0},

  /* Sign edge cases */
  {__LINE__, "+123.45", 123.45DF, -2, 123.45DD, -2, 123.45DL, -2, 0},
  {__LINE__, "+0.0", 0.0DF, -1, 0.0DD, -1, 0.0DL, -1, 0},
  {__LINE__, "-0.0", -0.0DF, -1, -0.0DD, -1, -0.0DL, -1, 0},
  {__LINE__, "+inf", DEC_INFINITY, 0, DEC_INFINITY, 0, DEC_INFINITY, 0, 0},
  {__LINE__, "-inf", -DEC_INFINITY, 0, -DEC_INFINITY, 0, -DEC_INFINITY, 0, 0},

  /* Empty and invalid input */
  {__LINE__, "", 0.DF, 0, 0.DD, 0, 0.DL, 0, 0},
  {__LINE__, "   ", 0.DF, 0, 0.DD, 0, 0.DL, 0, 3},
  {__LINE__, ".", 0.DF, 0, 0.DD, 0, 0.DL, 0, 1},
  {__LINE__, "+", 0.DF, 0, 0.DD, 0, 0.DL, 0, 1},
  {__LINE__, "-", 0.DF, 0, 0.DD, 0, 0.DL, 0, 1},

  /* Special value variations */
  {__LINE__, "INF", DEC_INFINITY, 0, DEC_INFINITY, 0, DEC_INFINITY, 0, 0},
  {__LINE__, "Inf", DEC_INFINITY, 0, DEC_INFINITY, 0, DEC_INFINITY, 0, 0},
  {__LINE__, "infinity", DEC_INFINITY, 0, DEC_INFINITY, 0, DEC_INFINITY, 0, 0},
  {__LINE__, "Infinity", DEC_INFINITY, 0, DEC_INFINITY, 0, DEC_INFINITY, 0, 0},
  {__LINE__, "+INFINITY", DEC_INFINITY, 0, DEC_INFINITY, 0, DEC_INFINITY, 0, 0},
  {__LINE__, "-INFINITY", -DEC_INFINITY, 0, -DEC_INFINITY, 0, -DEC_INFINITY, 0, 0},
  {__LINE__, "nan", DEC_NAN, 0, DEC_NAN, 0, DEC_NAN, 0, 0},
  {__LINE__, "NAN", DEC_NAN, 0, DEC_NAN, 0, DEC_NAN, 0, 0},
  {__LINE__, "NaN", DEC_NAN, 0, DEC_NAN, 0, DEC_NAN, 0, 0},
  {__LINE__, "+nan", DEC_NAN, 0, DEC_NAN, 0, DEC_NAN, 0, 0},
  {__LINE__, "-nan", DEC_NAN, 0, DEC_NAN, 0, DEC_NAN, 0, 0},

  /* Precision boundary tests - _Decimal32 has 7 significant digits */
  {__LINE__, "1234567", 1234567.DF, 0, 1234567.DD, 0, 1234567.DL, 0, 0},
  {__LINE__, "12345678", 1.234568E7DF, 1, 12345678.DD, 0, 12345678.DL, 0, 0},
  {__LINE__, "1.234567", 1.234567DF, -6, 1.234567DD, -6, 1.234567DL, -6, 0},
  {__LINE__, "1.2345678", 1.234568DF, -6, 1.2345678DD, -7, 1.2345678DL, -7, 0},
  {__LINE__, "0.1234567", 0.1234567DF, -7, 0.1234567DD, -7, 0.1234567DL, -7, 0},

  /* _Decimal64 precision tests - 16 significant digits */
  {__LINE__, "1234567890123456", 1234567890123456.DF, 9, 1234567890123456.DD, 0, 1234567890123456.DL, 0, 0},
  {__LINE__, "1.234567890123456", 1.234568DF, -6, 1.234567890123456DD, -15, 1.234567890123456DL, -15, 0},

  /* _Decimal128 precision tests - 34 significant digits */
  {__LINE__, "1234567890123456789012345678901234", 1.234568E33DF, 27,
   1.234567890123457E33DD, 18, 1234567890123456789012345678901234.DL, 0, 0},

  /* Exponent variations */
  {__LINE__, "1.5e10", 1.5E10DF, 9, 1.5E10DD, 9, 1.5E10DL, 9, 0},
  {__LINE__, "1.5E10", 1.5E10DF, 9, 1.5E10DD, 9, 1.5E10DL, 9, 0},
  {__LINE__, "1.5e+10", 1.5E10DF, 9, 1.5E10DD, 9, 1.5E10DL, 9, 0},
  {__LINE__, "1.5e-10", 1.5E-10DF, -11, 1.5E-10DD, -11, 1.5E-10DL, -11, 0},
  {__LINE__, "1.5e0", 1.5DF, -1, 1.5DD, -1, 1.5DL, -1, 0},
  {__LINE__, "1.5e00", 1.5DF, -1, 1.5DD, -1, 1.5DL, -1, 0},
  {__LINE__, "1.5e+00000010", 1.5E10DF, 9, 1.5E10DD, 9, 1.5E10DL, 9, 0},

  /* Leading zeros */
  {__LINE__, "0000123", 123.DF, 0, 123.DD, 0, 123.DL, 0, 0},
  {__LINE__, "0000123.456", 123.456DF, -3, 123.456DD, -3, 123.456DL, -3, 0},
  {__LINE__, "0.000123", 0.000123DF, -6, 0.000123DD, -6, 0.000123DL, -6, 0},
  {__LINE__, "00000.000000", 0.0DF, -6, 0.0DD, -6, 0.0DL, -6, 0},
  {__LINE__, "0000000", 0.DF, 0, 0.DD, 0, 0.DL, 0, 0},

  /* Trailing zeros (important for decimal types - they preserve trailing zeros) */
  {__LINE__, "123.000", 123.000DF, -3, 123.000DD, -3, 123.000DL, -3, 0},
  {__LINE__, "123.4560000", 123.4560DF, -4, 123.4560000DD, -7, 123.4560000DL, -7, 0},
  {__LINE__, "0.10000", 0.10000DF, -5, 0.10000DD, -5, 0.10000DL, -5, 0},

  /* Minimum normal values */
  {__LINE__, "1E-95", 1E-95DF, -95, 1E-95DD, -95, 1E-95DL, -95, 0},
  {__LINE__, "1E-383", 0.DF, 0, 1E-383DD, -383, 1E-383DL, -383, 0},
  {__LINE__, "1E-6143", 0.DF, 0, 0.DD, 0, 1E-6143DL, -6143, 0},

  /* Subnormal values */
  {__LINE__, "1E-96", 1E-96DF, -96, 1E-96DD, -96, 1E-96DL, -96, 0},
  {__LINE__, "1E-101", 1E-101DF, -101, 1E-101DD, -101, 1E-101DL, -101, 0},
  {__LINE__, "1E-384", 0.DF, 0, 1E-384DD, -384, 1E-384DL, -384, 0},
  {__LINE__, "1E-398", 0.DF, 0, 1E-398DD, -398, 1E-398DL, -398, 0},
  {__LINE__, "1E-6144", 0.DF, 0, 0.DD, 0, 1E-6144DL, -6144, 0},
  {__LINE__, "1E-6176", 0.DF, 0, 0.DD, 0, 1E-6176DL, -6176, 0},

  /* Values near limits */
  {__LINE__, "9.999999E96", 9.999999E96DF, 90, 9.999999E96DD, 90, 9.999999E96DL, 90, 0},
  {__LINE__, "9.999999999999999E384", DEC_INFINITY, 0, 9.999999999999999E384DD, 369, 9.999999999999999E384DL, 369, 0},

  /* Decimal point edge cases */
  {__LINE__, "123.", 123.DF, 0, 123.DD, 0, 123.DL, 0, 0},
  {__LINE__, ".123", 0.123DF, -3, 0.123DD, -3, 0.123DL, -3, 0},
  {__LINE__, ".0", 0.0DF, -1, 0.0DD, -1, 0.0DL, -1, 0},

  /* Very long mantissa */
  {__LINE__, "1.234567890123456789012345678901234567890", 1.234568DF, -6, 1.234567890123457DD, -15,
              1.234567890123456789012345678901235DL, -33, 0},

  /* Numbers with many fractional zeros */
  {__LINE__, "0.00000000000000000000000000000001", 1E-32DF, -32, 1E-32DD, -32, 1E-32DL, -32, 0},

  /* --- Format limits and coefficient clamping ---
     When the written exponent exceeds Qmax (= Emax - (precision - 1)) but the
     value still fits, the coefficient is padded with trailing zeros and the
     exponent is clamped down to Qmax.  For _Decimal32 Qmax = 96 - 6 = 90; for
     _Decimal64 Qmax = 384 - 15 = 369; for _Decimal128 Qmax = 6144 - 33 = 6111. */
  {__LINE__, "1E96", 1E96DF, 90, 1E96DD, 96, 1E96DL, 96, 0},
  {__LINE__, "1E369", DEC_INFINITY, 0, 1E369DD, 369, 1E369DL, 369, 0},
  {__LINE__, "1E384", DEC_INFINITY, 0, 1E384DD, 369, 1E384DL, 384, 0},
  {__LINE__, "1E6111", DEC_INFINITY, 0, DEC_INFINITY, 0, 1E6111DL, 6111, 0},
  {__LINE__, "1E6144", DEC_INFINITY, 0, DEC_INFINITY, 0, 1E6144DL, 6111, 0},

  /* Largest finite magnitude for each type, written with a full-length
     coefficient.  One ULP more in the coefficient overflows to infinity. */
  {__LINE__, "9999999E90", 9.999999E96DF, 90, 9.999999E96DD, 90, 9.999999E96DL, 90, 0},

  /* Round-half-even exactly at the precision boundary: the guard digit is 5
     with no following nonzero digits, so the tie breaks toward the even digit. */
  {__LINE__, "1.2345675", 1.234568DF, -6, 1.2345675DD, -7, 1.2345675DL, -7, 0}, /* 7 is odd -> round up */
  {__LINE__, "1.2345665", 1.234566DF, -6, 1.2345665DD, -7, 1.2345665DL, -7, 0}, /* 6 is even -> stay */
  {__LINE__, "1.2345655", 1.234566DF, -6, 1.2345655DD, -7, 1.2345655DL, -7, 0}, /* 5 is odd -> round up */

  /* Rounding that carries across a power of ten.  For _Decimal32 the carry
     (9999999|5 -> 1.000000E97) pushes the value past the maximum finite
     exponent and should overflow to +infinity.
     KNOWN BUG: strtod32() returns NaN here (and sets no errno) instead of
     overflowing; the coefficient carry into an 8th digit is mishandled. */
  {__LINE__, "9.9999995E96", DEC_INFINITY, 0, 9.9999995E96DD, 89, 9.9999995E96DL, 89, 0},

  /* Guard digit 5 followed by more nonzero digits must round up (sticky bit).
     KNOWN BUG: the fractional reader keeps only one guard digit with no sticky
     bit, so _Decimal32 rounds 1.2345665|0001 to 1.234566 instead of 1.234567. */
  {__LINE__, "1.23456650001", 1.234567DF, -6, 1.23456650001DD, -11, 1.23456650001DL, -11, 0},

  {0,0,0,0,0,0,0,0,0 }
};

const char DECLET32_NAN[] = "+0,000,000E-101";
const char DECLET64_NAN[] = "+0,000,000,000,000,000E-398";
const char DECLET128_NAN[] = "+0,000,000,000,000,000,000,000,000,000,000,000E-6176";

const char DECLET_ZERO_D32[] = "+0,000,000E+0";
const char DECLET_ZERO_D64[] = "+0,000,000,000,000,000E+0";

/* WARNING: these are byte-for-byte identical to the DECLET*_NAN strings above,
   and that is not a coincidence.  decoded32/64/128() (sysdeps/bid/decode.c)
   bail out for *any* special-encoded value, leaving coeff=0 and exp=-Bias, so
   infinity and NaN both decode to "+0,...,000E-<Bias>" (only the sign differs).
   Consequently a _DC_P comparison against these constants CANNOT distinguish
   overflow-to-infinity from NaN (nor from a genuine min-exponent zero) -- an
   overflow declet test here would pass even if strtod erroneously returned NaN.
   Real "overflow returns +/-infinity" verification must use the value-compare
   (_VC_P, x==y vs DEC_INFINITY) entries in strtods[] and the errno tests; those
   are what actually catch the overflow-carry->NaN divergence.  */
const char DECLET_HUGE_VAL_D32[] = "+0,000,000E-101";
const char DECLET_HUGE_VAL_D64[] = "+0,000,000,000,000,000E-398";
const char DECLET_HUGE_VAL_D128[] = "+0,000,000,000,000,000,000,000,000,000,000,000E-6176";

/* Inspired by GLIBC stdio-common/tfformat.c  */
typedef struct{
  int line;
  const char *input;
  const char *d32;
  const char *d64;
  const char *d128;
} d_nan_type;

d_nan_type strtods_nan[] =
{
  /* Compare against the decoded declet for each representation of DEC_NAN
   * since you can't compare DEC_NAN to DEC_NAN.  */
  {__LINE__, "NaN", DECLET32_NAN, DECLET64_NAN, DECLET128_NAN},
  {__LINE__, "4E-382", "+0,000,000E+0", "+0,000,000,000,000,004E-382", "+0,000,000,000,000,000,000,000,000,000,000,004E-382"},

  {__LINE__, "0.000000000000001E-383",  DECLET_ZERO_D32, "+0,000,000,000,000,001E-398", "+0,000,000,000,000,000,000,000,000,000,000,001E-398"},
  {__LINE__, "4E-399", DECLET_ZERO_D32, DECLET_ZERO_D64, "+0,000,000,000,000,000,000,000,000,000,000,004E-399"},
  {__LINE__, "4000000000000000E-383", DECLET_ZERO_D32, "+4,000,000,000,000,000E-383", "+0,000,000,000,000,000,004,000,000,000,000,000E-383"},
  {__LINE__, "4000000000000000E-384", DECLET_ZERO_D32, "+4,000,000,000,000,000E-384", "+0,000,000,000,000,000,004,000,000,000,000,000E-384"},
  {__LINE__, "4000000000000000E-398", DECLET_ZERO_D32, "+4,000,000,000,000,000E-398", "+0,000,000,000,000,000,004,000,000,000,000,000E-398"},
  {__LINE__, "4E-398", DECLET_ZERO_D32, "+0,000,000,000,000,004E-398", "+0,000,000,000,000,000,000,000,000,000,000,004E-398"},
  {__LINE__, "4E-399", DECLET_ZERO_D32, DECLET_ZERO_D64, "+0,000,000,000,000,000,000,000,000,000,000,004E-399"},
  {__LINE__, "4E-400", DECLET_ZERO_D32, DECLET_ZERO_D64, "+0,000,000,000,000,000,000,000,000,000,000,004E-400"},

  {__LINE__, "4E369", DECLET_HUGE_VAL_D32, "+0,000,000,000,000,004E+369", "+0,000,000,000,000,000,000,000,000,000,000,004E+369"},
  {__LINE__, "4E383", DECLET_HUGE_VAL_D32, "+4,000,000,000,000,000E+368", "+0,000,000,000,000,000,000,000,000,000,000,004E+383"},
  {__LINE__, "4E384", DECLET_HUGE_VAL_D32, "+4,000,000,000,000,000E+369", "+0,000,000,000,000,000,000,000,000,000,000,004E+384"},
  {__LINE__, "4E385", DECLET_HUGE_VAL_D32, DECLET_HUGE_VAL_D64,           "+0,000,000,000,000,000,000,000,000,000,000,004E+385"},
  {__LINE__, "4E97",  DECLET_HUGE_VAL_D32, "+0,000,000,000,000,004E+97",  "+0,000,000,000,000,000,000,000,000,000,000,004E+97"},
  {__LINE__, "4E6145",DECLET_HUGE_VAL_D32, DECLET_HUGE_VAL_D64,           DECLET_HUGE_VAL_D128},

  {__LINE__, "1.23456789E-7", "+1,234,568E-13", "+0,000,000,123,456,789E-15", "+0,000,000,000,000,000,000,000,000,123,456,789E-15" },
  {__LINE__, "1234.5678910111213e-5", "+1,234,568E-8", "+1,234,567,891,011,121E-17", "+0,000,000,000,000,000,012,345,678,910,111,213E-18" },

  /* Coefficient clamping.  "1E96" needs exponent 96, but _Decimal32's largest
     quantum exponent is 90, so the coefficient is padded to 1000000E90.  For
     _Decimal64 and _Decimal128 the exponent fits and the coefficient stays 1. */
  {__LINE__, "1E96", "+1,000,000E+90", "+0,000,000,000,000,001E+96", "+0,000,000,000,000,000,000,000,000,000,000,001E+96" },

  /* Largest finite magnitude, exact coefficient/exponent for each type. */
  {__LINE__, "9999999E90", "+9,999,999E+90", "+0,000,000,009,999,999E+90", "+0,000,000,000,000,000,000,000,000,009,999,999E+90" },

  /* Exact fraction with trailing zeros preserves the written quantum. */
  {__LINE__, "1.2500", "+0,012,500E-4", "+0,000,000,000,012,500E-4", "+0,000,000,000,000,000,000,000,000,000,012,500E-4" },

  /* Subnormal rounding for _Decimal32: the smallest quantum is -101, so
     1.5E-101 cannot be represented exactly and should round half-to-even to
     2E-101.  For _Decimal64/_Decimal128 the value is normal and exact.
     KNOWN BUG: strtod32() flushes 1.5E-101 to zero because the pre-rounding
     underflow check rejects exponent -102 without considering that rounding
     brings the value into the representable subnormal range. */
  {__LINE__, "1.5E-101", "+0,000,002E-101", "+0,000,000,000,000,015E-102", "+0,000,000,000,000,000,000,000,000,000,000,015E-102" },

  /* Half-to-even underflow: 0.5E-101 is exactly half the smallest _Decimal32
     subnormal and correctly rounds to +0.  For _Decimal64/_Decimal128 it is a
     normal, exact value. */
  {__LINE__, "0.5E-101", "+0,000,000E+0", "+0,000,000,000,000,005E-102", "+0,000,000,000,000,000,000,000,000,000,000,005E-102" },

  /* More _Decimal32 subnormal roundings, probing both directions and a tie.
     All have written exponent -102 (one below Qmin) and should round to a
     nonzero subnormal; the D64/D128 values are normal and exact.
     KNOWN BUG: strtod32() flushes every one of these to zero. */
  {__LINE__, "1.4E-101", "+0,000,001E-101", "+0,000,000,000,000,014E-102", "+0,000,000,000,000,000,000,000,000,000,000,014E-102" }, /* down -> 1E-101 */
  {__LINE__, "1.6E-101", "+0,000,002E-101", "+0,000,000,000,000,016E-102", "+0,000,000,000,000,000,000,000,000,000,000,016E-102" }, /* up   -> 2E-101 */
  {__LINE__, "2.5E-101", "+0,000,002E-101", "+0,000,000,000,000,025E-102", "+0,000,000,000,000,000,000,000,000,000,000,025E-102" }, /* tie  -> 2E-101 */

  /* The subnormal-rounding flaw is not specific to _Decimal32.  6E-399 rounds
     UP into the _Decimal64 subnormal range (-> 1E-398), unlike the neighbouring
     4E-399 above which correctly rounds DOWN to +0.  The D64 field is the buggy
     one here: it is flushed to zero instead of rounding to 1E-398. */
  {__LINE__, "6E-399", DECLET_ZERO_D32, "+0,000,000,000,000,001E-398", "+0,000,000,000,000,000,000,000,000,000,000,006E-399" }, /* D64 -> 1E-398 */
  {__LINE__, "1.5E-398", DECLET_ZERO_D32, "+0,000,000,000,000,002E-398", "+0,000,000,000,000,000,000,000,000,000,000,015E-399" }, /* D64 -> 2E-398 */

  /* Same flaw at the _Decimal128 subnormal boundary.  The D128 field is buggy. */
  {__LINE__, "6E-6177", DECLET_ZERO_D32, DECLET_ZERO_D64, "+0,000,000,000,000,000,000,000,000,000,000,001E-6176" }, /* D128 -> 1E-6176 */
  {__LINE__, "1.5E-6176", DECLET_ZERO_D32, DECLET_ZERO_D64, "+0,000,000,000,000,000,000,000,000,000,000,002E-6176" }, /* D128 -> 2E-6176 */
  {0,0,0,0,0 }
};

typedef enum {
  TEST_D32 = 1 << 0,
  TEST_D64 = 1 << 1,
  TEST_D128 = 1 << 2,
#define TEST_ALL (TEST_D32 | TEST_D64 | TEST_D128)
#define _TEST_TYPE_CNT 3
} test_type_flags;

typedef struct {
  int line;
  const char *input;
  int expected_errno;
  test_type_flags types;
  const char *description;
} errno_test;

errno_test overflow_errno_tests[] = {
  /* Overflow should set errno to ERANGE */
  {__LINE__, "1E97", ERANGE, TEST_D32, "D32 overflow"},
  {__LINE__, "1E100", ERANGE, TEST_D32, "D32 large overflow"},
  {__LINE__, "9.999999E97", ERANGE, TEST_D32, "D32 overflow with full mantissa"},
  {__LINE__, "-1E97", ERANGE, TEST_D32, "D32 negative overflow"},
  {__LINE__, "1E385", ERANGE, TEST_D64, "D64 overflow"},
  {__LINE__, "1E400", ERANGE, TEST_D64, "D64 large overflow"},
  {__LINE__, "-1E385", ERANGE, TEST_D64, "D64 negative overflow"},
  {__LINE__, "1E6145", ERANGE, TEST_D128, "D128 overflow"},
  {__LINE__, "1E10000", ERANGE, TEST_D128, "D128 large overflow"},
  {__LINE__, "-1E6145", ERANGE, TEST_D128, "D128 negative overflow"},
  {__LINE__, "9.999999E99999", ERANGE, TEST_ALL, "Extreme overflow"},

  /* Boundary: the largest finite magnitude must NOT set errno. */
  {__LINE__, "9.999999E96", 0, TEST_D32, "D32 largest finite is not overflow"},
  {__LINE__, "9.999999999999999E384", 0, TEST_D64, "D64 largest finite is not overflow"},

  /* Boundary: the smallest representable overflow (one ULP into infinity). */
  {__LINE__, "1.000000E97", ERANGE, TEST_D32, "D32 smallest overflow"},
  {__LINE__, "1.000000000000000E385", ERANGE, TEST_D64, "D64 smallest overflow"},

  /* Rounding of a full-precision coefficient carries past Emax and should
     overflow.  KNOWN BUG: strtodN() returns NaN and sets no errno instead of
     overflowing to infinity when the rounding carry adds a significant digit. */
  {__LINE__, "9.9999995E96", ERANGE, TEST_D32, "D32 overflow produced by rounding carry"},
  {__LINE__, "9.9999999999999995E384", ERANGE, TEST_D64, "D64 overflow produced by rounding carry"},
  {__LINE__, "9.9999999999999999999999999999999995E6144", ERANGE, TEST_D128, "D128 overflow produced by rounding carry"},

  /* A very long exponent field must be handled without integer overflow. */
  {__LINE__, "1E999999999999999999999", ERANGE, TEST_ALL, "Exponent field far beyond int range"},
  {0, NULL, 0, 0, NULL},

  /* Underflow to zero should set errno to ERANGE */
  {__LINE__, "1E-102", ERANGE, TEST_D32, "D32 underflow"},
  {__LINE__, "1E-200", ERANGE, TEST_D32, "D32 large underflow"},
  {__LINE__, "-1E-102", ERANGE, TEST_D32, "D32 negative underflow"},
  {__LINE__, "1E-399", ERANGE, TEST_D64, "D64 underflow"},
  {__LINE__, "1E-500", ERANGE, TEST_D64, "D64 large underflow"},
  {__LINE__, "-1E-399", ERANGE, TEST_D64, "D64 negative underflow"},
  {__LINE__, "1E-6177", ERANGE, TEST_D128, "D128 underflow"},
  {__LINE__, "1E-10000", ERANGE, TEST_D128, "D128 large underflow"},
  {__LINE__, "-1E-6177", ERANGE, TEST_D128, "D128 negative underflow"},
  {__LINE__, "0.0e100000", 0, TEST_ALL, "Zero should not set errno"},
  {__LINE__, "0.0e-100000", 0, TEST_ALL, "Zero with large negative exponent should not set errno"},

  /* Boundary: the smallest subnormal must NOT set errno (it is representable). */
  {__LINE__, "1E-101", 0, TEST_D32, "D32 smallest subnormal is not underflow"},
  {__LINE__, "1E-398", 0, TEST_D64, "D64 smallest subnormal is not underflow"},
  {__LINE__, "1E-6176", 0, TEST_D128, "D128 smallest subnormal is not underflow"},

  /* A very long negative exponent field must underflow without integer overflow. */
  {__LINE__, "1E-999999999999999999999", ERANGE, TEST_ALL, "Exponent field far below int range"},
  {0, NULL, 0, 0, NULL}
};

typedef struct {
  int line;
  const char *input;
  size_t expected_chars_parsed;
  test_type_flags types;
  const char *description;
} endptr_test;

endptr_test endptr_tests[] = {
  {__LINE__, "123abc", 3, TEST_ALL, "Number followed by letters"},
  {__LINE__, "123.45xyz", 6, TEST_ALL, "Decimal followed by letters"},
  {__LINE__, "123e10abc", 6, TEST_ALL, "Exponent followed by letters"},
  {__LINE__, "  123  ", 5, TEST_ALL, "Whitespace on both sides"},
  {__LINE__, "inf!", 3, TEST_ALL, "Infinity followed by character"},
  {__LINE__, "INFINITY!", 8, TEST_ALL, "INFINITY followed by character"},
  {__LINE__, "nan(123)extra", 8, TEST_ALL, "NaN with payload followed by text"},
  {__LINE__, "123,456", 3, TEST_ALL, "Comma is not valid (C locale)"},
  {__LINE__, "1.23.45", 4, TEST_ALL, "Second decimal point stops parsing"},
  {__LINE__, "1e2e3", 3, TEST_ALL, "Second exponent stops parsing"},
  {__LINE__, "++123", 0, TEST_ALL, "Invalid double sign"},
  {__LINE__, "--123", 0, TEST_ALL, "Invalid double negative"},
  {__LINE__, "+-123", 0, TEST_ALL, "Invalid mixed signs"},
  {__LINE__, "123e", 3, TEST_ALL, "Incomplete exponent"},
  {__LINE__, "123e+", 3, TEST_ALL, "Incomplete exponent with sign"},
  {__LINE__, "123e-", 3, TEST_ALL, "Incomplete exponent with negative sign"},
  {__LINE__, ".e5", 0, TEST_ALL, "Decimal point with exponent"},
  {__LINE__, "e10", 0, TEST_ALL, "Just exponent"},
  {__LINE__, "", 0, TEST_ALL, "Empty string"},
  {__LINE__, "   ", 0, TEST_ALL, "Just whitespace"},
  {__LINE__, ".", 0, TEST_ALL, "Just decimal point"},
  {__LINE__, "+", 0, TEST_ALL, "Just plus"},
  {__LINE__, "-", 0, TEST_ALL, "Just minus"},
  {__LINE__, "abc", 0, TEST_ALL, "No digits"},
  {__LINE__, "0x123", 1, TEST_ALL, "Hex prefix not supported (stops at x)"},
  {__LINE__, "0X123", 1, TEST_ALL, "Hex prefix uppercase not supported"},

  /* Complete exponents are fully consumed. */
  {__LINE__, "1E+2", 4, TEST_ALL, "Signed positive exponent consumed"},
  {__LINE__, "1e2junk", 3, TEST_ALL, "Exponent value then letters"},
  {__LINE__, "1e00000000000000000010", 22, TEST_ALL, "Exponent with many leading zeros consumed"},

  /* Overflow/underflow still consume the whole numeric token. */
  {__LINE__, "1E999999999", 11, TEST_ALL, "Overflowing exponent still fully consumed"},
  {__LINE__, "0.0e-100000", 11, TEST_ALL, "Huge-exponent zero fully consumed"},

  /* Fraction and integer forms without the optional other part. */
  {__LINE__, "+.5", 3, TEST_ALL, "Signed fraction with no integer part"},
  {__LINE__, "1.", 2, TEST_ALL, "Trailing decimal point is consumed"},
  {__LINE__, "5.e3", 4, TEST_ALL, "Integer, point, then exponent"},

  /* Long fully-valid tokens are consumed in their entirety. */
  {__LINE__, "9999999999999999999999999999999999999999", 40, TEST_ALL, "Long integer fully consumed"},

  /* Leading whitespace is skipped; all five characters are consumed. */
  {__LINE__, "\f\v123", 5, TEST_ALL, "Form-feed and vertical-tab whitespace skipped"},

  /* "INF" is a valid prefix; longer partial matches of "INFINITY" only
     consume "inf" and leave the rest.  C23 accepts either INF or INFINITY. */
  {__LINE__, "infin", 3, TEST_ALL, "Partial INFINITY consumes only inf"},
  {__LINE__, "infinit", 3, TEST_ALL, "INFINITY missing final y consumes only inf"},
  {__LINE__, "INFINITYYY", 8, TEST_ALL, "Full INFINITY then extra letters"},

  /* A NaN with no closing parenthesis is not a valid n-char-sequence, so only
     the bare "nan" is consumed and "(..." is left unparsed. */
  {__LINE__, "nan(123", 3, TEST_ALL, "NaN payload without closing paren consumes only nan"},

  /* NaN payload may contain letters and underscores (C23 n-char-sequence).
     KNOWN BUG: endptr stops on the ')' rather than after it (off by one). */
  {__LINE__, "nan(a_9)", 8, TEST_ALL, "NaN payload with letters and underscore"},

  /* C23 does not permit white space between the sign and the subject. */
  {__LINE__, "+ 1", 0, TEST_ALL, "Space between plus sign and digits"},
  {__LINE__, "- inf", 0, TEST_ALL, "Space between minus sign and inf"},

  /* Decimal point with no fractional digits, followed by an empty exponent:
     the "1." is a valid number and the stray 'e' is left unparsed. */
  {__LINE__, "1.e", 2, TEST_ALL, "Trailing point then empty exponent"},
  {0, NULL, 0, 0, NULL}
};

// Validate the pointer returned in endptr is as expected.
static void check_endptr(const char *input, const char *endptr, size_t n, int line) {
    size_t l = strlen(input);
    const char *input_end = input + l;
    if (endptr < input || endptr > input_end)
      {
	fprintf (stdout, "%-3d Error: *endptr is not within input string\n", testnum);
	fprintf (stdout, "    in: %s:%d.\n\n",__FILE__,line);
	++fail;
      }
    else
      {
	size_t rem = input_end - endptr;
	if (rem != n)
	  {
	    fprintf (stdout, "%-3d Error: *endptr leaves %d characters. Expected %d.\n", testnum, (int) rem, (int) n);
	    fprintf (stdout, "in: %s:%d.\n\n",__FILE__,line);
	    ++fail;
	  }
      }
}

static void check_wendptr(const wchar_t *input, const wchar_t *endptr, size_t n, int line) {
    size_t l = wcslen(input);
    const wchar_t *input_end = input + l;
    if (endptr < input || endptr > input_end)
      {
	fprintf (stdout, "%-3d Error: *wendptr is not within input string\n", testnum);
	fprintf (stdout, "    in: %s:%d.\n\n",__FILE__,line);
	++fail;
      }
    else
      {
	size_t rem = input_end - endptr;
	if (rem != n)
	  {
	    fprintf (stdout, "%-3d Error: *wendptr leaves %d characters. Expected %d.\n", testnum, (int) rem, (int) n);
	    fprintf (stdout, "in: %s:%d.\n\n",__FILE__,line);
	    ++fail;
	  }
      }
}

static void check_qexp(int line, int expected_qexp, int qexp, int type) {
  if (qexp != expected_qexp)
    {
      fprintf (stdout, "%-3d Error: expected quantum exponent %d, got %d for _Decimal%d\n", testnum, expected_qexp, qexp, type);
      fprintf (stdout, "in: %s:%d.\n\n",__FILE__,line);
      ++fail;
    }
}

static void check_zero_sign(int line, int type, bool same_sign, bool is_zero) {
  if (is_zero && !same_sign)
    {
      fprintf (stdout, "%-3d Error: incorrect zero sign returned for _Decimal%d\n", testnum, type);
      fprintf (stdout, "in: %s:%d.\n\n",__FILE__,line);
      ++fail;
    }
}

static void copy_to_wstr(wchar_t *dest, const char *src, size_t dest_len) {
  size_t cvt_status = mbstowcs(dest, src, dest_len);
  if (((size_t)-1 == cvt_status) || (dest_len == cvt_status))
    {
      fprintf (stdout, "failed to convert %s to wchar (%zu)\n", src, cvt_status);
      exit (1);
    }
}

#define CHECK_QEXP(dptr,result,type) check_qexp(dptr->line, dptr->qexp ## type, \
					 llquantexpd ## type (result), \
					 type)
#define CHECK_ZERO_SIGN(dptr,result,type) \
	check_zero_sign(dptr->line, \
			type, \
			signbit(result) == signbit(dptr->d ## type), \
			result == 0.DL)

#define WCHAR_BUF_LEN (256)

#define RUN_ONE_TEST(pfx,wid,inptr,eptr,checker, pf_mod) \
      endptr = NULL; \
      _Decimal ## wid result ## wid ## pfx = pfx ## tod ## wid (inptr, NULL); \
      if (verbose) fprintf (stdout, #pfx "tod" #wid "(\"%s\",NULL) == " pf_mod  "\n  ", dptr->input, result ## wid ## pfx); \
      _VC_P (__FILE__, dptr->line, dptr->d ## wid, result ## wid ## pfx, pf_mod); \
      _VC_P (__FILE__, dptr->line, dptr->d ## wid, pfx ## tod ## wid (inptr, &eptr), pf_mod); \
      checker (inptr, eptr, dptr->rem, dptr->line); \
      CHECK_ZERO_SIGN (dptr, result ## wid ## pfx, wid); \
      CHECK_QEXP (dptr, result ## wid ## pfx, wid);

static void run_errno_tests(void) {
  errno_test *test;
  test_type_flags types[] = {TEST_D32, TEST_D64, TEST_D128};

  for (test = overflow_errno_tests; test->input != NULL; test++) {
    for (int t = 0; t < _TEST_TYPE_CNT; t++) {
      if (!(test->types & types[t]))
        continue;

      testnum++;
      errno = 0;
      switch (types[t]) {
        case TEST_D32: strtod32(test->input, NULL); break;
        case TEST_D64: strtod64(test->input, NULL); break;
        case TEST_D128: strtod128(test->input, NULL); break;
      }
      /* Check errno */
      if (test->expected_errno != errno) {
        fprintf(stdout, "%-3d Error: %s (D%d) - unexpected errno result\n", testnum, test->description, types[t]*32);
        fprintf(stdout, "    Input: \"%s\"\n", test->input);
        fprintf(stdout, "    Expected errno %d, got %d\n", test->expected_errno, errno);
        fprintf(stdout, "    in: %s:%d\n\n", __FILE__, test->line);
        ++fail;
      }
    }
  }
}

static void run_endptr_tests(void) {
  endptr_test *test;
  test_type_flags types[] = {TEST_D32, TEST_D64, TEST_D128};

  for (test = endptr_tests; test->input != NULL; test++) {
    for (int t = 0; t < _TEST_TYPE_CNT; t++) {
      if (!(test->types & types[t]))
        continue;

      ++testnum;
      char *endptr = NULL;
        switch (types[t]) {
          case TEST_D32: strtod32(test->input, &endptr); break;
          case TEST_D64: strtod64(test->input, &endptr); break;
          case TEST_D128: strtod128(test->input, &endptr); break;
        }

        if (endptr == NULL) {
          fprintf(stdout, "%-3d Error: %s (D%d)\n", testnum, test->description, types[t]*32);
          fprintf(stdout, "    Input: \"%s\"\n", test->input);
          fprintf(stdout, "    endptr is NULL\n");
          fprintf(stdout, "    in: %s:%d\n\n", __FILE__, test->line);
          ++fail;
          continue;
        }

        size_t chars_parsed = endptr - test->input;
        if (chars_parsed != test->expected_chars_parsed) {
          fprintf(stdout, "%-3d Error: %s (D%d)\n", testnum, test->description, types[t]*32);
          fprintf(stdout, "    Input: \"%s\"\n", test->input);
          fprintf(stdout, "    Expected %zu chars parsed, got %zu\n",
                  test->expected_chars_parsed, chars_parsed);
          fprintf(stdout, "    endptr points to: \"%s\"\n", endptr);
          fprintf(stdout, "    in: %s:%d\n\n", __FILE__, test->line);
          ++fail;
        }
    }
  }
}

int main(int argc, char *argv[]) {

  d_type *dptr;
  char *endptr = NULL;
  wchar_t *wendptr = NULL;

  wchar_t winput[WCHAR_BUF_LEN];

  verbose = 0; // Make passing results quiet by default.

  /* Parse command-line arguments */
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
      verbose = 1;
    }
  }

  for (dptr = strtods; dptr->line; dptr++)
    {
      copy_to_wstr (winput, dptr->input, WCHAR_BUF_LEN);

      RUN_ONE_TEST (str, 32, dptr->input, endptr, check_endptr, "%.6He")
      RUN_ONE_TEST (wcs, 32, winput, wendptr, check_wendptr, "%.6He")
      RUN_ONE_TEST (str, 64, dptr->input, endptr, check_endptr, "%.15De")
      RUN_ONE_TEST (wcs, 64, winput, wendptr, check_wendptr, "%.15De")
      RUN_ONE_TEST (str, 128, dptr->input, endptr, check_endptr, "%.33DDe")
      RUN_ONE_TEST (wcs, 128, winput, wendptr, check_wendptr, "%.33DDe")
    }

  d_nan_type *dnanptr;
  for (dnanptr = strtods_nan; dnanptr->line; dnanptr++)
    {
      copy_to_wstr (winput, dnanptr->input, WCHAR_BUF_LEN);

      _DC_P(__FILE__, dnanptr->line,dnanptr->d32, strtod32(dnanptr->input, NULL));
      _DC_P(__FILE__, dnanptr->line,dnanptr->d64, strtod64(dnanptr->input, NULL));
      _DC_P(__FILE__, dnanptr->line,dnanptr->d128, strtod128(dnanptr->input, NULL));
    }

  run_errno_tests();
  run_endptr_tests();

  _REPORT();

  /* fail comes from scaffold.c  */
  return fail;
}
