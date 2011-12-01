/* Test expd[32|64|128].

   Copyright (C) 2011 Free Software Foundation, Inc.

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

#include <math.h>
#include <stdio.h>

#define _WANT_AVC 1    /* We want the _AVC_P(x,y,lim,fmt,lfmt) macro.  */
#include "scaffold.c" /* Pick up the _AVC_P(x,y,lim,fmt,lfmt) macro.  */

typedef struct{
  int line;
  _Decimal128 x;  /* Value to test  */
  _Decimal128 e;  /* Result should be this.  */
  _Decimal128 lim; /* Result can deviate +/- this much.  */
  const char *format; /* e.g., printf %DDgDL */
  const char *lformat; /* e.g., printf %DDfDL */
} d128_type;

d128_type printf_d128s[] =
{
  {__LINE__, -1.0DL, 0.367879DL, 0.00001DL ,"%.34DDgDL","%.34DDfDL"},
  {__LINE__, -1.0DL, 0.367879441171442321595523770161461DL, 0.000000000000000000000000000000001DL ,"%.34DDgDL","%.34DDfDL"},
  {__LINE__, 1.0DL,  2.718281828459045235360287471352663DL, 0.000000000000000000000000000000001DL ,"%.34DDgDL","%.34DDfDL"},
  {0,0,0,0,0,0 }
};

typedef struct{
  int line;
  _Decimal64 x;  /* Value to test  */
  _Decimal64 e;  /* Result should be this.  */
  _Decimal64 lim; /* Result can deviate +/- this much.  */
  const char *format; /* printf %DgDD */
  const char *lformat; /* printf %DfDD */
} d64_type;

d64_type printf_d64s[] =
{
  {__LINE__, -1.0DD, 0.367879DD, 0.00001DD ,"%0.16DgDD","%0.16DfDD"},
  {__LINE__, 1.0DD, 2.718281828459045DD, 0.0000000000001DD ,"%0.16DgDD","%0.16DfDD"},
  {0,0,0,0,0,0 }
};

int main( void )
{

  d128_type *d128ptr;
  d64_type *d64ptr;

  for (d128ptr = printf_d128s; d128ptr->line; d128ptr++)
    {
      _Decimal128 retval = expd128(d128ptr->x);
      fprintf(stdout,"%.34DDfDL = expd128(%.34DDfDL) in: %s:%d\n", retval, d128ptr->x,__FILE__,__LINE__-1);
      _AVC_P(__FILE__,d128ptr->line, d128ptr->e,retval,d128ptr->lim,d128ptr->format,d128ptr->lformat);
    }

  for (d64ptr = printf_d64s; d64ptr->line; d64ptr++)
    {
      _Decimal64 retval = expd64(d64ptr->x);
      fprintf(stdout,"%.34DfDD = expd64(%.34DfDD) in: %s:%d\n", retval, d64ptr->x,__FILE__,__LINE__-1);
      _AVC_P(__FILE__,d64ptr->line, d64ptr->e,retval,d64ptr->lim,d64ptr->format,d64ptr->lformat);
    }

  _REPORT();

  /* fail comes from scaffold.c  */
  return fail;
}
