/* Test the ostream operator in the cpp compatibility header <dfp/decimal>.

   Copyright (C) 2011 Free Software Foundation, Inc.

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

/* This is a CPP compatibility testcase.  Pick up the _Decimal[32|64|128]
 * definitions.  */
#include <float.h>

#include <decimal>

#include <iomanip>

/* For cout support.  */
#include <iostream>

using namespace std;
using namespace std::decimal;

//#define _WANT_PC 1 /* Pick up the _PC(x,y,...) macro.  */
//#include "scaffold.c"

/* Inspired by GLIBC stdio-common/tfformat.c  */
typedef struct{
  int line;
  decimal32 d;
  const char *expect;
} d32_type;

d32_type printf_d32s[] =
{
  {__LINE__, 123.456E-9DF, "1.23456e-7" },
  {__LINE__, 123.DF, "123" },
  {0,0,0}
};

int main(void)
{
  decimal32 d32 = 0.0000006E-90DF;
  decimal64 d64 = -9.999E-3DD;
  decimal128 d128 = -1234.56789123456789123455678DL;
//  cout << d32 << "DF" << endl;
//  cout << setprecision(__DEC64_MANT_DIG__) << d64 << "DD" << endl;
//  cout << setprecision(4) << d128 << "DL" << endl;
// cout << "**************" << endl;
  cout << d32 << "DF" << endl;
  cout << d64 << "DD" << endl;
  cout.flags(ios::scientific);
  cout << d64 << "DD" << endl;
  //cout.flags(ios::scientific | ios::uppercase);
  cout.flags(ios::scientific | ios::uppercase);
  cout << d64 << "DD" << endl;
  cout << d64 << "DD" << endl;
  cout.flags();
  cout.flags(ios::fixed);
  cout << d64 << "DD" << endl;
  d64 = (decimal64) __builtin_infd64();
  cout.flags(ios::fixed | ios::uppercase);
  cout << d64 << "DD" << endl;
  cout.flags(ios::fixed);
  cout << d64 << "DD" << endl;
  return 0;
}
