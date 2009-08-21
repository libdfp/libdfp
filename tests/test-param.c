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

#include <stdio.h>
#include <float.h>

/* This testcase is designed to test that the compiler is satisfying the
 * conditions of the ABI with regard to spilling of _Decimal* type parameters
 * to the stack.  In other words, we verify the integrity of the data after a
 * function call where parameters are spilled.  This should be tested for both
 * hard and soft dfp.  */

/* char * should ref a 14 byte char array, +0,000,000E+0\0  */
extern char * decoded32 (_Decimal32, char*);
/* char * should ref a 26 byte char array, +0,000,000,000,000,000E+0\0  */
extern char * decoded64 (_Decimal64, char*);
/* char * should ref a 50 byte char array, *
 * +0,000,000,000,000,000,000,000,000,000,000,000E+0\0  */
extern char * decoded128 (_Decimal128, char*);

typedef struct sparm {
  _Decimal32 df;
  _Decimal64 dd;
  _Decimal128 dl;
} sparm_t;

/* Test parameter spilling.  */
int func(_Decimal128 d128, _Decimal64 d64, _Decimal32 d32, struct sparm *s,
struct sparm *t, _Decimal32 e32, _Decimal64 e64, _Decimal64 z64, _Decimal128 e128, _Decimal64 f64, _Decimal128 f128)
{
  volatile _Decimal128 z;
  volatile _Decimal128 y;
  z = e128;
  y = f128;
  return 0;
}

int main() {
  int x;
  struct sparm s, t;
  char buf[256];
  _Decimal32 d32,e32 = 4.44444DF;
  _Decimal64 d64,e64,f64 = 9.99999DD;
  _Decimal128 d128,e128 = 1.0DL;
  _Decimal128 f128 = 2.0DL;

  x = func(d128,d64,d32,&s,&t,e32,e64,e64,e128,f64,f128);
  decoded128(e128,&buf[0]);
  printf("%s\n",buf);
  return 0;
}
