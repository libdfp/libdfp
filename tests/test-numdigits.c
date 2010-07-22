/* Unit test the internal numdigitsd[32|64|128]() functions.

   Copyright (C) 2010 Free Software Foundation, Inc.

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

#include <float.h> /* DEC_NAN definition.  */
#include <dfp.h>
#include <stdio.h>
#include <math.h>

#include <get_digits.h>
#define _DECIMAL_SIZE 32
#define DEC_TYPE _Decimal32
#include <numdigits.h>
#undef _DECIMAL_SIZE
#undef DEC_TYPE
#undef ADJUST
#undef Q
#undef DECIMAL_BIAS
#define _DECIMAL_SIZE 64
#define DEC_TYPE _Decimal64
#include <numdigits.h>
#undef _DECIMAL_SIZE
#undef DEC_TYPE
#undef ADJUST
#undef Q
#undef DECIMAL_BIAS
#define _DECIMAL_SIZE 128
#define DEC_TYPE _Decimal128
#include <numdigits.h>

#define _WANT_VC 1 /* Pick up the _VC_P(x,y,fmt) macro.  */
#include "scaffold.c" /* Pick up the _VC_P(x,y,fmt) macro.  */

/* We're going to be comparing fields so we need to extract the data.  This is a
 * sneaky way to get around the fact that get_digits_d* isn't exported from
 * libdfp.  */
#include "../sysdeps/dpd/dpd-private.c"

/* Inspired by GLIBC stdio-common/tfformat.c  */
typedef struct{
  int line;
  _Decimal128 x;  /* Value to test  */
  int e;  /* Result should be this.  */
  const char *format; /* printf %d */
} d128_type;

d128_type printf_d128s[] =
{
  {__LINE__, 0.02E-2DL, 1,  "%d"},
  {__LINE__, 0.0200E-2DL, 3,  "%d"},
  {__LINE__, 1.0E-2DL, 2,  "%d"},
  {__LINE__, 0.0000345E-2DL, 3,  "%d"},
  {__LINE__, 0.0000345E-10DL, 3,  "%d"},
  {__LINE__, 123456.0E-2DL, 7,  "%d"},
  {__LINE__, 123456.000E-2DL, 9,  "%d"},
  {__LINE__, 123456.000E-4DL, 9,  "%d"},
  {__LINE__, 123456.000E-6DL, 9,  "%d"},
  {__LINE__, 123456.00000000000000E-18DL, 20,  "%d"},
  {__LINE__, 123.456E-6DL, 6, "%d"},
  {0,0,0,0 }
};

typedef struct{
  int line;
  _Decimal64 x;  /* Value to test  */
  int e;  /* Result should be this.  */
  const char *format; /* printf %d */
} d64_type;

d64_type printf_d64s[] =
{
  {__LINE__, 0.02E-2DD, 1,  "%d"},
  {__LINE__, 0.0200E-2DD, 3,  "%d"},
  {__LINE__, 1.0E-2DD, 2,  "%d"},
  {__LINE__, 0.0000345E-2DD, 3,  "%d"},
  {__LINE__, 0.0000345E-10DD, 3,  "%d"},
  {__LINE__, 123456.0E-2DD, 7,  "%d"},
  {__LINE__, 123456.000E-2DD, 9,  "%d"},
  {__LINE__, 123456.000E-4DD, 9,  "%d"},
  {__LINE__, 123456.000E-6DD, 9,  "%d"},
  /* can't exceed __DEC64_MANT_DIG__  */
  {__LINE__, 123456.00000000000000E-18DD, 16,  "%d"},
  {0,0,0,0 }
};

typedef struct{
  int line;
  _Decimal32 x;  /* Value to test  */
  int e;  /* Result should be this.  */
  const char *format; /* printf %d */
} d32_type;

d32_type printf_d32s[] =
{
  {__LINE__, 0.02E-2DF, 1,  "%d"},
  {__LINE__, 0.0200E-2DF, 3,  "%d"},
  {__LINE__, 1.0E-2DF, 2,  "%d"},
  {__LINE__, 0.0000345E-2DF, 3,  "%d"},
  {__LINE__, 0.0000345E-10DF, 3,  "%d"},
  {__LINE__, 123456.0E-2DF, 7,  "%d"},
  {__LINE__, 123456.000E-2DF, 7,  "%d"}, /* can't exceed __DEC32_MANT_DIG__  */
  {__LINE__, 123456.000E-4DF, 7,  "%d"}, /* can't exceed __DEC32_MANT_DIG__  */
  {__LINE__, 123456.000E-6DF, 7,  "%d"}, /* can't exceed __DEC32_MANT_DIG__  */
  /* can't exceed __DEC32_MANT_DIG__  */
  {__LINE__, 123456.00000000000000E-18DD, 7,  "%d"},

   /* Problem numbers.  These should all return '1'.  At some point they were
    * returning '0' for soft-dfp and '1' for hard-dfp.  */
  {__LINE__, 0.00000E-101DF, 1,  "%d"},
  {__LINE__, 0.00000E-97DF, 1, "%d"},
  {__LINE__, 0.00000E-96DF, 1, "%d"},
  {__LINE__, 0.00000E-95DF, 1, "%d"},
  {__LINE__, 0e10DF, 1, "%d"},
  {__LINE__, 0.0e10DF, 1, "%d"},
  {__LINE__, 0.0e-09DF, 1, "%d"},
  {__LINE__, 0e-6DF,1, "%d"},
  {__LINE__, 0e-7DF, 1, "%d"},
  {__LINE__, 0.e-10DF, 1, "%d"},
  {__LINE__, 0.e-10DF, 1, "%d"},
  {__LINE__, 0.e-10DF, 1, "%d"},
  {__LINE__, 0e2DF, 1, "%d"},
  {__LINE__, 0.0e2DF, 1, "%d"},
  {__LINE__, 0.0e10DF, 1, "%d"},

  {0,0,0,0 }
};

int main (void)
{
  d128_type *d128ptr;
  d64_type *d64ptr;
  d32_type *d32ptr;

  for (d128ptr = printf_d128s; d128ptr->line; d128ptr++)
    {
      int retval = numdigitsd128(d128ptr->x);
      fprintf(stdout,"numdigitsd128(%DDfDL) in: %s:%d\n", d128ptr->x,__FILE__,__LINE__-1);
      _VC_P(__FILE__,d128ptr->line, d128ptr->e,retval,d128ptr->format);
    }

  for (d64ptr = printf_d64s; d64ptr->line; d64ptr++)
    {
      int retval = numdigitsd64(d64ptr->x);
      fprintf(stdout,"numdigitsd64(%DfDD) in: %s:%d\n", d64ptr->x,__FILE__,__LINE__-1);
      _VC_P(__FILE__,d64ptr->line, d64ptr->e,retval,d64ptr->format);
    }

  for (d32ptr = printf_d32s; d32ptr->line; d32ptr++)
    {
      int retval = numdigitsd32(d32ptr->x);
      fprintf(stdout,"numdigitsd32(%HfDF) in: %s:%d\n", d32ptr->x,__FILE__,__LINE__-1);
      _VC_P(__FILE__,d32ptr->line, d32ptr->e,retval,d32ptr->format);
    }

  _REPORT();

  /* fail comes from scaffold.c  */
  return fail;
}


