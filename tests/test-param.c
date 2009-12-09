/* Test parameter passing compliance with the ABI.

   Copyright (C) 2009 Free Software Foundation, Inc.

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

/* This testcase is designed to test that the compiler is satisfying the
 * conditions of the ABI with regard to spilling of _Decimal* type parameters
 * to the stack.  In other words, we verify the integrity of the data after a
 * function call where parameters are spilled.  This should be tested for both
 * hard-dfp and soft-dfp.  */

#include <stdio.h>
#include <float.h>

/* Pick up the decoded* headers.  */
#include "decode.h"

#include "scaffold.c" /* Pick up the _PC(x,y,...) macro.  */

/* Inspired by GLIBC stdio-common/tfformat.c  */
typedef struct{
  int line;
  _Decimal128 d;
  const char *expect;
} d128_type;

typedef struct{
  int line;
  _Decimal64 d;
  const char *expect;
} d64_type;

typedef struct{
  int line;
  _Decimal32 d;
  const char *expect;
} d32_type;

typedef struct sparm {
  _Decimal32 df;
  _Decimal64 dd;
  _Decimal128 dl;
} sparm_t;

/* Test parameter spilling.  Use __attribute__ ((unused)) to silence compiler
 * warnings.  */
static int param_test(_Decimal128 d128 __attribute__ ((unused)),
	       _Decimal64 d64 __attribute__ ((unused)),
	       _Decimal32 d32 __attribute__ ((unused)),
	       struct sparm *s __attribute__ ((unused)),
	       struct sparm *t __attribute__ ((unused)),
	       _Decimal32 e32 __attribute__ ((unused)),
	       _Decimal64 e64 __attribute__ ((unused)),
	       _Decimal64 z64 __attribute__ ((unused)),
	       _Decimal128 e128,
	       _Decimal64 f64 __attribute__ ((unused)),
	       _Decimal128 f128)
{
  volatile _Decimal128 z;
  volatile _Decimal128 y;
  z = e128;
  y = f128;
  return 0;
}

int main(void) {
  int x;
  struct sparm s, t;

  d32_type d32types[] =
  {
#ifdef _DPD_BACKEND
    {__LINE__,4.44444DF, "+0,444,444E-5"},
    {__LINE__,1.22222DF, "+0,122,222E-5"},
#else
    {__LINE__,4.44444DF, "BID not supported."},
    {__LINE__,1.22222DF, "BID not supported."},
#endif
    {0,0,0 }
  };

  d64_type d64types[] =
  {
#ifdef _DPD_BACKEND
    {__LINE__,1.99999DD, "+0,000,000,000,199,999E-5"},
    {__LINE__,2.88888DD, "+0,000,000,000,288,888E-5"},
    {__LINE__,3.77777DD, "+0,000,000,000,377,777E-5"},
    {__LINE__,4.66666DD, "+0,000,000,000,466,666E-5"},
#else
    {__LINE__,1.99999DD, "BID not supported."},
    {__LINE__,2.88888DD, "BID not supported."},
    {__LINE__,3.77777DD, "BID not supported."},
    {__LINE__,4.66666DD, "BID not supported."},
#endif
    {0,0,0 }
  };

  d128_type d128types[] =
  {
#ifdef _DPD_BACKEND
    {__LINE__,7.0DL, "+0,000,000,000,000,000,000,000,000,000,000,070E-1"},
    {__LINE__,6.0DL, "+0,000,000,000,000,000,000,000,000,000,000,060E-1"},
    {__LINE__,5.0DL, "+0,000,000,000,000,000,000,000,000,000,000,050E-1"},
#else
    {__LINE__,7.0DL, "BID not supported."},
    {__LINE__,6.0DL, "BID not supported."},
    {__LINE__,5.0DL, "BID not supported."},
#endif
    {0,0,0 }
  };

  /* We're not going to check these.  They're simply filler.  */
  s.df = 1.22222DF;
  s.dd = 2.33333DD;
  s.dl = 3.44444DL;

  t.df = 4.55555DF;
  t.dd = 5.66666DD;
  t.dl = 6.77777DL;

  /* This may seem like a trivial test but the compiler screwed this up at one
   * point and it'd be nice to not have this ever regress without us knowing it.
   */
  x = param_test(d128types[0].d,d64types[0].d,d32types[0].d,&s,&t,d32types[1].d,d64types[1].d,d64types[2].d,d128types[1].d,d64types[3].d,d128types[2].d);

  /* Make sure the compiler hasn't screwed up the save/restore of these
   * _Decimal types.  */
  d32_type *d32p;
  for (d32p = d32types; d32p->line; d32p++)
    {
      _DC_P(__FILE__,d32p->line, d32p->expect,d32p->d);
    }

  d64_type *d64p;
  for (d64p = d64types; d64p->line; d64p++)
    {
      _DC_P(__FILE__,d64p->line, d64p->expect,d64p->d);
    }

  d128_type *d128p;
  for (d128p = d128types; d128p->line; d128p++)
    {
      _DC_P(__FILE__,d128p->line, d128p->expect,d128p->d);
    }

  _REPORT();

  /* fail comes from scaffold.c  */
  return fail;
}
