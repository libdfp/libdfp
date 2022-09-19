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

#include "decode.h"

#define _WANT_VC 1
#define _WANT_DC 1

#include "scaffold.c"

/* Inspired by GLIBC stdio-common/tfformat.c  */
typedef struct{
  int line;
  const char *input;
  _Decimal32 d32;
  _Decimal64 d64;
  _Decimal128 d128;
  size_t rem;
} d_type;

d_type strtods[] =
{
  {__LINE__, "12.04", 12.04DF,12.04DD, 12.04DL, 0},
  {__LINE__, "1.0", 1.0DF, 1.0DD, 1.0DL, 0},
  {__LINE__, "1", 1.0DF, 1.0DD, 1.0DL, 0},
  {__LINE__, "0", 0.0DF, 0.0DD, 0.0DL, 0},
  {__LINE__, "0.0", 0.0DF, 0.0DD, 0.0DL, 0},
  {__LINE__, "-0.0001", -0.0001DF, -0.0001DD, -0.0001DL, 0},
  {__LINE__, "inf", DEC_INFINITY, DEC_INFINITY, DEC_INFINITY, 0},
  {__LINE__, "INFINITY", DEC_INFINITY, DEC_INFINITY, DEC_INFINITY, 0},
  {__LINE__, "0.0E+100", 0.0DF, 0.0DD, 0.0DL, 0},
  {__LINE__, "0.01", 0.01DF, 0.01DD, 0.01DL, 0},
  {__LINE__, "0.1", 0.1DF, 0.1DD, 0.1DL, 0},
  {__LINE__, "0.11", 0.11DF, 0.11DD, 0.11DL, 0},
  {__LINE__, "0.21", 0.21DF, 0.21DD, 0.21DL, 0},
  {__LINE__, "0.999999",     0.999999DF,     0.999999DD,     0.999999DL, 0},
  {__LINE__, "0.9999999",    0.9999999DF,    0.9999999DD,    0.9999999DL, 0},
  {__LINE__, "0.99999999",   1.000000DF,     0.99999999DD,   0.99999999DL, 0},
  {__LINE__, "0.999999999",  1.000000DF,     0.999999999DD,    0.999999999DL, 0},
  {__LINE__, "19e9", 19000000000.0DF, 19000000000.0DD, 19000000000.0DL, 0},
  {__LINE__, "3.14", 3.140000DF, 3.140000DD, 3.140000DL, 0},
  {__LINE__, "3.14e-2", 0.031400DF, 0.031400DD, 0.031400DL, 0},
  {__LINE__, "1234.5678910111213e-5", 0.01234568DF,0.01234567891011121DD, 0.012345678910111213DL, 0},
  {__LINE__, "-1234.57", -1234.57DF, -1234.57DD, -1234.57DL, 0},
  {__LINE__, "bogus", 0.DF, 0.DD, 0.DL, 5},

  /* TS 18661-2 strtodN accepts only base-10 strings. */
  {__LINE__, "0x123", 0.DF, 0.DD, 0.DL, 4},
  {__LINE__, "0X123", 0.DF, 0.DD, 0.DL, 4},
  {__LINE__, "1x123", 1.DF, 1.DD, 1.DL, 4},
  {__LINE__, "1X123", 1.DF, 1.DD, 1.DL, 4},
  {__LINE__, "10x123", 10.DF, 10.DD, 10.DL, 4},
  {__LINE__, "10X123", 10.DF, 10.DD, 10.DL, 4},
  {0,0,0,0,0,0 }
};

const char DECLET32_NAN[] = "+0,000,000E-101";
const char DECLET64_NAN[] = "+0,000,000,000,000,000E-398";
const char DECLET128_NAN[] = "+0,000,000,000,000,000,000,000,000,000,000,000E-6176";

const char DECLET_ZERO_D32[] = "+0,000,000E+0";
const char DECLET_ZERO_D64[] = "+0,000,000,000,000,000E+0";

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

  /* __DEC64_SUBNORMAL_MIN__ */
  {__LINE__, "0.000000000000001E-383",  DECLET_ZERO_D32, "+0,000,000,000,000,001E-398", "+0,000,000,000,000,000,000,000,000,000,000,001E-398"},
  /* This exceeds __DEC64_MIN_EXP__ so it can't be encoded in _Decimal64 and will underflow.  */
  {__LINE__, "4E-399", DECLET_ZERO_D32, "+0,000,000,000,000,000E+0", "+0,000,000,000,000,000,000,000,000,000,000,004E-399"},
  {__LINE__, "4000000000000000E-383", DECLET_ZERO_D32, "+4,000,000,000,000,000E-383", "+0,000,000,000,000,000,004,000,000,000,000,000E-383"},
  {__LINE__, "4000000000000000E-384", DECLET_ZERO_D32, "+4,000,000,000,000,000E-384", "+0,000,000,000,000,000,004,000,000,000,000,000E-384"},
  {__LINE__, "4000000000000000E-398", DECLET_ZERO_D32, "+4,000,000,000,000,000E-398", "+0,000,000,000,000,000,004,000,000,000,000,000E-398"},
  /* This exceeds __DEC64_MIN_EXP__ so it can't be encoded in _Decimal64 and will underflow.  */
  {__LINE__, "4000000000000000E-399", DECLET_ZERO_D32, DECLET_ZERO_D64, "+0,000,000,000,000,000,004,000,000,000,000,000E-399"},
  /* This exceeds __DEC64_SUBNORMAL_MIN__ so it can't be encoded in _Decimal64 and will underflow.  */
  {__LINE__, "4E-400", DECLET_ZERO_D32, DECLET_ZERO_D64, "+0,000,000,000,000,000,000,000,000,000,000,004E-400"},

  {__LINE__, "4E369", DECLET_HUGE_VAL_D32, "+0,000,000,000,000,004E+369", "+0,000,000,000,000,000,000,000,000,000,000,004E+369"},
  {__LINE__, "4E383", DECLET_HUGE_VAL_D32, "+4,000,000,000,000,000E+368", "+0,000,000,000,000,000,000,000,000,000,000,004E+383"},
  {__LINE__, "4E384", DECLET_HUGE_VAL_D32, "+4,000,000,000,000,000E+369", "+0,000,000,000,000,000,000,000,000,000,000,004E+384"},
  {__LINE__, "4E385", DECLET_HUGE_VAL_D32, DECLET_HUGE_VAL_D64,           "+0,000,000,000,000,000,000,000,000,000,000,004E+385"},
  {__LINE__, "4E97",  DECLET_HUGE_VAL_D32, "+0,000,000,000,000,004E+97",  "+0,000,000,000,000,000,000,000,000,000,000,004E+97"},
  {__LINE__, "4E6145",DECLET_HUGE_VAL_D32, DECLET_HUGE_VAL_D64,           DECLET_HUGE_VAL_D128},

  {__LINE__, "1.23456789E-7", "+1,234,568E-13", "+0,000,000,123,456,789E-15", "+0,000,000,000,000,000,000,000,000,123,456,789E-15" },
  {__LINE__, "1234.5678910111213e-5", "+1,234,568E-8", "+1,234,567,891,011,121E-17", "+0,000,000,000,000,000,012,345,678,910,111,213E-18" },
  {0,0,0,0,0 }
};

// Validate the pointer returned in endptr is as expected.
void check_endptr(const char *input, const char *endptr, size_t n, int line);

void check_endptr(const char *input, const char *endptr, size_t n, int line) {
    size_t l = strlen(input);
    const char *input_end = input + l;
    if (endptr < input || endptr > input_end)
      {
	fprintf (stderr, "%-3d Error: *endptr is not within input string\n", testnum);
	fprintf (stderr, "    in: %s:%d.\n\n",__FILE__,line);
      }
    else
      {
	size_t rem = input_end - endptr;
	if (rem != n)
	  {
	    fprintf (stderr, "%-3d Error: *endptr leaves %d characters. Expected %d.\n", testnum, (int) rem, (int) n);
	    fprintf (stderr, "in: %s:%d.\n\n",__FILE__,line);
	  }
      }
}

int main(void) {

  d_type *dptr;
  char *endptr = NULL;

  for (dptr = strtods; dptr->line; dptr++)
    {
      endptr = NULL;
      fprintf(stdout,"strtod32(\"%s\",NULL) == %Hf\n  ",dptr->input,strtod32(dptr->input, NULL));
      _VC_P(__FILE__,dptr->line,dptr->d32,strtod32(dptr->input,NULL), "%Hf");
      _VC_P(__FILE__,dptr->line,dptr->d32,strtod32(dptr->input,&endptr), "%Hf");
      check_endptr(dptr->input, endptr, dptr->rem, dptr->line);

      endptr = NULL;
      fprintf(stdout, "strtod64(\"%s\",NULL) == %Df\n  ", dptr->input, strtod64(dptr->input, NULL));
      _VC_P(__FILE__,dptr->line,dptr->d64, strtod64(dptr->input,NULL), "%Df");
      _VC_P(__FILE__,dptr->line,dptr->d64, strtod64(dptr->input,&endptr), "%Df");
      check_endptr(dptr->input, endptr, dptr->rem, dptr->line);

      endptr = NULL;
      fprintf(stdout, "strtod128(\"%s\",NULL) == %DDf\n  ", dptr->input, strtod128(dptr->input, NULL));
      _VC_P(__FILE__,dptr->line,dptr->d128, strtod128(dptr->input,NULL), "%DDf");
      _VC_P(__FILE__,dptr->line,dptr->d128, strtod128(dptr->input,&endptr), "%DDf");
      check_endptr(dptr->input, endptr, dptr->rem, dptr->line);
    }

  d_nan_type *dnanptr;
  for (dnanptr = strtods_nan; dnanptr->line; dnanptr++)
    {
      _DC_P(__FILE__,dnanptr->line,dnanptr->d32,strtod32(dnanptr->input,NULL));
      _DC_P(__FILE__,dnanptr->line,dnanptr->d64, strtod64(dnanptr->input,NULL));
      _DC_P(__FILE__,dnanptr->line,dnanptr->d128, strtod128(dnanptr->input,NULL));
    }

  _REPORT();

  /* fail comes from scaffold.c  */
  return fail;
}
