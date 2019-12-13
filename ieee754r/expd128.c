/* Returns the value of e raised to the power of x

   Copyright (C) 2019 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   This is adapted, derived, and in some spiritual form based on
   the version found in glibc's float128/ibm128 variant e_expl.c.

   Largely based on quad-precision code
   Contributed by Jakub Jelinek <jj@ultra.linux.cz>
   Partly based on double-precision code
   by Geoffrey Keating <geoffk@ozemail.com.au>

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

#include <math.h>
#include <ieee754r_private.h>
#include <dfp/fenv.h>

#include "dfp_inline.h"

/* Generated lookup tables */
#include "t_expd128.h"

static const _Decimal128 __1_LN_10 = 4.342944819032518276511289189166051e-1DL;

static const _Decimal128 LN10_0 = 2.30258509299404568401799145468DL;
static const _Decimal128 LN10_1 =
  0.000000000000000000000000000004364207601101488628772976033327900DL;

/* Round a fractional value to an integer when quantizing. Note we must
   choose a value with a uniquely representable value. */
const _Decimal128 QRNDTOI = 2.0e33DL - 0.01DL;
const _Decimal128 QRNDTOID2 = 2.0e31DL - 0.0001DL;
const _Decimal128 QRNDTOID4 = 2.0e29DL - 0.000001DL;

/*
    Chebyshev polynomial coefficients for (exp(x)-1)/x.

    These are the glibc constants as of today. They are
    slightly more accurate than can fit in d128.
*/
static const _Decimal128 P1 = 0.5L;
static const _Decimal128 P2 = 1.66666666666666666666666666666666683E-01DL;
static const _Decimal128 P3 = 4.16666666666666666666654902320001674E-02DL;
static const _Decimal128 P4 = 8.33333333333333333333314659767198461E-03DL;
static const _Decimal128 P5 = 1.38888888889899438565058018857254025E-03DL;
static const _Decimal128 P6 = 1.98412698413981650382436541785404286E-04DL;

_Decimal128
__expd128 (_Decimal128 val)
{
  _Decimal128 n, t, ex, x, xl, cp;
  int tval1, tval2;

  /* Computed by interval using mpfr in 512b precision */
  const _Decimal128 max = 14149.38539644841072829055748903541DL;
  const _Decimal128 min = -14144.78022626242263692252150612604DL;

  if (!(val >= min && val <= max))
    {
      /* should values below generate under/overflow? */
      if (__builtin_isnan (val))
	return val + val;
      else if (val > max)
	return DEC_INFINITY;
      else
	return 0.0DL;
    }

  SET_RESTORE_DROUND (FE_DEC_TONEAREST);

  /*
     First, reduce range via fitting an integer such that
     10^(n*ln(10)).

     This value will be 6145 >= n >= 6143. Ideally a little
     smaller, but rounding mode increases this.

     Round the value against the smallest integral val.
   */
  n = __quantized128 (val * __1_LN_10, QRNDTOI);

  /* xl is the remainder of val - n * ln(10).  */
  x = val - n * LN10_0;
  xl = n * -LN10_1;

  /* 10^n * e^(x), where x < |ln(10)|.  */

  /* Fit to a value x.yz value.  */
  t = __quantized128 (x, QRNDTOID2);
  tval1 = (int) (t * 1e2DL);

  /* |t| <= 115 when rounding to nearest (rtn). */
  x -= exptbl1[tval1 + EXPTBL1OFF].hi;
  xl -= exptbl1[tval1 + EXPTBL1OFF].lo;
  ex = exptbl1[tval1 + EXPTBL1OFF].out;

  /* Now determine a suitable tval2. |t| <= .0101 with rtn  */
  t = __quantized128 (x, QRNDTOID4);
  tval2 = (int) (t * 1e4DL);

  x -= exptbl2[tval2 + EXPTBL2OFF].hi;
  xl -= exptbl2[tval2 + EXPTBL2OFF].lo;
  ex *= exptbl2[tval2 + EXPTBL2OFF].out;

  /* Now compute a polynomial approximation with sufficient accuracy */
  x = x + xl;
  cp = x + x * x * (P1 + x * (P2 + x * (P3 + x * (P4 + x * (P5 + x * P6)))));
  ex = ex + ex * cp;

  /* Fit correct exponent and return */
  ex = __ldexpd128 (ex, n);
  return ex;
}

weak_alias (__expd128, expd128)
