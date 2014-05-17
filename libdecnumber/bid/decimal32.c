/* Copyright (C) 2007-2014 Free Software Foundation, Inc.

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

#include "dconfig.h"          /* GCC definitions */
#define  DECNUMDIGITS  7      /* make decNumbers with space for 7 */
#include "decNumber.h"        /* base number library */
#include "decNumberLocal.h"   /* decNumber local types, etc. */
#include "decimal32.h"        /* our primary include */
#include "bid-coeffbits.h"    /* for bid_required_bits  */

#if DECDPUN != 3
# error "_Decimal32 BID implementation only support DECDPUN==3"
#endif

decimal32 *decimal32FromString (decimal32 *, const char *, decContext *);
char *decimal32ToString (const decimal32 *, char *);
char *decimal32ToEngString (const decimal32 *, char *);
decimal32 *decimal32FromNumber (decimal32 *, const decNumber *, decContext *);
decNumber *decimal32ToNumber (const decimal32 *, decNumber *);

static void decDigitsFromBID (decNumber *dn, uInt sour);
static void decDigitsToBID (const decNumber *dn, uInt *sour);


/* ------------------------------------------------------------------ */
/* BID special encoding definitions.                                  */
/* ------------------------------------------------------------------ */

#define BID_SIGN_MASK          0x80000000
#define BID_INF_ENC_MASK       0x78000000
#define BID_NAN_ENC_MASK       0x7C000000
#define BID_SNAN_ENC_MASK      0x7E000000
#define BID_EXPONENT_ENC_MASK  0x60000000
#define BID_EXP_MASK32         0xff
#define BID_EXP_SHIFT_LARGE32  21
#define BID_EXP_SHIFT_SMALL32  23

/* ------------------------------------------------------------------ */
/* Decimal 32-bit format module 				      */
/* ------------------------------------------------------------------ */
/* This module comprises the routines for decimal32 format numbers.   */
/* Conversions are supplied to and from decNumber and String.	      */
/*								      */
/* This is used when decNumber provides operations, either for all    */
/* operations or as a proxy between decNumber and decSingle.	      */
/*								      */
/* Error handling is the same as decNumber (qv.).		      */
/* ------------------------------------------------------------------ */
decimal32 *
decimal32FromNumber (decimal32 *d32, const decNumber *dn, decContext *set)
{
  uInt status = 0;
  Int ae;			/* adjusted exponent */
  decNumber dw;
  decContext dc;
  uInt exp;
  uInt uiwork;
  uInt targ = 0;
  Int  shift;

  /* If the number has too many digits, or the exponent could be
     out of range then reduce the number under the appropriate
     constraints.  This could push the number to Infinity or zero,
     so this check and rounding must be done before generating the
     decimal32.  */
  ae = dn->exponent + dn->digits - 1;	/* [0 if special] */
  if (dn->digits > DECIMAL32_Pmax	/* too many digits */
      || ae > DECIMAL32_Emax		/* likely overflow */
      || ae < DECIMAL32_Emin)
    {					/* likely underflow */
      decContextDefault (&dc, DEC_INIT_DECIMAL32);
      dc.round = set->round;
      decNumberPlus (&dw, dn, &dc);	/* (round and check) */
      /* [this changes -0 to 0, so enforce the sign...] */
      dw.bits |= dn->bits & DECNEG;
      status = dc.status;
      dn = &dw;
    }

  if (dn->bits & DECSPECIAL)
    {
      if (dn->bits & DECINF)
	targ = DECIMAL_Inf << 24;
      else
	{
	  /* sNaN or qNaN */
	  if ((*dn->lsu != 0 || dn->digits > 1)	/* non-zero coefficient */
	      && (dn->digits < DECIMAL32_Pmax))
	    decDigitsToBID (dn, &targ);
	  if (dn->bits & DECNAN)
	    targ |= DECIMAL_NaN << 24;
	  else
	    targ |= DECIMAL_sNaN << 24;
	}
    }
  else
    {
      /* is finite */
      if (decNumberIsZero (dn))
	{
	  /* set and clamp exponent */
	  if (dn->exponent < -DECIMAL32_Bias)
	    {
	      exp = 0;
	      status |= DEC_Clamped;
	    }
	  else
	    {
	      exp = dn->exponent + DECIMAL32_Bias;
	      if (exp > DECIMAL32_Ehigh)
		{
		  exp = DECIMAL32_Ehigh;
		  status |= DEC_Clamped;
		}
	    }
	}
      else
	{
	  /* non-zero finite number  */
	  exp = (uInt) (dn->exponent + DECIMAL32_Bias);
	  if (exp > DECIMAL32_Ehigh)
	    {
	      exp = DECIMAL32_Ehigh;
	      status |= DEC_Clamped;
	    }
	  decDigitsToBID (dn, &targ);
        }

      /* Exponent is enconded as:
         - If coefficient fits in 23 bits:
           | sign - 1bit | exponent - 8 bits | coefficient - 23 bits |
	 - Otherwise:
           | sign - 1bit | 11 | exponent - 8 bits | coefficient - 21 bits |

	 Since decDigitsToBID will set '11' mask if coefficient does not fit
	 53 bits, we check it to adjust the exponent shift in higher word.  */
      if ((targ & BID_EXPONENT_ENC_MASK) == BID_EXPONENT_ENC_MASK)
	shift = BID_EXP_SHIFT_LARGE32;
      else
	shift = BID_EXP_SHIFT_SMALL32;

      targ |= (exp & BID_EXP_MASK32) << shift;
    }

  if (dn->bits & DECNEG)
    targ |= BID_SIGN_MASK;

  /* now write to storage; this is now always endian */
  UBFROMUIBW (d32->bytes, targ);

  if (status != 0)
    decContextSetStatus (set, status);
  /*decimal32Show(d32);*/
  return d32;
}

/* ------------------------------------------------------------------ */
/* decimal32ToNumber -- convert decimal32 to decNumber		      */
/*   d32 is the source decimal32				      */
/*   dn is the target number, with appropriate space		      */
/* No error is possible.					      */
/* ------------------------------------------------------------------ */
decNumber *
decimal32ToNumber (const decimal32 *d32, decNumber *dn)
{
  uInt uiwork;
  uInt sour;
  Int  shift;

  /* load source from storage; this is endian */
  sour = UBTOUIBW(d32->bytes);

  decNumberZero(dn);
  if (sour & BID_SIGN_MASK)
    dn->bits |= DECNEG;

  /* Infinities and NaN are encoded just like DPD:
     s 11 110 0.... 0    00 ...                 00    -> canonical infinites
     s 11 110  any                 any                -> infinites

     s 11 1110 0 .... 0         payloads              -> canonical qNaN
     s 11 1110  any                any                -> qNaN

     s 11 1111 0 .... 0         payloads              -> canonical sNaN
     s 11 1111   any               any                -> sNaN  */
  if ((sour & BID_SNAN_ENC_MASK) == BID_SNAN_ENC_MASK)
    dn->bits |= DECSNAN;
  else if ((sour & BID_NAN_ENC_MASK) == BID_NAN_ENC_MASK)
    dn->bits |= DECNAN;
  else if ((sour & BID_INF_ENC_MASK) == BID_INF_ENC_MASK)
    {
      dn->bits |= DECINF;
      return dn;		   /* no coefficient needed */
    }
  else
    {
      /* The exponent is decoded as:
         E = binary decode (bL−2 bL−3 · · · bL−w−1 ) if (bL−2 bL−3 ) != 11
                           \_ decimal32 (w=10) -> bL-2 bL-3 ...  bL-11
             binary decode (bL−4 bL−5 · · · bL−w−3 ) if (bL−2 bL−3 ) == 11
	                   \_ decimal32 (w=10) -> bl-4 bl-5 ...  bl-13

         The 0x60000000 is binary mask to check if bL-2, bL-3 are set.  */
      if ((sour & BID_EXPONENT_ENC_MASK) == BID_EXPONENT_ENC_MASK)
	shift = BID_EXP_SHIFT_LARGE32;
      else
	shift = BID_EXP_SHIFT_SMALL32;
      dn->exponent = ((sour >> shift) & BID_EXP_MASK32) - DECIMAL32_Bias;
    }

  decDigitsFromBID(dn, sour);
  /*decNumberShow (dn);*/
  return dn;
}

char *
decimal32ToString (const decimal32 *d32, char *string)
{
  decNumber dn;
  decimal32ToNumber (d32, &dn);
  decNumberToString (&dn, string);
  return string;
}

char *
decimal32ToEngString (const decimal32 *d32, char *string)
{
  decNumber dn;
  decimal32ToNumber (d32, &dn);
  decNumberToEngString (&dn, string);
  return string;
}

decimal32 *
decimal32FromString (decimal32 *result, const char *string, decContext *set)
{
  decContext dc;
  decNumber dn;

  decContextDefault (&dc, DEC_INIT_DECIMAL32);
  dc.round = set->round;

  decNumberFromString (&dn, string, &dc);
  decimal32FromNumber (result, &dn, &dc);
  if (dc.status != 0)
    decContextSetStatus (set, dc.status);
  return result;
}

/* ------------------------------------------------------------------ */
/* decDigitsToBID -- pack coefficient into BID form		      */
/*								      */
/*   dn   is the source number (assumed valid, max DECMAX754 digits)  */
/*   sour is the word in binary form                                  */
/* ------------------------------------------------------------------ */
static void
decDigitsToBID (const decNumber *dn, uInt *sour)
{
  const Unit *uin=dn->lsu;	   /* -> current output unit (uint16_t)  */
  uInt coeff = 0;
  uInt mult;
  Int digits;
  Int n;

  /* Convert the decNumber internal format to binary representation.  The
     number -2.718281, for instance, will be represented internally
     as:

     dn::lsu = [ 281, 718, 2 ]

     and to transform to a binary representation it just need to calculate:

     (281 * 1) +
     (718 * 1000) +
     (  2 * 1000000)
  */
  for (n = 0, mult = 1, digits = 1;
       digits <= dn->digits;
       ++n, mult *= 1000, digits += 3)
    {
      coeff += uin[n] * mult;
    }

  /* BID coefficient encoding is defined as:
     - If it fits on 23 bits (10*J + 3, with J=2):
     | 1bit (sign) | 8bits binary_encode(exp) | 23bits binary_encode (coeff) |

     - Otherwise:
     | 1bit (sign) | 2bits - 11 | 8bits binary_encode(exp) |
       21 bits lsbs binary_encode (coeff) |  */
  if (bid_required_bits_32 (coeff) <= 23)
    *sour = coeff & 0x007FFFFFU;
  else
    *sour = 0x60800000U | (coeff & 0x001FFFFFU);
}

/* ------------------------------------------------------------------ */
/* decDigitsFromDPD -- unpack a format's coefficient		      */
/*								      */
/*   dn is the target number, with 7, 16, or 34-digit space.	      */
/*   sour is the word in binary form                                  */
/*								      */
/* dn->digits is set, but not the sign or exponent.		      */
/* ------------------------------------------------------------------ */
static void
decDigitsFromBID (decNumber *dn, uInt sour)
{
  Int  n;
  Unit *uout = dn->lsu;		   /* -> current output unit (uint16_t)  */
  Unit *last = uout;		   /* will be unit containing msd */
  uInt bin;

  /* The BID coefficient is defined as:
     y = binary decode(b10J+2 b10J+1 ... b0 )     if (bL−2 bL−3 ) != (11)
                       \_ decimal32 (j=2): 22 ... 0
         binary decode(1 0 0 b10J b10J-1 ... b0 ) if (bL−2 bL−3 ) == (11)
                       \_ decimal32 (j=2): 20 ... 0

     And for _Decimal32:
     | 1 bit (sign) | 8 bits (exponent) |  23 bits (coefficient) |
   */

  if ((sour & BID_EXPONENT_ENC_MASK) == BID_EXPONENT_ENC_MASK)
    sour  = 0x00800000 | (sour & 0x001FFFFF);
  else
    sour &= 0x007FFFFF;

  bin = sour;
  for (n = DECNUMUNITS; (bin != 0) && (n >= 0); n--)
    {
      *uout = bin % 1000;
      bin /= 1000;
      last = uout;
      uout++;
    }

  /* 'last' points to the most significant unit with digits.  Inspect it to
     get the final digit count.  */
  dn->digits = (last-dn->lsu) * DECDPUN +1;	/* floor of digits, plus 1  */

  if (*last < 10) 				/* common odd digit or 0 */
    return;
  dn->digits++; 				/* must be 2 at least */
  if (*last < 100)				/* 10-99 */
    return;
  dn->digits++; 				/* must be 3 at least */
  if (*last < 1000)				/* 100-999 */
    return;
  dn->digits++; 				/* must be 4 at least */
  return;
}
