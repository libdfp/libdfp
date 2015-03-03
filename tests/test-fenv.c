/* Test fe_dec_getround() and fe_dec_setround()

   Copyright (C) 2010-2015 Free Software Foundation, Inc.

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

#include <float.h>		/* DEC_NAN definition.  */
#include <stdio.h>
#include <math.h>
#include <fenv.h>

#define _WANT_VC 1		/* Pick up the _VC_P(x,y,fmt) macro.  */
#include "scaffold.c"		/* Pick up the _VC_P(x,y,fmt) macro.  */

typedef struct
{
  int line;
  int set;			/* Set it to this.  */
  int expect;			/* Result should be this.  */
} d_type;

static const d_type round_env[] = {
  {__LINE__, FE_DEC_TONEAREST, FE_DEC_TONEAREST},
  {__LINE__, FE_DEC_TOWARDZERO, FE_DEC_TOWARDZERO},
  {__LINE__, FE_DEC_UPWARD, FE_DEC_UPWARD},
  {__LINE__, FE_DEC_DOWNWARD, FE_DEC_DOWNWARD},
  {__LINE__, FE_DEC_TONEARESTFROMZERO, FE_DEC_TONEARESTFROMZERO},
  {__LINE__, 5, 5},		/* Non-spec hardware rounding mode.  */
  {__LINE__, 6, 6},		/* Non-spec hardware rounding mode.  */
  {__LINE__, 7, 7},		/* Non-spec hardware rounding mode.  */
};

static const int round_env_size = sizeof (round_env) / sizeof (round_env[0]);

int
main (void)
{
  int i;
  for (i = 0; i < round_env_size; ++i)
    {
      int retval;
      fe_dec_setround (round_env[i].set);
      retval = fe_dec_getround ();
      _VC_P (__FILE__, round_env[i].line, round_env[i].expect, retval, "%d");
    }

  _REPORT ();

  /* fail comes from scaffold.c  */
  return fail;
}
