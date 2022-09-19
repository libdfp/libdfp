/* Test printf_dfp facility.

   Copyright (C) 2020 Free Software Foundation, Inc.

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
   if not, write to the Free Software Foundation, Inc., 51 Franklin
   Street, Fifth Floor, Boston, MA 02110-1301 USA.

   Please see libdfp/COPYING.txt for more information.  */

#ifndef __STDC_WANT_DEC_FP__
#define __STDC_WANT_DEC_FP__ 1
#endif

#include <fenv.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>

#define _WANT_PC
#include "scaffold.c"

struct test {
    int l;
    const char *fmt;
    const char *result;
    size_t n;
    size_t len;
    _Decimal32 d32;
    _Decimal64 d64;
    _Decimal128 d128;
};

struct test tests[] = {
      /* Test some bogus input.  We shouldn't crash, but the behavior
	 in such cases is undefined.  */
      {__LINE__, "", "", 0, 0, 0, 0, 0},
      {__LINE__, "", "", 5, 0, 0, 0, 0},
      {__LINE__, "%", "", 0, 0, 0, 0, 0},
      {__LINE__, "%", "", 5, 0, 0, 0, 0},
      {__LINE__, "%.", "", 0, 0, 0, 0, 0},
      {__LINE__, "%.", "", 5, 0, 0, 0, 0},
      {__LINE__, "%.5", "", 0, 0, 0, 0, 0},
      {__LINE__, "%.5", "", 5, 0, 0, 0, 0},
      {__LINE__, "%.55", "", 0, 0, 0, 0, 0},
      {__LINE__, "%.55", "", 5, 0, 0, 0, 0},
      {__LINE__, "%Hd", "", 0, 0, 5, 5, 5},
      {__LINE__, "%Hd", "", 5, 0, 5, 5, 5},
      /* Note: if truncated, a NUL will be placed at the end of the buffer.  */
      {__LINE__, "%.6e", "1.23", 5, 12, 123456e10DF, 123456e10DD, 123456e10DL},
      {__LINE__, 0, 0, 0, 0, 0, 0, 0}
};


int
main (void) {
  struct test *t = tests;
  for (; t->fmt; t++) {
    _SFC_P_N (__FILE__, t->l, t->result, t->fmt, strfromd32, t->d32, t->n, t->len);
    _SFC_P_N (__FILE__, t->l, t->result, t->fmt, strfromd64, t->d64, t->n, t->len);
    _SFC_P_N (__FILE__, t->l, t->result, t->fmt, strfromd128, t->d128, t->n, t->len);
  }

  /* Test for empty buffer length computations.  */
  _SFC_P_NULL (__FILE__, __LINE__, NULL, "%f", strfromd32, 1234567.DF, (size_t)14);
  _SFC_P_NULL (__FILE__, __LINE__, NULL, "%f", strfromd64, 1234567.DF, (size_t)14);
  _SFC_P_NULL (__FILE__, __LINE__, NULL, "%f", strfromd128, 1234567.DF, (size_t)14);

  /* And some bogus buffer,  but n = 0.  */
  _SFC_P_NULL (__FILE__, __LINE__, (char*)1, "%f", strfromd32, 1234567.DF, (size_t)14);
  _SFC_P_NULL (__FILE__, __LINE__, (char*)1, "%f", strfromd64, 1234567.DF, (size_t)14);
  _SFC_P_NULL (__FILE__, __LINE__, (char*)1, "%f", strfromd128, 1234567.DF, (size_t)14);

  _REPORT ();
  return fail;
}
