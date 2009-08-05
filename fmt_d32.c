/* Macroized version of the decimal floating point string formatting functions.
   Copyright (C) 2007 IBM Corporation.
   Copyright (C) 2007, 2009 Free Software Foundation.

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

   Please see libdfp/COPYING.txt for more information.  */

#ifndef _DECIMAL_SIZE
# define _DECIMAL_SIZE 32
   /* needed to pick up DECNUMDIGITS before including decNumber.h  */
# include <decimal32.h>
# include <dfptypeconv32.h>
#endif

#include <fmt_dfp.h>

#define FUNCTION_NAME fmt_

#include <dfpmacro.h>

#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* DECIMAL128_Pmax + 14 (where '14' represents all the extra characters present
 * in the output string).  Defined here so we avoid including <decimal128.h>.  */
#define MAX_DECIMAL128_STRING 48

/* Thirty-five array indices will account for the available number of digits of
 * precision for a _Decimal128.  */
static _Decimal128 powof10 [] = 
   { 1.e0dl, 1.e1dl, 1.e2dl, 1.e3dl,
     1.e4dl, 1.e5dl, 1.e6dl, 1.e7dl,
     1.e8dl, 1.e9dl, 1.e10dl, 1.e11dl,
     1.e12dl, 1.e13dl, 1.e14dl, 1.e15dl,
     1.e16dl, 1.e17dl, 1.e18dl, 1.e19dl,
     1.e20dl, 1.e21dl, 1.e22dl, 1.e23dl,
     1.e24dl, 1.e25dl, 1.e26dl, 1.e27dl,
     1.e28dl, 1.e29dl, 1.e30dl, 1.e31dl,
     1.e32dl, 1.e33dl, 1.e34dl };

static _Decimal128 negpowof10 [] = 
   { 1.e0dl, 1.e-1dl, 1.e-2dl, 1.e-3dl,
     1.e-4dl, 1.e-5dl, 1.e-6dl, 1.e-7dl,
     1.e-8dl, 1.e-9dl, 1.e-10dl, 1.e-11dl,
     1.e-12dl, 1.e-13dl, 1.e-14dl, 1.e-15dl,
     1.e-16dl, 1.e-17dl, 1.e-18dl, 1.e-19dl,
     1.e-20dl, 1.e-21dl, 1.e-22dl, 1.e-23dl,
     1.e-24dl, 1.e-25dl, 1.e-26dl, 1.e-27dl,
     1.e-28dl, 1.e-29dl, 1.e-30dl, 1.e-31dl,
     1.e-32dl, 1.e-33dl, 1.e-34dl };

/* Stripped down ldexpd128 without NaN or Infinity detection.  */
static _Decimal128
__dfp_printf_ldexpd128 (_Decimal128 x, int y)
{
  if (y == 0)
    return x;
  if (y > 0)
    return (x * (10 * powof10[y]));
  else
    return (x * (10 * negpowof10[-(y)]));
}

static _Decimal128 __dfp_adjust_precision(_Decimal128 x, int prec)
{
  _Decimal128 deconst;
  _Decimal128 y = x;
  int powof10 = (34 - prec);

  if (x==0.0DL || __builtin_isnand128(x) || __builtin_isinfd128(x))
    return x;

  /* Get a number which, when added to 'x' causes the bits outside of the
   * desired precision to be outside of the representable number of digits
   * for a _Decimal128 (i.e. 34 digits).  e.g.
   *  0.01235 with a requested precision of '4' expects 0.0124
   *    100000000000000000000000000000.0000|<- precision boundary for _Decimal128
   *  +                              0.01235<- guard digit causes rounding
   *    -----------------------------------
   *    100000000000000000000000000000.0124
   *  - 100000000000000000000000000000.0000
   *    -----------------------------------
   *                                 0.0124
   */
  deconst = __dfp_printf_ldexpd128(0.01DL, powof10);
  if (x<0.0DL)
      deconst = -(deconst); /* reverse the forthcoming operation.  */

  /* exceed the number of available digits, the guard digit causes rounding.  */
  x += deconst;
  x -= deconst;

  if (x==0.0DL && y < 0.0DL)
    return (-0.0DL); /* Explicitly return -0.0 because GCC is too smart.  */
  return x;
}

/* slen should be DECIMAL128_Pmax + 14 + info->width + 1 (for '\0')  */
char * INTERNAL_FUNCTION_NAME ( const struct printf_info *info,
				const void *const *args,
				char * str,
				int slen)
{
  DEC_TYPE *y = *(DEC_TYPE **)args[0];
  DEC_TYPE z; /* Used if we need to adjust the precision.  */
  char dtos[MAX_DECIMAL128_STRING];
  int dtoslen = 0;
  char * padded = str; /* Used if there is extra width padding necessary.  */

  if (!str || slen <= 0)
    return NULL;

  memset(dtos, 0x0, MAX_DECIMAL128_STRING);

  /* Overwrite 'z' with a precision adjusted _Decimal[32|64|128] per the printf
   * format precision flag.  */
  if (info->prec > 0)
    z = (DEC_TYPE)__dfp_adjust_precision((DEC_TYPE)*y, info->prec);
  else
    z = *y;

  if (info->spec == 'E' || info->spec == 'e')
    {
      IEEE_DECIMAL_TO_ENG_STRING(&z, dtos);
    }
  else
    IEEE_DECIMAL_TO_STRING(&z, dtos);

  dtoslen = strlen(dtos);

  /* We don't need to pad if dtoslen exceeds the width flag, or if no width flag
   * is set.  */
  if (info->width && (dtoslen < info->width))
    {
      wchar_t pad = ' '; /* default padding */
      int padnum;

      if (info->pad)
        pad = info->pad;

      padnum = info->width - dtoslen;

      /* Justification indicates where padding is placed.  */
      if (!info->left)
        {
          memset(str, pad, padnum);
          padded = str + padnum;
        }
      else
        memset(str + dtoslen, pad, padnum);
        /* No need in this case to adjust 'padded' from the beginning of 'str'.  */

      str[info->width] = 0x0;
    }
  else /* Set the null terminator before copying the string.  */
    str[dtoslen] = 0x0;

  strncpy(padded, dtos, dtoslen);
  memset(dtos, 0x0, MAX_DECIMAL128_STRING);

  return str;
}
weak_alias (INTERNAL_FUNCTION_NAME, EXTERNAL_FUNCTION_NAME)
hidden_def(INTERNAL_FUNCTION_NAME)
