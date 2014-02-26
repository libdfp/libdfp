/* Common definition for libdfp tests.

   Copyright (C) 2014 Free Software Foundation, Inc.

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

#ifndef _TEST_COMMON_H
# define _TEST_COMMON_H

/* Based on 'sysdeps/dpd/dpd-private.h' without the DPD specific fields.  */
union ieee754r_Decimal32
{
  _Decimal32 sd;

  struct
  {
#if __BYTE_ORDER == __BIG_ENDIAN
    unsigned int negative:1;
    unsigned int c:5;
    unsigned int signaling_nan:1;
    unsigned int unused0:25;
#else				/* __LITTLE_ENDIAN  */
    unsigned int unused0:25;
    unsigned int signaling_nan:1;
    unsigned int c:5;
    unsigned int negative:1;
#endif
  } ieee_nan;
};

union ieee754r_Decimal64
{
  _Decimal64 dd;

  struct
  {
#if __BYTE_ORDER == __BIG_ENDIAN
    unsigned int negative:1;
    unsigned int c:5;
    unsigned int signaling_nan:1;
    unsigned int unused0:25;
    unsigned int unused1:32;
#else				/* __LITTLE_ENDIAN  */
    unsigned int unused1:32;
    unsigned int unused0:25;
    unsigned int signaling_nan:1;
    unsigned int c:5;
    unsigned int negative:1;
#endif
  } ieee_nan;
};

union ieee754r_Decimal128
{
  _Decimal128 td;
  struct
  {
#if __BYTE_ORDER == __BIG_ENDIAN
    unsigned int negative:1;
    unsigned int c:5;
    unsigned int signaling_nan:1;
    unsigned int unused0:25;
    unsigned int unused1:32;
    unsigned int unused2:32;
    unsigned int unused3:32;
#endif				/* Big endian.  */
#if	__BYTE_ORDER == __LITTLE_ENDIAN
    unsigned int unused3:32;
    unsigned int unused2:32;
    unsigned int unused1:32;
    unsigned int unused0:25;
    unsigned int signaling_nan:1;
    unsigned int c:5;
    unsigned int negative:1;
#endif				/* Little endian.  */
  } ieee_nan;
};

#endif
