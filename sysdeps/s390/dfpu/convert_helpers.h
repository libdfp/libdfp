/* Prototypes for assisting converting larger numbers.

   Copyright (C) 2016 Free Software Foundation, Inc.

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

#ifndef _CONVERTHELPERS_
#define _CONVERTHELPERS_

#include <dfpfenv_private.h>

/* Define combine_and_truncd32 and combine_and_truncd64.  */
static inline _Decimal64
combine_and_truncd64 (_Decimal128 hi, _Decimal128 mid, _Decimal128 low)
{
  _Decimal128 tmp;
  _Decimal128 chunk = 1e17DL;
  asm volatile ("" : "+f" (hi), "+f" (mid), "+f" (low), "+f" (chunk));
  int old_rm = __fe_dec_getround ();
  __fe_dec_setround (7); /* Ensure, that rounding mode: "Prepare for shorter
			    precision" is used for exactly these instructions.
			 */
  tmp = (((hi * chunk) + mid) * chunk) + low;
  asm volatile ("" : "+f" (tmp), "+d" (old_rm));
  __fe_dec_setround (old_rm);
  return (_Decimal64) tmp;
}

static inline _Decimal32
combine_and_truncd32 (_Decimal128 hi, _Decimal128 mid, _Decimal128 low)
{
  _Decimal128 tmp;
  _Decimal128 chunk = 1e17DL;
  _Decimal64 tmp64;
  asm volatile ("" : "+f" (hi), "+f" (mid), "+f" (low), "+f" (chunk));
  int old_rm = __fe_dec_getround ();
  __fe_dec_setround (7); /* Ensure, that rounding mode: "Prepare for shorter
			    precision" is used for exactly these instructions.
			 */
  tmp = (((hi * chunk) + mid) * chunk) + low;
  tmp64 = (_Decimal64) tmp;
  asm volatile ("" : "+f" (tmp64), "+d" (old_rm));
  __fe_dec_setround (old_rm);
  return (_Decimal32) tmp64;
}

/* This is a simplified version of frexpd* which instead
   returns frexpd*(x) * 10^shift.  _y is assumed to be
   10^shift, but is not used here.  */
static inline _Decimal128
getmantandexpd128(_Decimal128 x, int *y, int shift
		  , _Decimal128 __attribute((unused)) _y)
{
  _Decimal128 result;
  int digits, exponent, set_exponent;

  /* Extract biased exponent of x.  */
  asm ("eextr %0,%1\n\t" : "=d" (exponent) : "f" (x));

  /* Get number of digits of x.  */
  asm ("esxtr %0,%1\n\t" : "=d" (digits) : "f" (x));

  *y = exponent - 6176 + digits;

  set_exponent = exponent - digits + shift + 1;

  /* Set number of digits to result.  */
  asm ("llgfr %2,%2\n\t" /* exponent must be 64bit!  */
       "iextr %0,%1,%2\n\t" : "=f" (result) : "f" (x), "d" (set_exponent));

  return result;
}

static inline _Decimal64
getmantandexpd64(_Decimal64 x, int *y, int shift
		 , _Decimal64 __attribute((unused)) _y)
{
  _Decimal64 result;
  int digits, exponent, set_exponent;

  /* Extract biased exponent of x.  */
  asm ("eedtr %0,%1\n\t" : "=d" (exponent) : "f" (x));

  /* Get number of digits of x.  */
  asm ("esdtr %0,%1\n\t" : "=d" (digits) : "f" (x));

  *y = exponent - 398 + digits;

  set_exponent = exponent - digits + shift + 1;

  /* Set number of digits to result.  */
  asm ("llgfr %2,%2\n\t" /* exponent must be 64bit!  */
       "iedtr %0,%1,%2\n\t" : "=f" (result) : "f" (x), "d" (set_exponent));

  return result;
}

static inline _Decimal32
getmantandexpd32(_Decimal32 x, int *y, int shift, _Decimal32 _y)
{
  return getmantandexpd64((_Decimal64) x, y, shift, _y);
}

/* Convert the entire mantissa to a quadword, and
   return the exponent in the form .a0...a33 * 10^exp
 */
#ifdef INT128
static inline INT128
getmantd128(_Decimal128 dq, long *exp)
{
  long e, tmp;
  INT128 mantissa;
  _Decimal128 dq2;
  long long int m, m2;

  /* Extract biased exponent.  */
  asm ("eextr %0,%1\n\t" : "=d" (e) : "f" (dq));

  asm (/* Set exponent 0 to dq (is 6176).  */
       "lghi %[tmp],6176\n\t"
       "iextr %[dq],%[dq],%[tmp]\n\t"
       /* Set exponent -17 to dq2 (is 6176-17).  */
       "lghi %[tmp],6159\n\t"
       "iextr %[dq2],%[dq],%[tmp]\n\t"
       /* Lop of lower 17 digits of dq (/=10^17).  */
       "srxt %[dq],%[dq],17\n\t"
       /* Lop of upper 17 digits of dq2 (*=10^17).  */
       "slxt %[dq2],%[dq2],17\n\t"
       : [dq] "+f" (dq), [dq2] "=f" (dq2) ,[tmp] "=d" (tmp) );

  /* Convert mantissa of dq to fixed.
     On 64bit, long long int is stored in one register.
     (31bit does not support INT128 data-type.)  */
  asm ("cgxtr %[m],0,%[dq]\n\t"
       "cgxtr %[m2],0,%[dq2]\n\t"
       : [m] "=d" (m), [m2] "=d" (m2)
       : [dq] "f" (dq), [dq2] "f" (dq2)
       );

  mantissa = (1E17L * m) + m2;

  *exp = e - 6176;

  return mantissa;
}
#endif

/* Convert the entire mantissa to a doubleword and
   return the normalized exponent, and the mantissa.  */
static inline long long int
getmantd64(_Decimal64 dd, long *exp)
{
  long e, tmp;
  long long int mantissa;

  /* Extract biased exponent.  */
  asm ("eedtr %0,%1\n\t" : "=d" (e) : "f" (dd));

  /* Set exponent of dd to 0.  */
  asm ("lghi %1,398\n\t" /* Exponent 0 is biased exponent 398 in 64bit reg.  */
       "iedtr %0,%0,%1\n\t"
       : "+f" (dd) ,"=d" (tmp) );

  /* Convert mantissa of dd to fixed.  */
#ifdef __s390x__
  /* On 64bit, long long int is stored in one register.  */
  asm ("cgdtr %[mant],0,%[dfp]\n\t"
       : [mant] "=d" (mantissa)
       : [dfp] "f" (dd));
#else
  /* On 31bit, long long int is stored in a register pair!
     Extract mantissa to 64bit tmp register and store it to memory.  */
  asm ("cgdtr %[tmp],0,%[dfp]\n\t"
       "stg %[tmp],%[mant]\n\t" /* Store 64bit register to memory.  */
       : [tmp] "=&d" (tmp)
       : [dfp] "f" (dd), [mant] "T" (&mantissa)
       : "memory");
#endif

  *exp = e - 398;

  return mantissa;
}

static inline int
getmantd32(_Decimal32 sd, long *exp)
{
  return getmantd64((_Decimal64) sd, exp);
}

/* Truncate quickly.  No need to validate input.  */
static inline _Decimal128
fast_truncd128 (_Decimal128 a)
{
  /* Load FP Integer with round toward 0 without inexact-exception.  */
  asm ("fixtr %0,5,%0,4\n\t" : "+f" (a));
  return a;
}

static inline _Decimal64
fast_truncd64 (_Decimal64 a)
{
  /* Load FP Integer with round toward 0 without inexact-exception.  */
  asm ("fidtr %0,5,%0,4\n\t" : "+f" (a));
  return a;
}

static inline _Decimal32
fast_truncd32 (_Decimal32 a)
{
  return (_Decimal32) fast_truncd64((_Decimal64) a);
}

#endif /* _CONVERTHELPERS_ */
