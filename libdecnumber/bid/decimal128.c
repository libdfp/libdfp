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

#include "dconfig.h"		/* GCC definitions */
#define DECNUMDIGITS 34		/* make decNumbers with space for 16 */
#include "decNumber.h"		/* base number library */
#define DECUSE128 1
#include "decNumberLocal.h"	/* decNumber local types, etc. */
#include "decimal128.h"		/* our primary include */
#include "bid-coeffbits.h"

#if DECDPUN != 3
# error "_Decimal128 BID implementation only support DECDPUN==3"
#endif

decimal128 *decimal128FromString (decimal128 *, const char *, decContext *);
char *decimal128ToString (const decimal128 *, char *);
char *decimal128ToEngString (const decimal128 *, char *);
decimal128 *decimal128FromNumber (decimal128 *, const decNumber *, decContext *);
decNumber *decimal128ToNumber (const decimal128 *, decNumber *);

static void decDigitsFromBID (decNumber *dn, uInt sourhi, uInt sourmh,
			      uInt sourml, uInt sourlo);
static void decDigitsToBID (const decNumber *dn, uInt *sourhi, uInt *sourmh,
			    uInt *sourml, uInt *sourlo);

/* ------------------------------------------------------------------ */
/* BID special encoding definitions.                                  */
/* ------------------------------------------------------------------ */

#define BID_SIGN_MASK          0x80000000
#define BID_INF_ENC_MASK       0x78000000
#define BID_NAN_ENC_MASK       0x7C000000
#define BID_SNAN_ENC_MASK      0x7E000000
#define BID_EXPONENT_ENC_MASK  0x60000000
#define BID_EXP_MASK128        0x3fff
#define BID_EXP_SHIFT_LARGE128 15
#define BID_EXP_SHIFT_SMALL128 17

/* ------------------------------------------------------------------ */
/* Decimal 128-bit format module 				      */
/* ------------------------------------------------------------------ */
/* This module comprises the routines for decimal128 format numbers.  */
/* Conversions are supplied to and from decNumber and String.	      */
/*								      */
/* This is used when decNumber provides operations, either for all    */
/* operations or as a proxy between decNumber and decSingle.	      */
/*								      */
/* Error handling is the same as decNumber (qv.).		      */
/* ------------------------------------------------------------------ */
decimal128 *
decimal128FromNumber (decimal128 *d128, const decNumber *dn, decContext *set)
{
  uInt status = 0;
  Int ae;			/* adjusted exponent */
  decNumber dw;
  decContext dc;
  uInt exp;
  uInt uiwork;
  uInt targhi = 0;
  uInt targmh = 0;
  uInt targml = 0;
  uInt targlo = 0;
  Int  shift;

  /* If the number has too many digits, or the exponent could be
     out of range then reduce the number under the appropriate
     constraints.  This could push the number to Infinity or zero,
     so this check and rounding must be done before generating the
     decimal128.  */
  ae = dn->exponent + dn->digits - 1;	/* [0 if special] */
  if (dn->digits > DECIMAL128_Pmax	/* too many digits */
      || ae > DECIMAL128_Emax		/* likely overflow */
      || ae < DECIMAL128_Emin)
    {					/* likely underflow */
      decContextDefault (&dc, DEC_INIT_DECIMAL128);
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
	targhi = DECIMAL_Inf << 24;
      else
	{
	  /* sNaN or qNaN */
	  if ((*dn->lsu != 0 || dn->digits > 1)	/* non-zero coefficient */
	      && (dn->digits < DECIMAL128_Pmax))
	    decDigitsToBID (dn, &targhi, &targmh, &targml, &targlo);
	  if (dn->bits & DECNAN)
	    targhi |= DECIMAL_NaN << 24;
	  else
	    targhi |= DECIMAL_sNaN << 24;
	}
    }
  else
    {
      /* is finite */
      if (decNumberIsZero (dn))
	{
	  /* set and clamp exponent */
	  if (dn->exponent < -DECIMAL128_Bias)
	    {
	      exp = 0;
	      status |= DEC_Clamped;
	    }
	  else
	    {
	      exp = dn->exponent + DECIMAL128_Bias;
	      if (exp > DECIMAL128_Ehigh)
		{
		  exp = DECIMAL128_Ehigh;
		  status |= DEC_Clamped;
		}
	    }
	}
      else
	{
	  /* non-zero finite number  */
	  exp = (uInt) (dn->exponent + DECIMAL128_Bias);	
	  if (exp > DECIMAL128_Ehigh)
	    {
	      exp = DECIMAL128_Ehigh;
	      status |= DEC_Clamped;
	    }
	  decDigitsToBID (dn, &targhi, &targmh, &targml, &targlo);
        }

      /* Exponent is enconded as:
         - If coefficient fits in 113 bits:
           | sign - 1bit | exponent - 14 bits | coefficient - 113 bits |
	 - Otherwise
           | sign - 1bit | 11 | exponent - 14 bits | coefficient - 111 bits |

	 Since decDigitsToBID will set the '11' mask if coefficient does not
	 fit 113 bits, we check it to adjust the exponent shift in higher
	 word.  */
      if ((targhi & BID_EXPONENT_ENC_MASK) == BID_EXPONENT_ENC_MASK)
	shift = BID_EXP_SHIFT_LARGE128;
      else
	shift = BID_EXP_SHIFT_SMALL128;

      targhi |= (exp & BID_EXP_MASK128) << shift;
    }

  if (dn->bits & DECNEG)
    targhi |= BID_SIGN_MASK;

  /* now write to storage; this is now always endian */
  UBFROMUIBW(d128->bytes,    targhi);
  UBFROMUIBW(d128->bytes+4,  targmh);
  UBFROMUIBW(d128->bytes+8,  targml);
  UBFROMUIBW(d128->bytes+12, targlo);

  if (status != 0)
    decContextSetStatus (set, status);
  /*decimal128Show(d128);*/
  return d128;
}

/* ------------------------------------------------------------------ */
/* decimal128ToNumber -- convert decimal128 to decNumber	      */
/*   d128 is the source decimal128				      */
/*   dn is the target number, with appropriate space		      */
/* ------------------------------------------------------------------ */
decNumber *
decimal128ToNumber (const decimal128 *d128, decNumber *dn)
{
  uInt uiwork;
  uInt sourhi;
  uInt sourmh;
  uInt sourml;
  uInt sourlo;
  Int  shift;

  /* load source from storage; this is endian */
  sourhi = UBTOUIBW(d128->bytes  );
  sourmh = UBTOUIBW(d128->bytes+4);
  sourml = UBTOUIBW(d128->bytes+8);
  sourlo = UBTOUIBW(d128->bytes+12);

  decNumberZero(dn);
  if (sourhi & BID_SIGN_MASK)
    dn->bits |= DECNEG;

  /* Infinities and NaN are encoded just like DPD:
     s 11 110 0.... 0    00 ...                 00    -> canonical infinites
     s 11 110  any                 any                -> infinites

     s 11 1110 0 .... 0         payloads              -> canonical qNaN
     s 11 1110  any                any                -> qNaN

     s 11 1111 0 .... 0         payloads              -> canonical sNaN
     s 11 1111   any               any                -> sNaN  */
  if ((sourhi & BID_SNAN_ENC_MASK) == BID_SNAN_ENC_MASK)
    dn->bits |= DECSNAN;
  else if ((sourhi & BID_NAN_ENC_MASK) == BID_NAN_ENC_MASK)
    dn->bits |= DECNAN;
  else if ((sourhi & BID_INF_ENC_MASK) == BID_INF_ENC_MASK)
    {
      dn->bits |= DECINF;
      return dn;		   /* no coefficient needed */
    }
  else
    {
      /* The exponent is decoded as:
         E = binary decode (bL−2 bL−3 · · · bL−w−1 ) if (bL−2 bL−3 ) != 11
                           \_ decimal128 (w=14) -> bL-2 bL-3 ...  bL-11
             binary decode (bL−4 bL−5 · · · bL−w−3 ) if (bL−2 bL−3 ) == 11
	                   \_ decimal128 (w=14) -> bl-4 bl-5 ...  bl-13

         The 0x60000000 is binary mask to check if bL-2, bL-3 are set.  */
      if ((sourhi & BID_EXPONENT_ENC_MASK) == BID_EXPONENT_ENC_MASK)
	shift = BID_EXP_SHIFT_LARGE128;
      else
	shift = BID_EXP_SHIFT_SMALL128;
      dn->exponent = ((sourhi >> shift) & BID_EXP_MASK128) - DECIMAL128_Bias;
    }

  decDigitsFromBID(dn, sourhi, sourmh, sourml, sourlo);
  /*decNumberShow (dn);*/
  return dn;
}

/* ------------------------------------------------------------------ */
/* to-scientific-string -- conversion to numeric string 	      */
/* to-engineering-string -- conversion to numeric string	      */
/*								      */
/*   decimal128ToString(d128, string);				      */
/*   decimal128ToEngString(d128, string); 			      */
/*								      */
/*  d128 is the decimal128 format number to convert		      */
/*  string is the string where the result will be laid out	      */
/*								      */
/*  string must be at least 24 characters			      */
/*								      */
/*  No error is possible, and no status can be set.		      */
/* ------------------------------------------------------------------ */
char *
decimal128ToString (const decimal128 *d128, char *string)
{
  decNumber dn;
  decimal128ToNumber (d128, &dn);
  decNumberToString (&dn, string);
  return string;
}

char *
decimal128ToEngString (const decimal128 *d128, char *string)
{
  decNumber dn;
  decimal128ToNumber (d128, &dn);
  decNumberToEngString (&dn, string);
  return string;
}

/* ------------------------------------------------------------------ */
/* to-number -- conversion from numeric string			      */
/*								      */
/*   decimal128FromString(result, string, set);			      */
/*								      */
/*  result  is the decimal128 format number which gets the result of  */
/*	    the conversion					      */
/*  *string is the character string which should contain a valid      */
/*	    number (which may be a special value)		      */
/*  set     is the context					      */
/*								      */
/* The context is supplied to this routine is used for error handling */
/* (setting of status and traps) and for the rounding mode, only.     */
/* If an error occurs, the result will be a valid decimal128 NaN.     */
/* ------------------------------------------------------------------ */
decimal128 *
decimal128FromString (decimal128 *result, const char *string, decContext *set)
{
  decContext dc;
  decNumber dn;

  decContextDefault (&dc, DEC_INIT_DECIMAL128);
  dc.round = set->round;

  decNumberFromString (&dn, string, &dc);
  decimal128FromNumber (result, &dn, &dc);
  if (dc.status != 0)
    decContextSetStatus (set, dc.status);
  return result;
}

/* ------------------------------------------------------------------ */
/* decDigitsToBID -- pack coefficient into BID form		      */
/*								      */
/*   dn   is the source number (assumed valid, max DECMAX754 digits)  */
/*   sourhi is the higher word in binary form                         */
/*   sourlo is the lower word in binary form                          */
/* ------------------------------------------------------------------ */
static void
decDigitsToBID (const decNumber *dn, uInt *sourhi, uInt *sourmh,
		uInt *sourml, uInt *sourlo)
{
  const Unit *uin=dn->lsu;	   /* -> current output unit (uint16_t)  */
  Quad coeff = 0;
  Quad mult;
  Int digits;
  Int n;

  /* Convert the decNumber internal format to binary representation.  The
     number -2.718281828459045, for instance, will be represented internally
     as:

     dn::lsu = [ 45, 459, 828, 281, 718, 2 ]

     and to transform to a binary representation it just need to calculate:

     ( 45 * 1) +
     (459 * 1000) +
     (828 * 1000000) +
     (281 * 1000000000) +
     (718 * 1000000000000) +
     (  2 * 1000000000000000)
  */
  for (n = 0, mult = 1, digits = 1;
       digits <= dn->digits;
       ++n, mult *= 1000, digits += 3)
    {
      coeff += uin[n] * mult;
    }

  /* BID coefficient encoding is defined as:
     - If it fits on 113 bits (10*J + 3, with J=11):
     | 1bit (sign) | 14bits binary_encode(exp) | 113bits binary_encode (coeff) |

     - Otherwise:
     | 1bit (sign) | 2bits - 11 | 14bits binary_encode(exp) |
       113 bits lsbs binary_encode (coeff) |  */
  if (bid_required_bits_128 (coeff) <= 133)
    *sourhi = (coeff >> 96) & 0x0001FFFF;
  else
    *sourhi = (coeff >> 96) & 0x60007FFF;
  *sourmh = (coeff >> 64) & 0xFFFFFFFFUL;
  *sourml = (coeff >> 32) & 0xFFFFFFFFUL;
  *sourlo = coeff & 0xFFFFFFFFUL;
}

/* ------------------------------------------------------------------ */
/* decDigitsFromDPD -- unpack a format's coefficient		      */
/*								      */
/*   dn is the target number, with 7, 16, or 34-digit space.	      */
/*   sourhi is the higher word in binary form                         */
/*   sourlo is the lower word in binary form                          */
/*								      */
/* dn->digits is set, but not the sign or exponent.		      */
/* ------------------------------------------------------------------ */
static void
decDigitsFromBID (decNumber *dn, uInt sourhi, uInt sourmh, uInt sourml,
		  uInt sourlo)
{
  Int  n;
  Unit *uout = dn->lsu;		   /* -> current output unit (uint16_t)  */
  Unit *last = uout;		   /* will be unit containing msd */
  uQuad bin;

  /* The BID coefficient is defined as:
     y = binary decode(b10J+2 b10J+1 ... b0 )     if (bL−2 bL−3 ) != (11)
                       \_ decimal128 (j=11): 112 ... 0
         binary decode(1 0 0 b10J b10J-1 ... b0 ) if (bL−2 bL−3 ) == (11)
                       \_ decimal128 (j=11): 110 ... 0

     And for _Decimal128:
     | 1 bit (sign) | 14 bits (exponent) |  113 bits (coefficient) |
   */

  if ((sourhi & BID_EXPONENT_ENC_MASK) == BID_EXPONENT_ENC_MASK)
    sourhi  = 0x00020000 | (sourhi & 0x00007FFF);
  else
    sourhi &= 0x0001FFFF;

  bin = ((uQuad)sourhi << 96) |
	((uQuad)sourmh << 64) |
        ((uQuad)sourml << 32) |
	 (uQuad)sourlo;
  for (n = DECNUMUNITS; (bin != 0) && (n >= 0); n--)
    {
      *uout = bin % 1000;
      bin /= 1000;
      last = uout;
      uout++;
    }

  /* 'last' points to the most significant unit with digits.  Inspect it to
     get the final digit count.  */
  dn->digits = (last-dn->lsu) * DECDPUN + 1;	/* floor of digits, plus 1  */

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
