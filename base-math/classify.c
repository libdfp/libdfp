/* Classification functions for binary floating point types

   Copyright (C) 2007, 2008 IBM Corporation.
   Copyright (C) 2008, 2009 Free Software Foundation, Inc.

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

   Please see libdfp/COPYING.txt for more information.  */

#include <math.h>
#include "convert.h"

extern int __dfp_classify_sf (float a);

int __dfp_classify_sf (float a)
{
  union {
    float x;
    struct {
      unsigned int sign:1;
      unsigned int exp:8;
      unsigned int sig:23;
    } e;
  } u;
  u.x = a;
  if (__builtin_expect (a == 0.0f, 0))
    return FP_ZERO;
  
  if (__builtin_expect (u.e.exp == 0xff, 0))
    {
      if (u.e.sig == 0)
	return FP_INFINITE;
      else
	return FP_NAN;
    }
  return FP_NORMAL;
}

extern int __dfp_classify_df (double a);

int __dfp_classify_df (double a)
{
  union {
    double x;
    struct {
      unsigned int sign:1;
      unsigned int exp:11;
      unsigned int sig0:20;
      unsigned int sig1:32;
    } e;
  } u;
  u.x = a;
  if (__builtin_expect (a == 0.0, 0))
    return FP_ZERO;
  
  if (__builtin_expect (u.e.exp == 0x7ff, 0))
    {
      if (u.e.sig0 == 0 && u.e.sig1 == 0)
	return FP_INFINITE;
      else
	return FP_NAN;
    }
  return FP_NORMAL;
}
