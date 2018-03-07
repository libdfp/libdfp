/* Test Inexact Exception.

   Copyright (C) 2018 Free Software Foundation, Inc.

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

#define UNUSED(expr) do { (void)(expr); } while (0)

typedef struct
{
  int line;
  _Decimal128 set;
  int expect;
} d_type;

/* Decimal 128 to Binary Double 64.  */
static const d_type d128to64[] = {
 {__LINE__, 0.1DL, FE_INEXACT},
 {__LINE__, 0.5DL, FE_INEXACT}
};

/* Decimal 128 to Binary Float 32.  */
static const d_type d128to32[] = {
 {__LINE__, 0.1DL, FE_INEXACT},
 {__LINE__, 0.5DL, FE_INEXACT}
};

int
/* Disable optmizations or else GCC will optimize cast to a const and will
   lost the raised inexact flag.   */
__attribute__((optimize("O0")))
main(void)
{
  double retd;
  float retf;
  UNUSED(retd); /* Avoid unused warning.  */
  UNUSED(retf);
  int ex;

  /* 128-to-64  */
  feclearexcept(FE_INEXACT);

  retd = (double) d128to64[0].set;
  ex = fetestexcept (FE_INEXACT);
   _VC_P (__FILE__, d128to64[0].line, d128to64[0].expect, ex, "%d");

  feclearexcept(FE_INEXACT);

  feraiseexcept (FE_INEXACT);
  retd = (double) d128to64[1].set;
  ex = fetestexcept (FE_INEXACT);
  _VC_P (__FILE__, d128to64[1].line, d128to64[1].expect, ex, "%d");

  /* 128-to-32  */

  feclearexcept(FE_INEXACT);

  retf = (float) d128to32[0].set;
  ex = fetestexcept (FE_INEXACT);
   _VC_P (__FILE__, d128to32[0].line, d128to32[0].expect, ex, "%d");

  feclearexcept(FE_INEXACT);

  feraiseexcept (FE_INEXACT);
  retf = (float) d128to32[1].set;
  ex = fetestexcept (FE_INEXACT);
  _VC_P (__FILE__, d128to32[1].line, d128to32[1].expect, ex, "%d");

  _REPORT();

  /* fail comes from scaffold.c  */
  return fail;
}
