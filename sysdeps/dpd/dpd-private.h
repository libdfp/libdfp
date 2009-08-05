/* Data types for DPD format output.

   Copyright (C) 2006, 2007 IBM Corporation.
   Copyright (C) 2007, 2009 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Steve Munroe <sjmunroe@us.ibm.com>

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

#ifndef _DPD_PRIVATE_H
#define _DPD_PRIVATE_H 1

#include <endian.h>

/* copy the declet in the 'declet' param to the string in the 'str' param.  */
static inline void __dpd_cpydeclet(char * str, const char * declet)
{
    str[0] = declet[0];
    str[1] = declet[1];
    str[2] = declet[2];
}

/* Basically itoa  */
static inline char* __dpd_exptostr(char *str, int val)
{
    int digit;
    if (val < 0)
    {
        *str++ = '-';
        val = -val;
    } else {
        *str++ = '+';
    }

    if (val > 999)
    {
        digit = val / 1000;
        val %= 1000;
        *str++ = '0' + digit;
        digit = val / 100;
        val %= 100;
        *str++ = '0' + digit;
        digit = val / 10;
        val %= 10;
        *str++ = '0' + digit;
    } else if (val > 99)
    {
        digit = val / 100;
        val %= 100;
        *str++ = '0' + digit;
        digit = val / 10;
        val %= 10;
        *str++ = '0' + digit;
    } else if (val > 9)
    {
        digit = val / 10;
        val %= 10;
        *str++ = '0' + digit;
    }
    *str++ = '0' + val;

    return str;
}

union ieee754r_Decimal32
  {
    _Decimal32 sd;
    unsigned int si;

    /* This is the IEEE754r single-precision format.  */
    struct
      {
#if	__BYTE_ORDER == __BIG_ENDIAN
	unsigned int negative:1;
	unsigned int c:5;
	unsigned int bec:6;
	unsigned int cc0:10;
	unsigned int cc1:10;
#endif				/* Big endian.  */
#if	__BYTE_ORDER == __LITTLE_ENDIAN
	unsigned int cc1:10;
	unsigned int cc0:10;
	unsigned int bec:6;
	unsigned int c:5;
	unsigned int negative:1;
#endif				/* Little endian.  */
      } ieee;

    /* This format makes it easier to see if a NaN is a signalling NaN.  */
    struct
      {
#if	__BYTE_ORDER == __BIG_ENDIAN
	unsigned int negative:1;
	unsigned int c:5;
	unsigned int signaling_nan:1;
	unsigned int bec:5;
	unsigned int cc0:10;
	unsigned int cc1:10;
#endif				/* Big endian.  */
#if	__BYTE_ORDER == __LITTLE_ENDIAN
	unsigned int cc1:10;
	unsigned int cc0:10;
	unsigned int bec:5;
	unsigned int signaling_nan:1;
	unsigned int c:5;
	unsigned int negative:1;
#endif				/* Little endian.  */
      } ieee_nan;
  };

#define DECIMAL32_Bias 101	/* bias for the exponent  */
#define DECIMAL32_BEC_bits 6	/* Bits in BEC  */
#define DECIMAL32_CC_bits 20	/* Bits in CC  */

union ieee754r_Decimal64
  {
    _Decimal64 dd;
    unsigned int di[2];

    /* This is the IEEE754r _Decimal64 format.  */
    struct
      {
#if	__BYTE_ORDER == __BIG_ENDIAN
	unsigned int negative:1;
	unsigned int c:5;
	unsigned int bec:8;
	unsigned int cc0:10;
	unsigned int cc1H8:8;
	unsigned int cc1L2:2;
	unsigned int cc2:10;
	unsigned int cc3:10;
	unsigned int cc4:10;
#endif				/* Big endian.  */
#if	__BYTE_ORDER == __LITTLE_ENDIAN
	unsigned int cc4:10;
	unsigned int cc3:10;
	unsigned int cc2:10;
	unsigned int cc1L2:2;
	unsigned int cc1H8:8;
	unsigned int cc0:10;
	unsigned int bec:8;
	unsigned int c:5;
	unsigned int negative:1;
#endif				/* Little endian.  */
      } ieee;

    /* This format makes it easier to see if a NaN is a signalling NaN.  */
    struct
      {
#if	__BYTE_ORDER == __BIG_ENDIAN
	unsigned int negative:1;
	unsigned int c:5;
	unsigned int signaling_nan:1;
	unsigned int bec:7;
	unsigned int cc0:10;
	unsigned int cc1H8:8;
	unsigned int cc1L2:2;
	unsigned int cc2:10;
	unsigned int cc3:10;
	unsigned int cc4:10;
#endif				/* Big endian.  */
#if	__BYTE_ORDER == __LITTLE_ENDIAN
	unsigned int cc4:10;
	unsigned int cc3:10;
	unsigned int cc2:10;
	unsigned int cc1L2:2;
	unsigned int cc1H8:8;
	unsigned int cc0:10;
	unsigned int bec:7;
	unsigned int signaling_nan:1;
	unsigned int c:5;
	unsigned int negative:1;
#endif				/* Little endian.  */
      } ieee_nan;
  };

#define DECIMAL64_Bias 398	/* bias for the exponent  */
#define DECIMAL64_BEC_bits 8	/* Bits in BEC  */
#define DECIMAL64_CC_bits 50	/* Bits in CC  */

union ieee754r_Decimal128
  {
    _Decimal128 td;
    unsigned int ti[4];

    /* This is the IEEE754r _Decimal128 format.  */
    struct
      {
#if	__BYTE_ORDER == __BIG_ENDIAN
	unsigned int negative:1;
	unsigned int c:5;
	unsigned int bec:12;
	unsigned int cc0:10;
	unsigned int cc1H4:4;
	unsigned int cc1L6:6;
	unsigned int cc2:10;
	unsigned int cc3:10;
	unsigned int cc4H6:6;
	unsigned int cc4L4:4;
	unsigned int cc5:10;
	unsigned int cc6:10;
	unsigned int cc7H8:8;
	unsigned int cc7L2:2;
	unsigned int cc8:10;
	unsigned int cc9:10;
	unsigned int cc10:10;
#endif				/* Big endian.  */
#if	__BYTE_ORDER == __LITTLE_ENDIAN
	unsigned int cc10:10;
	unsigned int cc9:10;
	unsigned int cc8:10;
	unsigned int cc7L2:2;
	unsigned int cc7H8:8;
	unsigned int cc6:10;
	unsigned int cc5:10;
	unsigned int cc4L4:4;
	unsigned int cc4H6:6;
	unsigned int cc3:10;
	unsigned int cc2:10;
	unsigned int cc1L6:6;
	unsigned int cc1H4:4;
	unsigned int cc0:10;
	unsigned int bec:12;
	unsigned int c:5;
	unsigned int negative:1;
#endif				/* Little endian.  */
      } ieee;

    /* This format makes it easier to see if a NaN is a signalling NaN.  */
    struct
      {
#if	__BYTE_ORDER == __BIG_ENDIAN
	unsigned int negative:1;
	unsigned int c:5;
	unsigned int signaling_nan:1;
	unsigned int bec:11;
	unsigned int cc0:10;
	unsigned int cc1H4:4;
	unsigned int cc1L6:6;
	unsigned int cc2:10;
	unsigned int cc3:10;
	unsigned int cc4H6:6;
	unsigned int cc4L4:4;
	unsigned int cc5:10;
	unsigned int cc6:10;
	unsigned int cc7H8:8;
	unsigned int cc7L2:2;
	unsigned int cc8:10;
	unsigned int cc9:10;
	unsigned int cc10:10;
#endif				/* Big endian.  */
#if	__BYTE_ORDER == __LITTLE_ENDIAN
	unsigned int cc10:10;
	unsigned int cc9:10;
	unsigned int cc8:10;
	unsigned int cc7L2:2;
	unsigned int cc7H8:8;
	unsigned int cc6:10;
	unsigned int cc5:10;
	unsigned int cc4L4:4;
	unsigned int cc4H6:6;
	unsigned int cc3:10;
	unsigned int cc2:10;
	unsigned int cc1L6:6;
	unsigned int cc1H4:4;
	unsigned int cc0:10;
	unsigned int bec:11;
	unsigned int signaling_nan:1;
	unsigned int c:5;
	unsigned int negative:1;
#endif				/* Little endian.  */
      } ieee_nan;
  };

#define DECIMAL128_Bias   6176		/* bias for the exponent  */
#define DECIMAL128_BEC_bits   12	/* Bits in BEC  */
#define DECIMAL128_CC_bits   110	/* Bits in CC  */

struct ieee754r_c_field
  {
    unsigned int is_nan:1;
    unsigned int is_inf:1;
    unsigned int lm_exp:2;
    unsigned int lmd:4;
  };

extern const struct ieee754r_c_field c_decoder[32];
extern const unsigned char lm2lmd_to_c[10][3];
extern const char	dpd_to_char[1024][4];
extern const const short int	dpd_to_bcd[1024];
extern const short int	bcd_to_dpd[2464];

#endif /* _DPD_PRIVATE */
