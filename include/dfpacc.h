/* DFP Arithmetic, Conversion, and Comparison routines w/ exceptions.

   Copyright (C) 2009 IBM Corporation.
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

   Please see dfp/COPYING.txt for more information.  */

#ifndef _DFPACC_PRIVATE_H
#define _DFPACC_PRIVATE_H 1

#ifndef PASTE
# define PASTE(x,y) PASTE2(x,y)
# define PASTE2(x,y) x##y
#endif

#if __DECIMAL_BID_FORMAT__==1
# define __BACKEND_(x)  PASTE(__bid_,x)
#else
# define __BACKEND_(x)  PASTE(__dpd_,x)
#endif

/* This file contains the prototypes for the DFP arithmetic, conversion, and
 * comparison routines.  These also exist in libgcc, but these versions
 * support decimal floating point rounding control and exception support
 * whilst those in libgcc do not.  GCC will place these in the code in
 * soft-dfp in place of arithmetic, conversion, or comparison functions, e.g.
 *   _Decimal128 foo = bar + bat;
 * will be replaced with:
 *   _Decima128 = __dpd_addtd3(bar,bat);  */

_Decimal32  __BACKEND_(addsd3) (_Decimal32,_Decimal32);
_Decimal64  __BACKEND_(adddd3) (_Decimal64,_Decimal64);
_Decimal128 __BACKEND_(addtd3) (_Decimal128,_Decimal128);

_Decimal32  __BACKEND_(divsd3) (_Decimal32,_Decimal32);
_Decimal64  __BACKEND_(divdd3) (_Decimal64,_Decimal64);
_Decimal128 __BACKEND_(divtd3) (_Decimal128,_Decimal128);

int __BACKEND_(eqsd2) (_Decimal32,_Decimal32);
int __BACKEND_(eqdd2) (_Decimal64,_Decimal64);
int __BACKEND_(eqtd2) (_Decimal128,_Decimal128);

_Decimal64  __BACKEND_(extendsddd2) (_Decimal32);
_Decimal128  __BACKEND_(extendsdtd2) (_Decimal32);
_Decimal128  __BACKEND_(extendddtd2) (_Decimal64);

_Decimal32  __BACKEND_(extendsfsd) (float);
double  __BACKEND_(extendsddf) (_Decimal32);
long double  __BACKEND_(extendsdtf) (_Decimal32);

_Decimal64  __BACKEND_(extendsfdd) (float);
_Decimal64  __BACKEND_(extenddfdd) (double);
long double  __BACKEND_(extendddtf) (_Decimal64);

_Decimal128  __BACKEND_(extendsftd) (float);
_Decimal128  __BACKEND_(extenddftd) (double);
_Decimal128  __BACKEND_(extendtftd) (long double);

int __BACKEND_(fixsdsi) (_Decimal32);
int __BACKEND_(fixddsi) (_Decimal64);
int __BACKEND_(fixtdsi) (_Decimal128);

long long __BACKEND_(fixsddi) (_Decimal32);
long long __BACKEND_(fixdddi) (_Decimal64);
long long __BACKEND_(fixtddi) (_Decimal128);

unsigned int __BACKEND_(fixunssdsi) (_Decimal32);
unsigned int __BACKEND_(fixunsddsi) (_Decimal64);
unsigned int __BACKEND_(fixunstdsi) (_Decimal128);

unsigned long long __BACKEND_(fixunssddi) (_Decimal32);
unsigned long long __BACKEND_(fixunsdddi) (_Decimal64);
unsigned long long __BACKEND_(fixunstddi) (_Decimal128);

_Decimal64 __BACKEND_(floatdidd) (long long);
_Decimal32 __BACKEND_(floatdisd) (long long);
_Decimal128 __BACKEND_(floatditd) (long long);

_Decimal64 __BACKEND_(floatsidd) (int);
_Decimal32 __BACKEND_(floatsisd) (int);
_Decimal128 __BACKEND_(floatsitd) (int);

_Decimal64 __BACKEND_(floatunsdidd) (unsigned long long);
_Decimal32 __BACKEND_(floatunsdisd) (unsigned long long);
_Decimal128 __BACKEND_(floatunsditd) (unsigned long long);

_Decimal64 __BACKEND_(floatunssidd) (unsigned int);
_Decimal32 __BACKEND_(floatunssisd) (unsigned int);
_Decimal128 __BACKEND_(floatunssitd) (unsigned int);

int __BACKEND_(gesd2) (_Decimal32,_Decimal32);
int __BACKEND_(gedd2) (_Decimal64,_Decimal64);
int __BACKEND_(getd2) (_Decimal128,_Decimal128);

int __BACKEND_(gtsd2) (_Decimal32,_Decimal32);
int __BACKEND_(gtdd2) (_Decimal64,_Decimal64);
int __BACKEND_(gttd2) (_Decimal128,_Decimal128);

int __BACKEND_(lesd2) (_Decimal32,_Decimal32);
int __BACKEND_(ledd2) (_Decimal64,_Decimal64);
int __BACKEND_(letd2) (_Decimal128,_Decimal128);

int __BACKEND_(ltsd2) (_Decimal32,_Decimal32);
int __BACKEND_(ltdd2) (_Decimal64,_Decimal64);
int __BACKEND_(lttd2) (_Decimal128,_Decimal128);

_Decimal32  __BACKEND_(mulsd3) (_Decimal32,_Decimal32);
_Decimal64  __BACKEND_(muldd3) (_Decimal64,_Decimal64);
_Decimal128 __BACKEND_(multd3) (_Decimal128,_Decimal128);

int __BACKEND_(nesd2) (_Decimal32,_Decimal32);
int __BACKEND_(nedd2) (_Decimal64,_Decimal64);
int __BACKEND_(netd2) (_Decimal128,_Decimal128);

_Decimal32  __BACKEND_(subsd3) (_Decimal32,_Decimal32);
_Decimal64  __BACKEND_(subdd3) (_Decimal64,_Decimal64);
_Decimal128 __BACKEND_(subtd3) (_Decimal128,_Decimal128);

_Decimal32 __BACKEND_(trunctdsd2) (_Decimal128);
_Decimal32 __BACKEND_(truncddsd2) (_Decimal64);
_Decimal64 __BACKEND_(trunctddd2) (_Decimal128);

float __BACKEND_(truncsdsf) (_Decimal32);
_Decimal32 __BACKEND_(truncdfsd) (double);
_Decimal32 __BACKEND_(trunctfsd) (long double);

float __BACKEND_(truncddsf) (_Decimal64);
double __BACKEND_(truncdddf) (_Decimal64);
_Decimal64 __BACKEND_(trunctfdd) (long double);

float __BACKEND_(trunctdsf) (_Decimal128);
double __BACKEND_(trunctddf) (_Decimal128);
long double __BACKEND_(trunctdtf) (_Decimal128);

int __BACKEND_(unordsd2) (_Decimal32,_Decimal32);
int __BACKEND_(unorddd2) (_Decimal64,_Decimal64);
int __BACKEND_(unordtd2) (_Decimal128,_Decimal128);
#endif /* _DFPACC_PRIVATE_H  */
