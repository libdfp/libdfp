/* Test GCC PR<number> which causes and ICE in soft-dfp.

   Copyright (C) 2012 Free Software Foundation, Inc.

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

int
foo (_Decimal64 x, _Decimal64 y)
{
  /* This is known to cause a GCC ICE when libdfp is compiled w/o -mcpu and -O1
     optimization level or greater.
     http://gcc.gnu.org/bugzilla/show_bug.cgi?id=52140  */
  return (x < y) || (x > y);
}

int main(void)
{
  int ret = -1;
  _Decimal64 a = 12.45DD;
  _Decimal64 b = 12.43DD;
  ret = foo (a,b);
  return 0;
}
