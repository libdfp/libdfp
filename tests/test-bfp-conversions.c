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
  {__LINE__, 12.345, 12.345DD,  "%De"},
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

/* Test extenddfdd () - Double float to double decimal conversions,
 * i.e., double -> _Decimal64.  */
typedef struct{
  int line;
  double x;       /* df 'double float' value to convert.  */
  _Decimal64 e;  /* dd 'double decimal' converted value.  */
  const char *format; /* printf %Df */
} dfdd_type;

dfdd_type dfdd_tests[] =
{
  {__LINE__, 12.3456789, 12.3456789DD,  "%De"},
  {0,0,0,0 }
};

/* Test extenddftd () - Double float to quad decimal conversions,
 * i.e., double -> _Decimal128.  */
typedef struct{
  int line;
  double x;       /* df 'double float' value to convert.  */
  _Decimal128 e;  /* td 'quad decimal' converted value.  */
  const char *format; /* printf %DDf */
} dftd_type;

dftd_type dftd_tests[] =
{
  {__LINE__, 12.3456789, 12.3456789DL,  "%DDe"},
  {0,0,0,0 }
};

/* Test extendtftd () - long double float to quad decimal conversions,
 * i.e., long double -> _Decimal128.  */
typedef struct{
  int line;
  long double x;       /* tf 'long double float' value to convert.  */
  _Decimal128 e;  /* td 'quad decimal' converted value.  */
  const char *format; /* printf %DDe */
} tftd_type;

tftd_type tftd_tests[] =
{
  {__LINE__, 12.3456789, 12.3456789DL,  "%DDe"},
  {0,0,0,0 }
};

/* Test extendsddf () - Single decimal to double float conversions,
 * i.e., _Decimal32 -> double.  */
typedef struct{
  int line;
  _Decimal32 x;	/* sd 'single decimal' value to convert.  */
  double e;	/* df 'double float' converted value.  */
  const char *format; /* printf %e */
} sddf_type;

sddf_type sddf_tests[] =
{
  {__LINE__, 12.345DF, 12.345,  "%e"},
  {0,0,0,0 }
};

/* Test extendsdtf () - Single decimal to long double float conversions,
 * i.e., _Decimal32 -> long double.  */
typedef struct{
  int line;
  _Decimal32 x;	/* sd 'single decimal' value to convert.  */
  long double e;/* df 'long double float' converted value.  */
  const char *format; /* printf %Le */
} sdtf_type;

sdtf_type sdtf_tests[] =
{
  {__LINE__, 12.345DF, 12.345,  "%Le"},
  {0,0,0,0 }
};

/* Test extendddtf () - Double decimal to long double float conversions,
 * i.e., _Decimal64 -> long double.  */
typedef struct{
  int line;
  _Decimal64 x;	/* dd 'double decimal' value to convert.  */
  long double e;/* df 'long double float' converted value.  */
  const char *format; /* printf %Le */
} ddtf_type;

ddtf_type ddtf_tests[] =
{
  {__LINE__, 12.3456789DD, 12.3456789,  "%Le"},
  {0,0,0,0 }
};

int main (void)
{
  sfsd_type *sfsdp;
  sfdd_type *sfddp;
  sftd_type *sftdp;
  dfdd_type *dfddp;
  dftd_type *dftdp;
  tftd_type *tftdp;
  sddf_type *sddfp;
  sdtf_type *sdtfp;
  ddtf_type *ddtfp;

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
      fprintf(stdout, "%e; /* float */ in: %s: %d\n", (double) sfsdp->x,__FILE__,__LINE__-4);
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
      fprintf(stdout, "%e; /* float */ in: %s: %d\n", (double) sfddp->x,__FILE__,__LINE__-4);
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
      fprintf(stdout, "%e; /* float */ in: %s: %d\n", (double) sftdp->x,__FILE__,__LINE__-4);
      fprintf(stdout, "expected: %s\n", decoded128(sftdp->e,decodebuf));
      fprintf(stdout, "retval:   %s\n", decoded128(retval,decodebuf));
      _VC_P(__FILE__,sftdp->line, sftdp->e, retval, sftdp->format);
    }

  for (dfddp = dfdd_tests; dfddp->line; dfddp++)
    {
      /* This will force the conversion and result in the hidden call to
       * __dpd_extenddfdd ().  */
      _Decimal64 retval = dfddp->x;
      /* Broken into two because printf has a bug when you do %Hf and %f in the
       * same printf statement.  */
      fprintf(stdout, "%De = (_Decimal64) ", retval);
      fprintf(stdout, "%e; /* double */ in: %s: %d\n", dfddp->x,__FILE__,__LINE__-4);
      fprintf(stdout, "expected: %s\n", decoded64(dfddp->e,decodebuf));
      fprintf(stdout, "retval:   %s\n", decoded64(retval,decodebuf));
      _VC_P(__FILE__,dfddp->line, dfddp->e, retval, dfddp->format);
    }

  for (dftdp = dftd_tests; dftdp->line; dftdp++)
    {
      /* This will force the conversion and result in the hidden call to
       * __dpd_extenddftd ().  */
      _Decimal128 retval = dftdp->x;
      /* Broken into two because printf has a bug when you do %Hf and %f in the
       * same printf statement.  */
      fprintf(stdout, "%DDe = (_Decimal128) ", retval);
      fprintf(stdout, "%e; /* double */ in: %s: %d\n", dftdp->x,__FILE__,__LINE__-4);
      fprintf(stdout, "expected: %s\n", decoded128(dftdp->e,decodebuf));
      fprintf(stdout, "retval:   %s\n", decoded128(retval,decodebuf));
      _VC_P(__FILE__,dftdp->line, dftdp->e, retval, dftdp->format);
    }

  for (tftdp = tftd_tests; tftdp->line; tftdp++)
    {
      /* This will force the conversion and result in the hidden call to
       * __dpd_extendtftd ().  */
      _Decimal128 retval = tftdp->x;
      /* Broken into two because printf has a bug when you do %Hf and %f in the
       * same printf statement.  */
      fprintf(stdout, "%DDe = (_Decimal128) ", retval);
      fprintf(stdout, "%Le; /* long double */ in: %s: %d\n", tftdp->x,__FILE__,__LINE__-4);
      fprintf(stdout, "expected: %s\n", decoded128(tftdp->e,decodebuf));
      fprintf(stdout, "retval:   %s\n", decoded128(retval,decodebuf));
      _VC_P(__FILE__,tftdp->line, tftdp->e, retval, tftdp->format);
    }

  for (sddfp = sddf_tests; sddfp->line; sddfp++)
    {
      /* This will force the conversion and result in the hidden call to
       * __dpd_extendsddf ().  */
      double retval = sddfp->x;
      /* Broken into two because printf has a bug when you do %Hf and %f in the
       * same printf statement.  */
      fprintf(stdout, "%e = (double) ", retval);
      fprintf(stdout, "%He; /* _Decimal32 */ in: %s: %d\n", sddfp->x,__FILE__,__LINE__-4);
      _VC_P(__FILE__,sddfp->line, sddfp->e, retval, sddfp->format);
    }

  for (sdtfp = sdtf_tests; sdtfp->line; sdtfp++)
    {
      /* This will force the conversion and result in the hidden call to
       * __dpd_extendsdtf ().  */
      long double retval = sdtfp->x;
      /* Broken into two because printf has a bug when you do %Hf and %f in the
       * same printf statement.  */
      fprintf(stdout, "%Le = (long double) ", retval);
      fprintf(stdout, "%He; /* _Decimal32 */ in: %s: %d\n", sdtfp->x,__FILE__,__LINE__-4);
      _VC_P(__FILE__,sdtfp->line, sdtfp->e, retval, sdtfp->format);
    }

  for (ddtfp = ddtf_tests; ddtfp->line; ddtfp++)
    {
      /* This will force the conversion and result in the hidden call to
       * __dpd_extendddtf ().  */
      long double retval = ddtfp->x;
      /* Broken into two because printf has a bug when you do %Hf and %f in the
       * same printf statement.  */
      fprintf(stdout, "%Le = (long double) ", retval);
      fprintf(stdout, "%De; /* _Decimal64 */ in: %s: %d\n", ddtfp->x,__FILE__,__LINE__-4);
      _VC_P(__FILE__,ddtfp->line, ddtfp->e, retval, ddtfp->format);
    }


  _REPORT();

  /* fail comes from scaffold.c  */
  return fail;
}

