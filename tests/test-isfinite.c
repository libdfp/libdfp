/* Test isfinite[32|64|128] and polymorphic macro isfinite() in math.h.

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
  {__LINE__, 1.20DL, 1,  "%d"},
  {__LINE__, DEC_NAN, 0,  "%d"},
  {__LINE__, HUGE_VAL_D128, 0, "%d"},
  {__LINE__, DEC_INFINITY, 0, "%d"},
#if defined __s390__ || defined __s390x__
  {__LINE__, -1.95DL, 1, "%d"},
#else
  /* isfinite returns 'nonzero' if the argument is finite.  This could be
   * anything other than zero.  */
  {__LINE__, -1.95DL, -1, "%d"},
#endif
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
  {__LINE__, 1.20DD, 1,  "%d"},
  {__LINE__, DEC_NAN, 0,  "%d"},
  {__LINE__, HUGE_VAL_D64, 0, "%d"},
  {__LINE__, DEC_INFINITY, 0, "%d"},
#if defined __s390__ || defined __s390x__
  {__LINE__, -1.95DD, 1, "%d"},
#else
  /* isfinite returns 'nonzero' if the argument is finite.  This could be
   * anything other than zero.  */
  {__LINE__, -1.95DD, -1, "%d"},
#endif

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
  {__LINE__, 1.20DF, 1,  "%d"},
  {__LINE__, DEC_NAN, 0,  "%d"},
  {__LINE__, HUGE_VAL_D32, 0, "%d"},
  {__LINE__, DEC_INFINITY, 0, "%d"},
#if defined __s390__ || defined __s390x__
  {__LINE__, -1.95DF, 1, "%d"},
#else
  /* isfinite returns 'nonzero' if the argument is finite.  This could be
   * anything other than zero.  */
  {__LINE__, -1.95DF, -1, "%d"},
#endif
  {0,0,0,0 }
};

int main (void)
{
  d128_type *d128ptr;
  d64_type *d64ptr;
  d32_type *d32ptr;

  for (d128ptr = printf_d128s; d128ptr->line; d128ptr++)
    {
      int retval = isfinited128(d128ptr->x);
      fprintf(stdout,"%d = isfinited128(%DDfDL) in: %s:%d\n", retval, d128ptr->x,__FILE__,__LINE__-1);
      _VC_P(__FILE__,d128ptr->line, d128ptr->e,retval,d128ptr->format);

      retval = isfinite(d128ptr->x);
      fprintf(stdout,"%d = isfinite(%DDfDL) in: %s:%d\n", retval, d128ptr->x,__FILE__,__LINE__-1);
      _VC_P(__FILE__,d128ptr->line, d128ptr->e,retval,d128ptr->format);
    }

  for (d64ptr = printf_d64s; d64ptr->line; d64ptr++)
    {
      int retval = isfinited64(d64ptr->x);
      fprintf(stdout,"%d = isfinited64(%DfDD) in: %s:%d\n", retval, d64ptr->x,__FILE__,__LINE__-1);
      _VC_P(__FILE__,d64ptr->line, d64ptr->e,retval,d64ptr->format);

      retval = isfinite(d64ptr->x);
      fprintf(stdout,"%d = isfinite(%DfDD) in: %s:%d\n", retval, d64ptr->x,__FILE__,__LINE__-1);
      _VC_P(__FILE__,d64ptr->line, d64ptr->e,retval,d64ptr->format);
    }

  for (d32ptr = printf_d32s; d32ptr->line; d32ptr++)
    {
      int retval = isfinited32(d32ptr->x);
      fprintf(stdout,"%d = isfinited32(%HfDF) in: %s:%d\n", retval, d32ptr->x,__FILE__,__LINE__-1);
      _VC_P(__FILE__,d32ptr->line, d32ptr->e,retval,d32ptr->format);

      retval = isfinite(d32ptr->x);
      fprintf(stdout,"%d = isfinite(%HfDF) in: %s:%d\n", retval, d32ptr->x,__FILE__,__LINE__-1);
      _VC_P(__FILE__,d32ptr->line, d32ptr->e,retval,d32ptr->format);
    }

  _REPORT();

  /* fail comes from scaffold.c  */
  return fail;
}


