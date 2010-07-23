/* Test fpclassifyd[32|64|128] and polymorphic macro fpclassify() in math.h.

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

#define _WANT_VC 1 /* Pick up the _VC_P(x,y,fmt) macro.  */
#include "scaffold.c" /* Pick up the _VC_P(x,y,fmt) macro.  */

/* Inspired by GLIBC stdio-common/tfformat.c  */
typedef struct{
  int line;
  _Decimal128 x;  /* Value to test  */
  int e;  /* Result should be this.  */
  const char *format; /* printf %d */
} d128_type;

d128_type printf_d128s[] =
{
  {__LINE__, DEC_NAN, FP_NAN,  "%d"},
  {__LINE__, DEC_INFINITY, FP_INFINITE,  "%d"},
  {__LINE__, HUGE_VAL_D128, FP_INFINITE, "%d"},
  {__LINE__, 0.0DL, FP_ZERO, "%d"},
  {__LINE__, 2.3DL, FP_NORMAL, "%d"},
  {__LINE__, -2.3DL, FP_NORMAL, "%d"},
  {__LINE__, -__DEC128_SUBNORMAL_MIN__, FP_SUBNORMAL, "%d"}, /* Subnormal  */
  {__LINE__, __DEC128_SUBNORMAL_MIN__, FP_SUBNORMAL, "%d"}, /* Subnormal  */
  {__LINE__, 0.000000000000000000000000000000001E-6143DL, FP_SUBNORMAL, "%d"},
  {__LINE__, -0.000000000000000000000000000000001E-6143DL, FP_SUBNORMAL, "%d"},
  {__LINE__, __DEC128_MIN__, FP_NORMAL, "%d"},
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
  {__LINE__, DEC_NAN, FP_NAN,  "%d"},
  {__LINE__, DEC_INFINITY, FP_INFINITE,  "%d"},
  {__LINE__, HUGE_VAL_D64, FP_INFINITE, "%d"},
  {__LINE__, 0.0DD, FP_ZERO, "%d"},
  {__LINE__, 2.3DD, FP_NORMAL, "%d"},
  {__LINE__, -2.3DD, FP_NORMAL, "%d"},
  {__LINE__, __DEC64_SUBNORMAL_MIN__, FP_SUBNORMAL, "%d"}, /* Subnormal  */
  {__LINE__, -__DEC64_SUBNORMAL_MIN__, FP_SUBNORMAL, "%d"}, /* Subnormal  */
  {__LINE__, 0.000000000000001E-383DD, FP_SUBNORMAL, "%d"},
  {__LINE__, -0.000000000000001E-383DD, FP_SUBNORMAL, "%d"},
  {__LINE__, __DEC64_MIN__, FP_NORMAL, "%d"},
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
  {__LINE__, DEC_NAN, FP_NAN,  "%d"},
  {__LINE__, DEC_INFINITY, FP_INFINITE,  "%d"},
  {__LINE__, HUGE_VAL_D32, FP_INFINITE, "%d"},
  {__LINE__, 0.0DF, FP_ZERO, "%d"},
  {__LINE__, 2.3DF, FP_NORMAL, "%d"},
  {__LINE__, -2.3DF, FP_NORMAL, "%d"},
  {__LINE__, __DEC32_SUBNORMAL_MIN__, FP_SUBNORMAL, "%d"}, /* Subnormal  */
  {__LINE__, -__DEC32_SUBNORMAL_MIN__, FP_SUBNORMAL, "%d"}, /* Subnormal  */
  {__LINE__, 0.000001E-95DF, FP_SUBNORMAL, "%d"},
  {__LINE__, -0.000001E-95DF, FP_SUBNORMAL, "%d"},
  {__LINE__, __DEC32_MIN__, FP_NORMAL, "%d"},
  {0,0,0,0 }
};

static void fpc(int i)
{
  switch (i) {
    case FP_NAN:
      fprintf(stdout,"FP_NAN");
      break;
    case FP_INFINITE:
      fprintf(stdout,"FP_INFINITE");
      break;
    case FP_ZERO:
      fprintf(stdout,"FP_ZERO");
      break;
    case FP_SUBNORMAL:
      fprintf(stdout,"FP_SUBNORMAL");
      break;
    case FP_NORMAL:
      fprintf(stdout,"FP_NORMAL");
      break;
  default:
      fprintf(stdout,"unknown");
  }
}


int main (void)
{
  d128_type *d128ptr;
  d64_type *d64ptr;
  d32_type *d32ptr;

  for (d128ptr = printf_d128s; d128ptr->line; d128ptr++)
    {
      int retval = fpclassify(d128ptr->x);
      fprintf(stdout,"%d (",retval);
      fpc(retval);
      fprintf(stdout,") = fpclassify(%DDfDL) in: %s:%d\n", d128ptr->x,__FILE__,__LINE__-1);
      _VC_P(__FILE__,d128ptr->line, d128ptr->e,retval,d128ptr->format);
    }

  for (d64ptr = printf_d64s; d64ptr->line; d64ptr++)
    {
      int retval = fpclassify(d64ptr->x);
      fprintf(stdout,"%d (",retval);
      fpc(retval);
      fprintf(stdout,") = fpclassify(%DfDD) in: %s:%d\n", d64ptr->x,__FILE__,__LINE__-1);
      _VC_P(__FILE__,d64ptr->line, d64ptr->e,retval,d64ptr->format);
    }


  for (d32ptr = printf_d32s; d32ptr->line; d32ptr++)
    {
      int retval = fpclassify(d32ptr->x);
      fprintf(stdout,"%d (",retval);
      fpc(retval);
      fprintf(stdout,") = fpclassify(%HfDF) in: %s:%d\n", d32ptr->x,__FILE__,__LINE__-1);
      _VC_P(__FILE__,d32ptr->line, d32ptr->e,retval,d32ptr->format);
    }

  _REPORT();

  /* fail comes from scaffold.c  */
  return fail;
}


