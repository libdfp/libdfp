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
  long double expect;
} d128_type;

typedef struct
{
  int line;
  int round_mode;
  _Decimal64 set;
  double expect;
} d64_type;

typedef struct
{
  int line;
  int round_mode;
  _Decimal32 set;
  float expect;
} d32_type;

/* Decimal 128 to Binary Double 128.  */
static const d128_type d128to128[] = {
 {__LINE__, FE_TONEAREST, 1e6000DL, INFINITY},
 {__LINE__, FE_TONEAREST, -1e6000DL, -INFINITY},
 {__LINE__, FE_TONEAREST, 1e4933DL, INFINITY},
 {__LINE__, FE_TONEAREST, -1e4933DL, -INFINITY},
#if __LDBL_MANT_DIG__ == 106
 {__LINE__, FE_TONEAREST, 1.797693134862316e308DL, INFINITY},
 {__LINE__, FE_TONEAREST, -1.797693134862316e308DL, -INFINITY},
#else
 {__LINE__, FE_TONEAREST, 1.189731495357232e4932DL, INFINITY},
 {__LINE__, FE_TONEAREST, -1.189731495357232e4932DL, -INFINITY},
#endif
 {__LINE__, FE_TOWARDZERO, 1e6000DL, LDBL_MAX},
 {__LINE__, FE_TOWARDZERO, -1e6000DL, -LDBL_MAX},
 {__LINE__, FE_TOWARDZERO, 1e4933DL, LDBL_MAX},
 {__LINE__, FE_TOWARDZERO, -1e4933DL, -LDBL_MAX},
#if __LDBL_MANT_DIG__ == 106
 {__LINE__, FE_TOWARDZERO, 1.797693134862316e308DL, LDBL_MAX},
 {__LINE__, FE_TOWARDZERO, -1.797693134862316e308DL, -LDBL_MAX},
#else
 {__LINE__, FE_TOWARDZERO, 1.189731495357232e4932DL, LDBL_MAX},
 {__LINE__, FE_TOWARDZERO, -1.189731495357232e4932DL, -LDBL_MAX},
#endif
 {__LINE__, FE_DOWNWARD, 1e6000DL, LDBL_MAX},
 {__LINE__, FE_DOWNWARD, -1e6000DL, -INFINITY},
 {__LINE__, FE_DOWNWARD, 1e4933DL, LDBL_MAX},
 {__LINE__, FE_DOWNWARD, -1e4933DL, -INFINITY},
#if __LDBL_MANT_DIG__ == 106
 {__LINE__, FE_DOWNWARD, 1.797693134862316e308DL, LDBL_MAX},
 {__LINE__, FE_DOWNWARD, -1.797693134862316e308DL, -INFINITY},
#else
 {__LINE__, FE_DOWNWARD, 1.189731495357232e4932DL, LDBL_MAX},
 {__LINE__, FE_DOWNWARD, -1.189731495357232e4932DL, -INFINITY},
#endif
 {__LINE__, FE_UPWARD, 1e6000DL, INFINITY},
 {__LINE__, FE_UPWARD, -1e6000DL, -LDBL_MAX},
 {__LINE__, FE_UPWARD, 1e4933DL, INFINITY},
 {__LINE__, FE_UPWARD, -1e4933DL, -LDBL_MAX},
#if __LDBL_MANT_DIG__ == 106
 {__LINE__, FE_UPWARD, 1.797693134862316e308DL, INFINITY},
 {__LINE__, FE_UPWARD, -1.797693134862316e308DL, -LDBL_MAX}
#else
 {__LINE__, FE_UPWARD, 1.189731495357232e4932DL, INFINITY},
 {__LINE__, FE_UPWARD, -1.189731495357232e4932DL, -LDBL_MAX}
#endif
};

/* Decimal 128 to Binary Double 64.  */
static const d128_type d128to64[] = {
 {__LINE__, FE_TONEAREST, 1e1000DL, INFINITY},
 {__LINE__, FE_TONEAREST, -1e1000DL, -INFINITY},
 {__LINE__, FE_TONEAREST, 1.797693134862316e308DL, INFINITY},
 {__LINE__, FE_TONEAREST, -1.797693134862316e308DL, -INFINITY},
 {__LINE__, FE_TOWARDZERO, 1e1000DL, DBL_MAX},
 {__LINE__, FE_TOWARDZERO, -1e1000DL, -DBL_MAX},
 {__LINE__, FE_TOWARDZERO, 1.797693134862316e308DL, DBL_MAX},
 {__LINE__, FE_TOWARDZERO, -1.797693134862316e308DL, -DBL_MAX},
 {__LINE__, FE_DOWNWARD, 1e1000DL, DBL_MAX},
 {__LINE__, FE_DOWNWARD, -1e1000DL, -INFINITY},
 {__LINE__, FE_DOWNWARD, 1.797693134862316e308DL, DBL_MAX},
 {__LINE__, FE_DOWNWARD, -1.797693134862316e308DL, -INFINITY},
 {__LINE__, FE_UPWARD, 1e1000DL, INFINITY},
 {__LINE__, FE_UPWARD, -1e1000DL, -DBL_MAX},
 {__LINE__, FE_UPWARD, 1.797693134862316e308DL, INFINITY},
 {__LINE__, FE_UPWARD, -1.797693134862316e308DL, -DBL_MAX}
};

/* Decimal 128 to Binary Float 32.  */
static const d128_type d128to32[] = {
 {__LINE__, FE_TONEAREST, 1e1000DL, INFINITY},
 {__LINE__, FE_TONEAREST, -1e1000DL, -INFINITY},
 {__LINE__, FE_TONEAREST, 3.4028236e38DL, INFINITY},
 {__LINE__, FE_TONEAREST, -3.4028236e38DL, -INFINITY},
 {__LINE__, FE_TOWARDZERO, 1e1000DL, FLT_MAX},
 {__LINE__, FE_TOWARDZERO, -1e1000DL, -FLT_MAX},
 {__LINE__, FE_TOWARDZERO, 3.4028236e38DL, FLT_MAX},
 {__LINE__, FE_TOWARDZERO, -3.4028236e38DL, -FLT_MAX},
 {__LINE__, FE_DOWNWARD, 1e1000DL, FLT_MAX},
 {__LINE__, FE_DOWNWARD, -1e1000DL, -INFINITY},
 {__LINE__, FE_DOWNWARD, 3.4028236e38DL, FLT_MAX},
 {__LINE__, FE_DOWNWARD, -3.4028236e38DL, -INFINITY},
 {__LINE__, FE_UPWARD, 1e1000DL, INFINITY},
 {__LINE__, FE_UPWARD, -1e1000DL, -FLT_MAX},
 {__LINE__, FE_UPWARD, 3.4028236e38DL, INFINITY},
 {__LINE__, FE_UPWARD, -3.4028236e38DL, -FLT_MAX}
};

/* Decimal 64 to Binary Double 64.  */
static const d64_type d64to64[] = {
 {__LINE__, FE_TONEAREST, 1e369DD, INFINITY},
 {__LINE__, FE_TONEAREST, -1e369DD, -INFINITY},
 {__LINE__, FE_TONEAREST, 1.797693134862316e308DD, INFINITY},
 {__LINE__, FE_TONEAREST, -1.797693134862316e308DD, -INFINITY},
 {__LINE__, FE_TOWARDZERO, 1e369DD, DBL_MAX},
 {__LINE__, FE_TOWARDZERO, -1e369DD, -DBL_MAX},
 {__LINE__, FE_TOWARDZERO, 1.797693134862316e308DD, DBL_MAX},
 {__LINE__, FE_TOWARDZERO, -1.797693134862316e308DD, -DBL_MAX},
 {__LINE__, FE_DOWNWARD, 1e369DD, DBL_MAX},
 {__LINE__, FE_DOWNWARD, -1e369DD, -INFINITY},
 {__LINE__, FE_DOWNWARD, 1.797693134862316e308DD, DBL_MAX},
 {__LINE__, FE_DOWNWARD, -1.797693134862316e308DD, -INFINITY},
 {__LINE__, FE_UPWARD, 1e369DD, INFINITY},
 {__LINE__, FE_UPWARD, -1e369DD, -DBL_MAX},
 {__LINE__, FE_UPWARD, 1.797693134862316e308DD, INFINITY},
 {__LINE__, FE_UPWARD, -1.797693134862316e308DD, -DBL_MAX}
};

/* Decimal 64 to Binary Float 32.  */
static const d64_type d64to32[] = {
 {__LINE__, FE_TONEAREST, 1e369DD, INFINITY},
 {__LINE__, FE_TONEAREST, -1e369DD, -INFINITY},
 {__LINE__, FE_TONEAREST, 3.402823567797337e38DD, INFINITY},
 {__LINE__, FE_TONEAREST, -3.402823567797337e38DD, -INFINITY},
 {__LINE__, FE_TOWARDZERO, 1e369DD, FLT_MAX},
 {__LINE__, FE_TOWARDZERO, -1e369DD, -FLT_MAX},
 {__LINE__, FE_TOWARDZERO, 3.402823567797337e38DD, FLT_MAX},
 {__LINE__, FE_TOWARDZERO, -3.402823567797337e38DD, -FLT_MAX},
 {__LINE__, FE_DOWNWARD, 1e369DD, FLT_MAX},
 {__LINE__, FE_DOWNWARD, -1e369DD, -INFINITY},
 {__LINE__, FE_DOWNWARD, 3.402823567797337e38DD, FLT_MAX},
 {__LINE__, FE_DOWNWARD, -3.402823567797337e38DD, -INFINITY},
 {__LINE__, FE_UPWARD, 1e369DD, INFINITY},
 {__LINE__, FE_UPWARD, -1e369DD, -FLT_MAX},
 {__LINE__, FE_UPWARD, 3.402823567797337e38DD, INFINITY},
 {__LINE__, FE_UPWARD, -3.402823567797337e38DD, -FLT_MAX}
};

/* Decimal 64 to Binary Float 32.  */
static const d32_type d32to32[] = {
 {__LINE__, FE_TONEAREST, 1e90DF, INFINITY},
 {__LINE__, FE_TONEAREST, -1e90DF, -INFINITY},
 {__LINE__, FE_TONEAREST, 3.402826e38DF, INFINITY},
 {__LINE__, FE_TONEAREST, -3.402826e38DF, -INFINITY},
 {__LINE__, FE_TOWARDZERO, 1e90DF, FLT_MAX},
 {__LINE__, FE_TOWARDZERO, -1e90DF, -FLT_MAX},
 {__LINE__, FE_TOWARDZERO, 3.402826e38DF, FLT_MAX},
 {__LINE__, FE_TOWARDZERO, -3.402826e38DF, -FLT_MAX},
 {__LINE__, FE_DOWNWARD, 1e90DF, FLT_MAX},
 {__LINE__, FE_DOWNWARD, -1e90DF, -INFINITY},
 {__LINE__, FE_DOWNWARD, 3.402826e38DF, FLT_MAX},
 {__LINE__, FE_DOWNWARD, -3.402826e38DF, -INFINITY},
 {__LINE__, FE_UPWARD, 1e90DF, INFINITY},
 {__LINE__, FE_UPWARD, -1e90DF, -FLT_MAX},
 {__LINE__, FE_UPWARD, 3.402826e38DF, INFINITY},
 {__LINE__, FE_UPWARD, -3.402826e38DF, -FLT_MAX}
};

static const int d128to128_size = sizeof (d128to128) / sizeof (d128to128[0]);
static const int d128to64_size = sizeof (d128to64) / sizeof (d128to64[0]);
static const int d128to32_size = sizeof (d128to32) / sizeof (d128to32[0]);

static const int d64to64_size = sizeof (d64to64) / sizeof (d64to64[0]);
static const int d64to32_size = sizeof (d64to32) / sizeof (d64to32[0]);

static const int d32to32_size = sizeof (d32to32) / sizeof (d32to32[0]);

int
main(void)
{
  int i;
  for (i=0; i<d128to128_size; ++i)
    {
      long double ret;
      fesetround (d128to128[i].round_mode);
      ret = (long double) d128to128[i].set;
      _VC_P (__FILE__, d128to128[i].line, d128to128[i].expect, ret, "%La");
    }

  for (i=0; i<d128to64_size; ++i)
    {
      double ret;
      fesetround (d128to64[i].round_mode);
      ret = (double) d128to64[i].set;
      _VC_P (__FILE__, d128to64[i].line, d128to64[i].expect, (long double) ret,
	     "%La");
    }

  for (i=0; i<d128to32_size; ++i)
    {
      float ret;
      fesetround (d128to32[i].round_mode);
      ret = (float) d128to32[i].set;
      _VC_P (__FILE__, d128to32[i].line, d128to32[i].expect, (long double) ret,
	     "%La");
    }

  for (i=0; i<d64to64_size; ++i)
    {
      double ret;
      fesetround (d64to64[i].round_mode);
      ret = (double) d64to64[i].set;
      _VC_P (__FILE__, d64to64[i].line, d64to64[i].expect, ret, "%a");
    }

  for (i=0; i<d64to32_size; ++i)
    {
      float ret;
      fesetround (d64to32[i].round_mode);
      ret = (float) d64to32[i].set;
      _VC_P (__FILE__, d64to32[i].line, d64to32[i].expect, ret, "%a");
    }

  for (i=0; i<d32to32_size; ++i)
    {
      float ret;
      fesetround (d32to32[i].round_mode);
      ret = (float) d32to32[i].set;
      _VC_P (__FILE__, d32to32[i].line, d32to32[i].expect, ret, "%a");
    }

  _REPORT();

  /* fail comes from scaffold.c  */
  return fail;
}
