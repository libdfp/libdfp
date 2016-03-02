/* Unit test all casts from|to _Decimal32|64|128 types.

   Copyright (C) 2016 Free Software Foundation, Inc.

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
   if not, write to the Free Software Foundation, Inc., 59 Temple Place,
   Suite 330, Boston, MA 02111-1307 USA.

   Please see libdfp/COPYING.txt for more information.  */

#ifndef __STDC_WANT_DEC_FP__
#define __STDC_WANT_DEC_FP__
#endif

#include <stdio.h>
#define _WANT_PC 1 /* Pick up the _PC(x,y,...) macro.  */
#include "scaffold.c"

_Decimal32 castd128tod32 (_Decimal128 x);
_Decimal64 castd128tod64 (_Decimal128 x);

_Decimal32 castd64tod32 (_Decimal64 x);
_Decimal128 castd64tod128 (_Decimal64 x);

_Decimal64 castd32tod64 (_Decimal32 x);
_Decimal128 castd32tod128 (_Decimal32 x);

int main (void)
{
  /* In case of a type cast from|to _Decimal32|64|128, gcc emits
     either the concrete instructions or a call to the corresponding
     trunc*|extend* functions, which are implemented in base-math/.
     If these functions also calls the same trunc*|extend* function,
     then the application segfaults due to endless recursive call.
     Thus this test checks the possible casts.  */

  {
    _Decimal128 in = 1.23DL;
    _Decimal32 out = castd128tod32 (in);
    _PC("1.23", "%.2Hf", out);
  }

  {
    _Decimal128 in = 1.23DL;
    _Decimal64 out = castd128tod64 (in);
    _PC("1.23", "%.2Df", out);
  }

  {
    _Decimal64 in = 1.23DD;
    _Decimal32 out = castd64tod32 (in);
    _PC("1.23", "%.2Hf", out);
  }

  {
    _Decimal64 in = 1.23DD;
    _Decimal128 out = castd64tod128 (in);
    _PC("1.23", "%.2DDf", out);
  }

  {
    _Decimal32 in = 1.23DF;
    _Decimal64 out = castd32tod64 (in);
    _PC("1.23", "%.2Df", out);
  }

  {
    _Decimal32 in = 1.23DF;
    _Decimal128 out = castd32tod128 (in);
    _PC("1.23", "%.2DDf", out);
  }

  _REPORT();

  /* fail comes from scaffold.c  */
  return fail;
}

_Decimal32
castd128tod32 (_Decimal128 x)
{
  return (_Decimal32) x;
}

_Decimal64
castd128tod64 (_Decimal128 x)
{
  return (_Decimal64) x;
}

_Decimal32
castd64tod32 (_Decimal64 x)
{
  return (_Decimal32) x;
}

_Decimal128
castd64tod128 (_Decimal64 x)
{
  return (_Decimal128) x;
}

_Decimal64
castd32tod64 (_Decimal32 x)
{
  return (_Decimal64) x;
}

_Decimal128
castd32tod128 (_Decimal32 x)
{
  return (_Decimal128) x;
}
