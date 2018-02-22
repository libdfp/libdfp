/* Test trucate cast Decimal -> Binary overflow.

   Copyright (C) 2017 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author: Rogerio Alves <rogealve@br.ibm.com>

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
#include <stdio.h>
#include <fenv.h>
#include <math.h>

#define _WANT_VC 1
#include "scaffold.c"

typedef struct
{
  int line;
  int round_mode;
  _Decimal128 set;
  double expect;
} d_type;

/* Decimal 128 to Binary Double 64.  */
static const d_type d128to64[] = {
 {__LINE__, FE_TONEAREST, 1e-1000DL, 0.0},
 {__LINE__, FE_TONEAREST, -1e-1000DL, -0.0},
 {__LINE__, FE_TONEAREST, 2.4703282292062327e-325DL, 0.0},
 {__LINE__, FE_TONEAREST, -2.4703282292062327e-325DL, -0.0},
 {__LINE__, FE_TONEAREST, 2.4703282292062326e-325DL, __DBL_DENORM_MIN__},
 {__LINE__, FE_TONEAREST, -2.4703282292062326e-325DL, -__DBL_DENORM_MIN__},
 {__LINE__, FE_TOWARDZERO, 1e-1000DL, 0.0},
 {__LINE__, FE_TOWARDZERO, -1e-1000DL, -0.0},
 {__LINE__, FE_DOWNWARD, 1e-1000DL, 0.0},
 {__LINE__, FE_DOWNWARD, -1e-1000DL, -__DBL_DENORM_MIN__},
 {__LINE__, FE_UPWARD, 1e-1000DL, __DBL_DENORM_MIN__},
 {__LINE__, FE_UPWARD, -1e-1000DL, -0.0}
};

/* Decimal 128 to Binary Float 32.  */
static const d_type d128to32[] = {
 {__LINE__, FE_TONEAREST, 1e-1000DL, 0.0},
 {__LINE__, FE_TONEAREST, -1e-1000DL, -0.0},
 {__LINE__, FE_TONEAREST, 7.0064923e-46DL, 0.0},
 {__LINE__, FE_TONEAREST, -7.0064923e-46DL, -0.0},
 {__LINE__, FE_TONEAREST, 7.0064922e-46DL, __FLT_DENORM_MIN__},
 {__LINE__, FE_TONEAREST, -7.0064922e-46DL, -__FLT_DENORM_MIN__},
 {__LINE__, FE_TOWARDZERO, 1e-1000DL, 0.0},
 {__LINE__, FE_TOWARDZERO, -1e-1000DL, -0.0},
 {__LINE__, FE_DOWNWARD, 1e-1000DL, 0.0},
 {__LINE__, FE_DOWNWARD, -1e-1000DL, -__FLT_DENORM_MIN__},
 {__LINE__, FE_UPWARD, 1e-1000DL, __FLT_DENORM_MIN__},
 {__LINE__, FE_UPWARD, -1e-1000DL, -0.0}
};

static const int d128to64_size = sizeof (d128to64) / sizeof (d128to64[0]);

static const int d128to32_size = sizeof (d128to32) / sizeof (d128to32[0]);

int
main(void)
{
  int i;
  for (i=0; i<d128to64_size; ++i)
    {
      double ret;
      fesetround (d128to64[i].round_mode);
      ret = (double) d128to64[i].set;
      _VC_P (__FILE__, d128to64[i].line, d128to64[i].expect, ret, "%a");
    }

  for (i=0; i<d128to32_size; ++i)
    {
      float ret;
      fesetround (d128to32[i].round_mode);
      ret = (float) d128to32[i].set;
      _VC_P (__FILE__, d128to32[i].line, d128to32[i].expect, ret, "%a");
    }

  _REPORT();

  /* fail comes from scaffold.c  */
  return fail;
}
