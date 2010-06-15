/* Test quantized[32|64|128]

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

#include <float.h>
#include <dfp.h>
#include <stdio.h>
#include <math.h>

#define _WANT_VC 1 /* Pick up the _VC_P(x,y,fmt) macro.  */
#include "scaffold.c"

/* Inspired by GLIBC stdio-common/tfformat.c  */
typedef struct{
  int line;
  _Decimal128 qx;  /* Set the exponent of this... */
  _Decimal128 qy;  /* to the exponent of this...  */
  _Decimal128 qe;  /* and the result should be this.  */
  const char *format; /* format code for displaying %H, %D, %DD  */
} d128_type;

d128_type printf_d128s[] =
{
  {__LINE__, 123.456DL, 0.12DL, 123.46DL, "%DDfDL"},
  {__LINE__, 123.456DL, 0.00DL, 123.46DL, "%DDfDL"},
  {__LINE__, -1.95DL, -190.55DL, -1.95DL, "%DDfDL"},
  {0,0,0,0,0 }
};

typedef struct{
  int line;
  _Decimal64 qx;  /* Set the exponent of this... */
  _Decimal64 qy;  /* to the exponent of this...  */
  _Decimal64 qe;  /* and the result should be this.  */
  const char *format; /* format code for displaying %H, %D, %DD  */
} d64_type;

d64_type printf_d64s[] =
{
  {__LINE__, 123.456DD, 0.12DD, 123.46DD, "%DfDD"},
  {__LINE__, 123.456DD, 0.00DD, 123.46DD, "%DfDD"},
  {__LINE__, -1.95DD, -190.55DD, -1.95DD, "%DfDD"},
  {0,0,0,0,0 }
};

typedef struct{
  int line;
  _Decimal32 qx;  /* Set the exponent of this... */
  _Decimal32 qy;  /* to the exponent of this...  */
  _Decimal32 qe;  /* and the result should be this.  */
  const char *format; /* format code for displaying %H, %D, %DD  */
} d32_type;

d32_type printf_d32s[] =
{
  {__LINE__, 123.456DF, 0.12DF, 123.46DF, "%HfDF"},
  {__LINE__, 123.456DF, 0.00DF, 123.46DF, "%HfDF"},
  {__LINE__, -1.95DF, -190.55DF, -1.95DF, "%HfDF"},
  {0,0,0,0,0 }
};

int main (void)
{
  d128_type *d128ptr;
  d64_type *d64ptr;
  d32_type *d32ptr;

  for (d128ptr = printf_d128s; d128ptr->line; d128ptr++)
    {
      _Decimal128 retval = quantized128(d128ptr->qx,d128ptr->qy);
      fprintf(stdout,"%DDfDL = quantized128(%DDfDL,%DDfDL)\n", retval, d128ptr->qx, d128ptr->qy);
      _VC_P(__FILE__,d128ptr->line, d128ptr->qe,retval,d128ptr->format);
    }

  for (d64ptr = printf_d64s; d64ptr->line; d64ptr++)
    {
      _Decimal64 retval = quantized64(d64ptr->qx,d64ptr->qy);
      fprintf(stdout,"%DfDD = quantized64(%DfDD,%DfDD)\n", retval, d64ptr->qx, d64ptr->qy);
      _VC_P(__FILE__,d64ptr->line, d64ptr->qe,retval,d64ptr->format);
    }

  for (d32ptr = printf_d32s; d32ptr->line; d32ptr++)
    {
      _Decimal32 retval = quantized32(d32ptr->qx,d32ptr->qy);
      fprintf(stdout,"%HfDF = quantized32(%HfDF,%HfDF)\n", retval, d32ptr->qx, d32ptr->qy);
      _VC_P(__FILE__,d32ptr->line, d32ptr->qe,retval,d32ptr->format);
    }

  _REPORT();

  /* fail comes from scaffold.c  */
  return fail;
}


