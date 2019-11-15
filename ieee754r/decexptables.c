/* A utility to generate the crazy looking double-decimal128 tables
   used by expd128.

   Copyright (C) 2019 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   This is adapted, derived, and in some spiritual form based on
   the version found in glibc's float128/ibm128 variant.

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

   Please see libdfp/COPYING.txt for more information.


   In the non-zero, by hopefully approximating zero case, I have
   done this wrong, an educated observer can regenerate and fix
   this tables.

   We use a hypothetical _Decimal256 and use rounding tricks to
   approximate a 256 digit decimal value for select and small
   exp values.

   We use decNumber directly and live with <= 1ULP whereby it is
   usually correctly rounded except in cases whereby it cannot
   describe but claims exist.
*/

/* ensure room enough for full double-double precision */
#define DECNUMDIGITS 68

#include "decNumber.h"

const decNumber *dzero;

static char TBLHDRSTR[] =
  "static const struct { _Decimal128 hi; _Decimal128 lo; _Decimal128 out; } %s[] = {\n";

void
gentable (const decNumber * start, int tmax, decContext * ctxp)
{
  enum rounding rnd;
  char dstr[DECNUMDIGITS + 4 + 4 + 1];
  char ddstr[sizeof (dstr)];
  char dddstr[sizeof (dstr)];
  char ddddstr[sizeof (dstr)];

  for (int x = -tmax; x <= tmax; x++)
    {
      decNumber expv_rnd;
      decNumber expv;
      decNumber expv_fdg;
      decNumber expv_l;
      decNumber expv_h;
      decNumber tmp;
      decNumberFromInt32 (&tmp, x);
      decNumberMultiply (&tmp, &tmp, start, ctxp);

      /* Only compute the value to the target precision (34) */
      ctxp->digits /= 2;
      decNumberExp (&expv, &tmp, ctxp);
      ctxp->digits *= 2;

      /* Truncate result to 17 digits. */
      ctxp->digits /= 4;
      rnd = ctxp->round;
      ctxp->round = DEC_ROUND_DOWN;
      decNumberAdd (&expv_rnd, &expv, dzero, ctxp);
      ctxp->round = rnd;
      ctxp->digits *= 4;

      /* This is kind of crude. Does it stand that +/- 1ULP gives us a closer
         a closer fit (e.g better reduction of input? */
      decNumberLn (&expv_fdg, &expv_rnd, ctxp);

      ctxp->digits /= 2;
      decNumberExp (&tmp, &expv_fdg, ctxp);
      ctxp->digits *= 2;

      /* Compute high and low D32 parts */
      ctxp->digits /= 2;
      decNumberAdd (&expv_h, &expv_fdg, dzero, ctxp);
      ctxp->digits *= 2;
      decNumberSubtract (&expv_l, &expv_fdg, &expv_h, ctxp);

      decNumberToString (&expv_h, dstr);
      decNumberToString (&expv_l, ddstr);
      decNumberToString (&expv_l, ddstr);
      decNumberToString (&expv_rnd, dddstr);
      decNumberToString (&expv_fdg, ddddstr);
      if (x != 0)
	printf ("/*%5d*/ {%38sDL, %41sDL, %sDL}, /* %s */\n", x, dstr, ddstr,
		dddstr, ddddstr);
      else
	printf ("/*    0*/ {%38sDL, %41sDL, %sDL}, /* %s */\n", "0.0", "0.0",
		"1.0", ddddstr);
    }
}

int
main (void)
{

  enum rounding rnd;
  decContext ctx;
  decNumber ln10d2, ln10, ten, two, tp2, tmp, ln10dt1max;
  decNumber _17, _0, _1;
  int t1max;
  char dstr[DECNUMDIGITS + 4 + 4 + 1];
  char ddstr[sizeof (dstr)];
  char dddstr[sizeof (dstr)];
  char ddddstr[sizeof (dstr)];
  dzero = &_0;
  /*
     Table one, fit values for where x <= |ln(10)/2| in
     round-to-nearest-int(x*1000) steps, or -115 to 115.
   */


  decContextDefault (&ctx, DEC_INIT_DECIMAL128);
  ctx.digits *= 2;		/* The d128 exponent range is sufficient here */

  decNumberFromInt32 (&ten, 10);
  decNumberFromInt32 (&tp2, 100);
  decNumberFromInt32 (&two, 2);
  decNumberFromInt32 (&_17, 17);
  decNumberFromInt32 (&_0, 0);
  decNumberFromInt32 (&_1, 1);
  decNumberLn (&ln10, &ten, &ctx);
  decNumberDivide (&ln10d2, &ln10, &two, &ctx);
  decNumberMultiply (&tmp, &ln10d2, &tp2, &ctx);
  decNumberToIntegralValue (&tmp, &tmp, &ctx);
  t1max = decNumberToInt32 (&tmp, &ctx);
  decNumberFromInt32 (&ln10dt1max, t1max * 2);
  decNumberDivide (&ln10dt1max, &ln10, &ln10dt1max, &ctx);

  printf (TBLHDRSTR, "exptbl1");
  decNumberFromInt32 (&ln10dt1max, 100);
  decNumberDivide (&ln10dt1max, &_1, &ln10dt1max, &ctx);
  gentable (&ln10dt1max, t1max, &ctx);
  printf ("#define EXPTBL1OFF %d\n", t1max);
  printf ("};\n");

  /*
     Second table. Not sure how to range reduce here. Values
     are between +/-0.01
     100 + 1 for possible integer rounding.
   */
  t1max = 101;
  decNumberFromInt32 (&ln10dt1max, 100 * 100);
  decNumberDivide (&ln10dt1max, &_1, &ln10dt1max, &ctx);
  printf (TBLHDRSTR, "exptbl2");
  gentable (&ln10dt1max, t1max, &ctx);
  printf ("#define EXPTBL2OFF %d\n", t1max);
  printf ("};\n");
  return 0;
};

/* This is kind of slow, but it simplifies life greatly. */
#include "decNumber.c"
#include "decContext.c"
