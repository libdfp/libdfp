/* Test issignaling[32|64|128] and polymorphic macro isinf() in math.h.

   Copyright (C) 2014 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

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
# define __STDC_WANT_DEC_FP__
#endif

#include <float.h>		/* DEC_NAN definition.  */
#include <stdio.h>
#include <math.h>

#include <dpd-private.h>	/* For ieee754r_Decimal[32|64|128].  */

#define _WANT_VC 1		/* Pick up the _VC_P(x,y,fmt) macro.  */
#include "scaffold.c"		/* Pick up the _VC_P(x,y,fmt) macro.  */

/* Inspired by GLIBC stdio-common/tfformat.c  */
typedef struct
{
  int line;
  union ieee754r_Decimal128 x;	/* Value to test  */
  int e;			/* Result should be this.  */
  const char *format;		/* printf %d */
} d128_type;

static const d128_type d128s[] = {
  {__LINE__, {.td = 1.20DL}, 0, "%d"},
  {__LINE__, {.td = -1.95DL}, 0, "%d"},
  {__LINE__, {.td = DEC_NAN}, 0, "%d"},
  /* GCC does not provide a __builtin_nans for _Decimal types.  */
  {__LINE__, {.ieee_nan = {.c = 0x1f,.signaling_nan = 1}}, 1, "%d"},
  {__LINE__, {.ieee_nan = {.negative = 1,.c = 0x1f,.signaling_nan = 1}}, 1,
   "%d"},
  {__LINE__, {.td = HUGE_VAL_D128}, 0, "%d"},
  {__LINE__, {.td = DEC_INFINITY}, 0, "%d"},
  {__LINE__, {.td = -DEC_INFINITY}, 0, "%d"},
};

static const int d128s_size = sizeof (d128s) / sizeof (d128s[0]);


typedef struct
{
  int line;
  union ieee754r_Decimal64 x;	/* Value to test  */
  int e;			/* Result should be this.  */
  const char *format;		/* printf %d */
} d64_type;

static const d64_type d64s[] = {
  {__LINE__, {.dd = 1.20DL}, 0, "%d"},
  {__LINE__, {.dd = -1.95DL}, 0, "%d"},
  {__LINE__, {.dd = DEC_NAN}, 0, "%d"},
  /* GCC does not provide a __builtin_nans for _Decimal types.  */
  {__LINE__, {.ieee_nan = {.c = 0x1f,.signaling_nan = 0x1}}, 1, "%d"},
  {__LINE__, {.ieee_nan = {.negative = 1,.c = 0x1f,.signaling_nan = 1}}, 1,
   "%d"},
  {__LINE__, {.dd = HUGE_VAL_D128}, 0, "%d"},
  {__LINE__, {.dd = DEC_INFINITY}, 0, "%d"},
  {__LINE__, {.dd = -DEC_INFINITY}, 0, "%d"},
};

static const int d64s_size = sizeof (d64s) / sizeof (d64s[0]);


typedef struct
{
  int line;
  union ieee754r_Decimal32 x;	/* Value to test  */
  int e;			/* Result should be this.  */
  const char *format;		/* printf %d */
} d32_type;

static const d32_type d32s[] = {
  {__LINE__, {.sd = 1.20DL}, 0, "%d"},
  {__LINE__, {.sd = -1.95DL}, 0, "%d"},
  {__LINE__, {.sd = DEC_NAN}, 0, "%d"},
  /* GCC does not provide a __builtin_nans for _Decimal types.  */
  {__LINE__, {.ieee_nan = {.c = 0x1f,.signaling_nan = 0x1}}, 1, "%d"},
  {__LINE__, {.ieee_nan = {.negative = 1,.c = 0x1f,.signaling_nan = 1}}, 1,
   "%d"},
  {__LINE__, {.sd = HUGE_VAL_D128}, 0, "%d"},
  {__LINE__, {.sd = DEC_INFINITY}, 0, "%d"},
  {__LINE__, {.sd = -DEC_INFINITY}, 0, "%d"},
};

static const int d32s_size = sizeof (d32s) / sizeof (d32s[0]);


int
main (void)
{
  int i;

  for (i = 0; i < d128s_size; ++i)
    {
      int retval = issignalingd128 (d128s[i].x.td);
      fprintf (stdout, "%d = issignalingd128(%DDfDL) in: %s:%d\n", retval,
	       d128s[i].x.td, __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d128s[i].line, d128s[i].e, retval, d128s[i].format);

      retval = issignaling (d128s[i].x.td);
      fprintf (stdout, "%d = isinf(%DDfDL) in: %s:%d\n", retval,
	       d128s[i].x.td, __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d128s[i].line, d128s[i].e, retval, d128s[i].format);
    }

  for (i = 0; i < d64s_size; ++i)
    {
      int retval = issignalingd64 (d64s[i].x.dd);
      fprintf (stdout, "%d = issignalingd64(%DfDD) in: %s:%d\n", retval,
	       d64s[i].x.dd, __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d64s[i].line, d64s[i].e, retval, d64s[i].format);

      retval = issignaling (d64s[i].x.dd);
      fprintf (stdout, "%d = isinf(%DfDD) in: %s:%d\n", retval, d64s[i].x.dd,
	       __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d64s[i].line, d64s[i].e, retval, d64s[i].format);
    }

  for (i = 0; i < d32s_size; ++i)
    {
      int retval = issignalingd32 (d32s[i].x.sd);
      fprintf (stdout, "%d = issignalingd32(%HfDF) in: %s:%d\n", retval,
	       d32s[i].x.sd, __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d32s[i].line, d32s[i].e, retval, d32s[i].format);

      retval = issignaling (d32s[i].x.sd);
      fprintf (stdout, "%d = isinf(%HfDF) in: %s:%d\n", retval, d32s[i].x.sd,
	       __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d32s[i].line, d32s[i].e, retval, d32s[i].format);
    }

  _REPORT ();

  /* fail comes from scaffold.c  */
  return fail;
}
