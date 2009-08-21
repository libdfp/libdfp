/* Test printf_dfp facility.

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

#include <float.h>
#include <dfp.h>
#include <stdio.h>
#include <wchar.h> /* This should pick up the libdfp wchar in dfp/wchar.h.  */

#include "scaffold.c" /* Pick up the _PC(x,y,...) macro.  */

/* Inspired by GLIBC stdio-common/tfformat.c  */
typedef struct{
  int line;
  _Decimal128 d;
  const char *expect;
  const char *format;
} d128_type;

d128_type printf_d128s[] =
{
  {__LINE__, 0.000033333DL, "0.000033333", "%DDf"},
  /* Three digits of precision right of the decimal place.  */
  {__LINE__, 231.2315DL, "231.232", "%.3DDf"},
  /* Four digits of precision right of the decimal place.  */
  {__LINE__, 231.2315DL, "231.2315", "%.4DDf"},
  /* Space padded to 12,  Right justified.  */
  {__LINE__, 231.2315DL, "12.3", "%12.3DDf"},
  /* Left justified, Space padded to 12.  */
  {__LINE__, 231.2315DL, "12.3", "%-12.3DDf"},

  {0,0,0,0 }
};

int main (int argc, char ** argv)
{
  d128_type *dptr;

  fprintf(stdout, "Testing marker prior to register_printf_dfp() invocation.\n");
  _PC("1.234567e+00", "%e", (double) 1.234567);

  register_printf_dfp();

  /* We do this to make sure that the registration didn't mess up the printf
   * internals.  */
  fprintf(stdout, "Testing marker after register_printf_dfp() invocation.\n");
  _PC("1.234567e+00", "%e", (double) 1.234567);

  for (dptr = printf_d128s; dptr->line; dptr++)
    {
      _PC_P(__FILE__,dptr->line, dptr->expect,dptr->format,dptr->d);
    }

  _REPORT();

  return fail;
}
