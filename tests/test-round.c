/* Test fe_dec_getround() and fe_dec_setround()

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
#include <fenv.h>

#define _WANT_VC 1 /* Pick up the _VC_P(x,y,fmt) macro.  */
#include "scaffold.c" /* Pick up the _VC_P(x,y,fmt) macro.  */

/* Inspired by GLIBC stdio-common/tfformat.c  */
typedef struct{
  int line;
  int set;     /* Set it to this.  */
  int expect;  /* Result should be this.  */
  const char *format; /* printf %d */
} d_type;

d_type printf_rms[] =
{
  {__LINE__, FE_DEC_TONEAREST, FE_DEC_TONEAREST,  "%d"},
  {__LINE__, FE_DEC_TOWARDZERO, FE_DEC_TOWARDZERO,  "%d"},
  {__LINE__, FE_DEC_UPWARD, FE_DEC_UPWARD,  "%d"},
  {__LINE__, FE_DEC_DOWNWARD, FE_DEC_DOWNWARD,  "%d"},
  {__LINE__, FE_DEC_TONEARESTFROMZERO, FE_DEC_TONEARESTFROMZERO,  "%d"},
  {__LINE__, 5, 5, "%d"}, /* Non-spec hardware rounding mode.  */
  {__LINE__, 6, 6, "%d"}, /* Non-spec hardware rounding mode.  */
  {__LINE__, 7, 7, "%d"}, /* Non-spec hardware rounding mode.  */
  {0,0,0,0 }
};

int main (void)
{
  d_type *dptr;

  for (dptr = printf_rms; dptr->line; dptr++)
    {
      int retval;
      fe_dec_setround(dptr->set);
      retval = fe_dec_getround();
      _VC_P(__FILE__,dptr->line, dptr->expect,retval,dptr->format);
    }

  _REPORT();

  /* fail comes from scaffold.c  */
  return fail;
}


