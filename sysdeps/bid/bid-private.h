/* Data types for BID format output.

   Copyright (C) 2014-2015 Free Software Foundation, Inc.

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

   Please see dfp/COPYING.txt for more information.  */

#ifndef _BID_PRIVATE_H
#define _BID_PRIVATE_H 1

#include <endian.h>

struct ieee754r_c_field
  {
    unsigned int is_nan:1;
    unsigned int is_inf:1;
    unsigned int lm_exp:2;
    unsigned int lmd:4;
  };


union ieee754r_Decimal32
  {
    _Decimal32 sd;
    unsigned int si;

    struct
      {
#if	__BYTE_ORDER == __BIG_ENDIAN
	unsigned int negative:1;
	unsigned int c:2;
	unsigned int els:29;
#endif				/* Big endian.  */
#if	__BYTE_ORDER == __LITTLE_ENDIAN
	unsigned int els:29;
	unsigned int c:2;
	unsigned int negative:1;
#endif				/* Little endian.  */
      } divided;

    /* This format makes it easier to see if is a NaN.  */
    struct
      {
#if	__BYTE_ORDER == __BIG_ENDIAN
	unsigned int negative:1;
	unsigned int nan:5;
	unsigned int els:26;
#endif				/* Big endian.  */
#if	__BYTE_ORDER == __LITTLE_ENDIAN
	unsigned int els:26;
	unsigned int nan:5;
	unsigned int negative:1;
#endif				/* Little endian.  */
      } nan;
  };

#define DECIMAL32_Bias 101	/* bias for the exponent  */
#define DECIMAL32_TH 10000000	/* threshold  */

union ieee754r_Decimal64
  {
    _Decimal64 dd;
    unsigned int di[2];

    struct
      {
#if	__BYTE_ORDER == __BIG_ENDIAN
	unsigned int negative:1;
	unsigned int c:2;
	unsigned int else1:29;
	unsigned int else2:32;
#endif				/* Big endian.  */
#if	__BYTE_ORDER == __LITTLE_ENDIAN
	unsigned int else2:32;
	unsigned int else1:29;
	unsigned int c:2;
	unsigned int negative:1;
#endif				/* Little endian.  */
      } divided;

    /* This format makes it easier to see if is a NaN.  */
    struct
      {
#if	__BYTE_ORDER == __BIG_ENDIAN
	unsigned int negative:1;
	unsigned int nan:5;
	unsigned int else1:26;
	unsigned int else2:32;
#endif				/* Big endian.  */
#if	__BYTE_ORDER == __LITTLE_ENDIAN
	unsigned int else2:32;
	unsigned int else1:26;
	unsigned int nan:5;
	unsigned int negative:1;
#endif				/* Little endian.  */
      } nan;
  };

#define DECIMAL64_Bias 398		/* bias for the exponent  */
#define DECIMAL64_TH 10000000000000000	/* threshold  */

union ieee754r_Decimal128
  {
    _Decimal128 td;
    unsigned int ti[4];

    struct
      {
#if	__BYTE_ORDER == __BIG_ENDIAN
	unsigned int negative:1;
	unsigned int c:2;
	unsigned int else1:29;
	unsigned int else2:32;
	unsigned int else3:32;
	unsigned int else4:32;
#endif				/* Big endian.  */
#if	__BYTE_ORDER == __LITTLE_ENDIAN
	unsigned int else4:32;
	unsigned int else3:32;
	unsigned int else2:32;
	unsigned int else1:29;
	unsigned int c:2;
	unsigned int negative:1;
#endif				/* Little endian.  */
      } divided;

    /* This format makes it easier to see if is a NaN.  */
    struct
      {
#if	__BYTE_ORDER == __BIG_ENDIAN
	unsigned int negative:1;
	unsigned int nan:5;
	unsigned int else1:26;
	unsigned int else2:32;
	unsigned int else3:32;
	unsigned int else4:32;
#endif				/* Big endian.  */
#if	__BYTE_ORDER == __LITTLE_ENDIAN
	unsigned int else4:32;
	unsigned int else3:32;
	unsigned int else2:32;
	unsigned int else1:26;
	unsigned int nan:5;
	unsigned int negative:1;
#endif				/* Little endian.  */
      } nan;
  };

#define DECIMAL128_Bias 6176		/* bias for the exponent  */
#define DECIMAL128_TH 1000000000000000	/* threshold (10³⁴ = 10¹⁵ x 10¹⁹) */

#endif /* _BID_PRIVATE */
