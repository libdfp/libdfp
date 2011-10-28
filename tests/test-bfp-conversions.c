/* Test bfp -> dfp and dfp -> bfp conversions

   Copyright (C) 2011 Free Software Foundation, Inc.

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

#ifndef __STDC_WANT_DEC_FP__
#define __STDC_WANT_DEC_FP__
#endif


#define _WANT_VC 1 /* Pick up the _VC_P(x,y,fmt) macro.  */
#include "scaffold.c" /* Pick up the _VC_P(x,y,fmt) macro.  */

/* Inspired by GLIBC stdio-common/tfformat.c  */

/* Test extendsfsd () - Single float to single decimal conversions,
 * i.e., float -> _Decimal32.  */
typedef struct{
  int line;
  float x;       /* sf 'single float' value to convert.  */
  _Decimal32 e;  /* sd 'single decimal' converted value.  */
  const char *format; /* printf %Hf */
} sfsd_type;

sfsd_type sfsd_tests[] =
{
  {__LINE__, 12.345, 12.345DF,  "%He"},
  {__LINE__, 12.345678, 12.345678DF,  "%He"},
  {__LINE__, 123456.7, 123456.7DF,  "%He"},
  {__LINE__, 999999.9, 999999.9DF,  "%He"},
  {0,0,0,0 }
};


/* Test extendsfdd () - Single float to double decimal conversions,
 * i.e., float -> _Decimal64.  */
typedef struct{
  int line;
  float x;       /* sf 'single float' value to convert.  */
  _Decimal64 e;  /* dd 'double decimal' converted value.  */
  const char *format; /* printf %Df */
} sfdd_type;

sfdd_type sfdd_tests[] =
{
  {__LINE__, 12.345, 12.345DD,  "%.De"},
  {0,0,0,0 }
};

/* Test extendsftd () - Single float to quad decimal conversions,
 * i.e., float -> _Decimal128.  */
typedef struct{
  int line;
  float x;       /* sf 'single float' value to convert.  */
  _Decimal128 e;  /* td 'quad decimal' converted value.  */
  const char *format; /* printf %DDf */
} sftd_type;

sftd_type sftd_tests[] =
{
  {__LINE__, 12.345, 12.345DL,  "%DDe"},
  {0,0,0,0 }
};

int main (void)
{
  sfsd_type *sfsdp;
  sfdd_type *sfddp;
  sftd_type *sftdp;

#include "decode.h"
  char decodebuf[256];

  for (sfsdp = sfsd_tests; sfsdp->line; sfsdp++)
    {
      /* This will force the conversion and result in the hidden call to
       * __dpd_extendsfsd ().  */
      _Decimal32 retval = sfsdp->x;
      /* Broken into two because printf has a bug when you do %Hf and %f in the
       * same printf statement.  */
      fprintf(stdout, "%He = (_Decimal32) ", retval);
      fprintf(stdout, "%f; /* float */ in: %s: %d\n", (double) sfsdp->x,__FILE__,__LINE__-4);
      fprintf(stdout, "expected: %s\n", decoded32(sfsdp->e,decodebuf));
      fprintf(stdout, "retval:   %s\n", decoded32(retval,decodebuf));
      _VC_P(__FILE__,sfsdp->line, sfsdp->e, retval, sfsdp->format);
    }

  for (sfddp = sfdd_tests; sfddp->line; sfddp++)
    {
      /* This will force the conversion and result in the hidden call to
       * __dpd_extendsfdd ().  */
      _Decimal64 retval = sfddp->x;
      /* Broken into two because printf has a bug when you do %Hf and %f in the
       * same printf statement.  */
      fprintf(stdout, "%De = (_Decimal64) ", retval);
      fprintf(stdout, "%f; /* float */ in: %s: %d\n", (double) sfddp->x,__FILE__,__LINE__-4);
      fprintf(stdout, "expected: %s\n", decoded64(sfddp->e,decodebuf));
      fprintf(stdout, "retval:   %s\n", decoded64(retval,decodebuf));
      _VC_P(__FILE__,sfddp->line, sfddp->e, retval, sfddp->format);
    }

  for (sftdp = sftd_tests; sftdp->line; sftdp++)
    {
      /* This will force the conversion and result in the hidden call to
       * __dpd_extendsftd ().  */
      _Decimal128 retval = sftdp->x;
      /* Broken into two because printf has a bug when you do %Hf and %f in the
       * same printf statement.  */
      fprintf(stdout, "%DDe = (_Decimal128) ", retval);
      fprintf(stdout, "%f; /* float */ in: %s: %d\n", (double) sftdp->x,__FILE__,__LINE__-4);
      fprintf(stdout, "expected: %s\n", decoded128(sftdp->e,decodebuf));
      fprintf(stdout, "retval:   %s\n", decoded128(retval,decodebuf));
      _VC_P(__FILE__,sftdp->line, sftdp->e, retval, sftdp->format);
    }



}

