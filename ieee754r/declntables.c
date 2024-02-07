/* A utility to generate the ln(t) - t + 1 table
   used by logd128.

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
   if not, write to the Free Software Foundation, Inc., 51 Franklin
   Street, Fifth Floor, Boston, MA 02110-1301 USA.

   Please see libdfp/COPYING.txt for more information.

   gcc declntables.c -O2 -ldecnumber -o declntables
   ./declntables > t_logd128.h
*/

#define DECNUMDIGITS 68

#include <decnumber/decContext.h>
#include <decnumber/decNumber.h>
#include <string.h>
#include <stdlib.h>

const decNumber *dzero;

const char COPYRIGHT[] =
"/* Generated table of ln(x) - (t - 1) table values.\n"
"\n"
"   See declntables.c to understand how, or fix these\n"
"   values if such is neeeded.\n"
"\n"
"   Copyright (C) 2019 Free Software Foundation, Inc.\n"
"\n"
"   This file is part of the Decimal Floating Point C Library.\n"
"\n"
"   This is adapted, derived, and in some spiritual form based on\n"
"   the version found in glibc's float128/ibm128 variant.\n"
"\n"
"   The Decimal Floating Point C Library is free software; you can\n"
"   redistribute it and/or modify it under the terms of the GNU Lesser\n"
"   General Public License version 2.1.\n"
"\n"
"   The Decimal Floating Point C Library is distributed in the hope that\n"
"   it will be useful, but WITHOUT ANY WARRANTY; without even the implied\n"
"   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See\n"
"   the GNU Lesser General Public License version 2.1 for more details.\n"
"\n"
"   You should have received a copy of the GNU Lesser General Public\n"
"   License version 2.1 along with the Decimal Floating Point C Library;\n"
"   if not, write to the Free Software Foundation, Inc., 51 Franklin\n"
"   Street, Fifth Floor, Boston, MA 02110-1301 USA.\n"
"\n"
"   Please see libdfp/COPYING.txt for more information.  */\n";

static char TBLHDRSTR[] =
  "static const struct { _Decimal128 t; _Decimal128 lnt; } %s[] = {\n";

static void
p_lntptm1 (int ix, decNumber const *_1, decContext * ctx,
	   decNumber const *_1000)
{
  char dstr[DECNUMDIGITS + 4 + 4 + 1];
  char ddstr[sizeof (dstr)];
  decNumber x, tp1, lnt;

  /* Compute in a higher precision to avoid precision loss during
     final subtraction. */
  ctx->digits *= 2;
  decNumberFromInt32 (&x, ix);
  decNumberDivide (&x, &x, _1000, ctx);
  decNumberLn (&lnt, &x, ctx);
  decNumberSubtract (&tp1, &x, _1, ctx);
  decNumberSubtract (&tp1, &lnt, &tp1, ctx);
  ctx->digits /= 2;
  decNumberAdd( &tp1, dzero, &tp1, ctx);
  decNumberToString (&tp1, dstr);
  decNumberToString (&x, ddstr);
  printf ("  {%5sDL, %+36sDL},\n", ddstr, dstr);
}

int
main (void)
{

  enum rounding rnd;
  int prec = 100;		/* 10 ^ n - 1 digits of precision, 3 digits suffices for float128 p(x) */
  int ixs = 2 * prec;
  int decade_shift = prec * 10;
  int ixe = ixs * 10;
  decContext ctx;
  decNumber _1, _1000, _0;
  dzero = &_0;


  decContextDefault (&ctx, DEC_INIT_DECIMAL128);
  decNumberZero (&_0);
  decNumberFromInt32 (&_1, 1);
  decNumberFromInt32 (&_1000, decade_shift);

  puts (COPYRIGHT);
  printf (TBLHDRSTR, "lntvals");
  printf ("  {%5sDL, %+36sDL},\n", "1.0", "0.0");
  for (int ix = decade_shift + prec / 10; ix < ixe; ix += 10)
    {
      p_lntptm1 (ix, &_1, &ctx, &_1000);
    }
  for (int ix = ixs; ix < decade_shift; ix += 1)
    {
      p_lntptm1 (ix, &_1, &ctx, &_1000);
    }
  /* And one final entry if the rounded value rounds up to 1.0. */
  printf ("  {%5sDL, %+36sDL},\n", "1.0", "0.0");
  printf ("};\n");
  printf ("#define LNTVALS_START %d\n", prec);

  return 0;
};
