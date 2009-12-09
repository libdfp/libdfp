/* Number of digits functions.

   Copyright (C) 2006, 2007, 2008 IBM Corporation.
   Copyright (C) 2007, 2008, 2009 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Pete Eberlein <eberlein@us.ibm.com>

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


#ifndef _NUMDIGITS_H
#define _NUMDIGITS_H 1

#define NUMDIGITS_SUPPORT 1

#ifndef DEC_TYPE
#error DEC_TYPE must be declared
#endif

#ifndef _DECIMAL_SIZE
#error _DECIMAL_SIZE must be declared
#endif

#include "dpd-private.h"
#include <string.h>
#include "get_digits.h"

#ifndef PASTE
# define PASTE(x,y) PASTE2(x,y)
# define PASTE2(x,y) x##y
#endif

#ifndef FUNC_D
# define FUNC_D(x)		PASTE(x,PASTE(d,_DECIMAL_SIZE))
#endif

static inline int
FUNC_D (getexp) (DEC_TYPE x)
{
  int exp;
  struct ieee754r_c_field c_f;

#if _DECIMAL_SIZE == 32
  union ieee754r_Decimal32 d;
  d.sd = x;
  c_f =  c_decoder[d.ieee.c];
  exp = c_f.lm_exp << DECIMAL32_BEC_bits;
  exp += d.ieee.bec;
  exp -= DECIMAL32_Bias;

#elif _DECIMAL_SIZE == 64
  union ieee754r_Decimal64 d;
  d.dd = x;
  c_f =  c_decoder[d.ieee.c];
  exp = c_f.lm_exp << DECIMAL64_BEC_bits;
  exp += d.ieee.bec;
  exp -= DECIMAL64_Bias;

#elif _DECIMAL_SIZE == 128
  union ieee754r_Decimal128 d;
  d.td = x;
  c_f =  c_decoder[d.ieee.c];
  exp = c_f.lm_exp << DECIMAL128_BEC_bits;
  exp += d.ieee.bec;
  exp -= DECIMAL128_Bias;

#endif

  return exp;
}
#define SETEXP_NAME	PASTE(setexp,PASTE(d,_DECIMAL_SIZE))

static inline DEC_TYPE
SETEXP_NAME (DEC_TYPE x, int exp)
{
#if _DECIMAL_SIZE == 32
  union ieee754r_Decimal32 d;
  d.sd = x;
  exp += DECIMAL32_Bias;
  d.ieee.bec = exp;
  d.ieee.c = lm2lmd_to_c[c_decoder[d.ieee.c].lmd][exp >> DECIMAL32_BEC_bits];
  return d.sd;

#elif _DECIMAL_SIZE == 64
  union ieee754r_Decimal64 d;
  d.dd = x;
  exp += DECIMAL64_Bias;
  d.ieee.bec = exp;
  d.ieee.c = lm2lmd_to_c[c_decoder[d.ieee.c].lmd][exp >> DECIMAL64_BEC_bits];
  return d.dd;

#elif _DECIMAL_SIZE == 128
  union ieee754r_Decimal128 d;
  d.td = x;
  exp += DECIMAL128_Bias;
  d.ieee.bec = exp;
  d.ieee.c = lm2lmd_to_c[c_decoder[d.ieee.c].lmd][exp >> DECIMAL128_BEC_bits];
  return d.td;

#endif

}

static inline unsigned int
__dfp_declet_to_dpd(char *str) 
{
  return bcd_to_dpd[(str[0]<<8) + (str[1]<<4) + str[2] - '0'*0x111];
}

static inline DEC_TYPE
setdigits (DEC_TYPE x, char *str)
{
#if _DECIMAL_SIZE == 32
  union ieee754r_Decimal32 d;
  d.sd = x;
  d.ieee.c = lm2lmd_to_c[str[0]-'0'][c_decoder[d.ieee.c].lm_exp];

  d.ieee.cc0 = __dfp_declet_to_dpd(str+1);
  d.ieee.cc1 = __dfp_declet_to_dpd(str+4);

  return d.sd;

#elif _DECIMAL_SIZE == 64
  unsigned int bcd;
  union ieee754r_Decimal64 d;
  d.dd = x;
  d.ieee.c = lm2lmd_to_c[str[0]-'0'][c_decoder[d.ieee.c].lm_exp];

  d.ieee.cc0 = __dfp_declet_to_dpd(str+1);
  /* Packed fields crossing a word boundary require special handling. */
  bcd = __dfp_declet_to_dpd(str+4);
  d.ieee.cc1H8 = bcd>>2;
  d.ieee.cc1L2 = bcd;
  d.ieee.cc2 = __dfp_declet_to_dpd(str+7);
  d.ieee.cc3 = __dfp_declet_to_dpd(str+10);
  d.ieee.cc4 = __dfp_declet_to_dpd(str+13);

  return d.dd;

#elif _DECIMAL_SIZE == 128
  unsigned int bcd;
  union ieee754r_Decimal128 d;
  d.td = x;
  d.ieee.c = lm2lmd_to_c[str[0]-'0'][c_decoder[d.ieee.c].lm_exp];

  d.ieee.cc0 = __dfp_declet_to_dpd(str+1);
  /* Packed fields crossing a word boundary require special handling. */
  bcd = __dfp_declet_to_dpd(str+4);
  d.ieee.cc1H4 = bcd>>6;
  d.ieee.cc1L6 = bcd;
  d.ieee.cc2 = __dfp_declet_to_dpd(str+7);
  d.ieee.cc3 = __dfp_declet_to_dpd(str+10);
  bcd = __dfp_declet_to_dpd(str+13);
  d.ieee.cc4H6 = bcd>>4;
  d.ieee.cc4L4 = bcd;
  d.ieee.cc5 = __dfp_declet_to_dpd(str+16);
  d.ieee.cc6 = __dfp_declet_to_dpd(str+19);
  bcd = __dfp_declet_to_dpd(str+22);
  d.ieee.cc7H8 = bcd>>2;
  d.ieee.cc7L2 = bcd;
  d.ieee.cc8 = __dfp_declet_to_dpd(str+25);
  d.ieee.cc9 = __dfp_declet_to_dpd(str+28);
  d.ieee.cc10 = __dfp_declet_to_dpd(str+31);

  return d.td;

#endif
}

static inline int
FUNC_D (numdigits) (DEC_TYPE x)
{
  int firstdigit = 0;
#if _DECIMAL_SIZE == 32
  char digits[8];
  __get_digits_d32(x, digits, NULL, NULL, NULL, NULL);
#elif _DECIMAL_SIZE == 64
  char digits[17];
  __get_digits_d64(x, digits, NULL, NULL, NULL, NULL);
#elif _DECIMAL_SIZE == 128
  char digits[35];
  __get_digits_d128(x, digits, NULL, NULL, NULL, NULL);
#endif
  while (digits[firstdigit] == '0') firstdigit++;

  return strlen(digits + firstdigit);
}

static inline DEC_TYPE
left_justify (DEC_TYPE x)
{
  int firstdigit = 0, len;
#if _DECIMAL_SIZE == 32
  char digits[8+7];
  __get_digits_d32(x, digits, NULL, NULL, NULL, NULL);
#elif _DECIMAL_SIZE == 64
  char digits[17+16];
  __get_digits_d64(x, digits, NULL, NULL, NULL, NULL);
#elif _DECIMAL_SIZE == 128
  char digits[35+34];
  __get_digits_d128(x, digits, NULL, NULL, NULL, NULL);
#endif
  while (digits[firstdigit] == '0') firstdigit++;
  len = strlen(digits + firstdigit);
  if (len)
    {
      /* pad the significant digits with enough trailing zeroes */
      memset(digits + firstdigit + len, '0', firstdigit);
      x = setdigits(x, digits + firstdigit);
      x = FUNC_D(setexp) (x, FUNC_D (getexp) (x) - firstdigit);
    }

  return x;
}


#endif /* _NUMDIGITS_H */
