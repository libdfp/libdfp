/* Test bfp -> dfp and dfp -> bfp conversions

   Copyright (C) 2011-2015 Free Software Foundation, Inc.

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
  {__LINE__, 2.0, 2.0DF, "%He"},
  {__LINE__, 30.0, 30.0DF, "%He"},
  {__LINE__, 1.0e-20, 1.0e-20DF, "%He"},
  {__LINE__, 1.0e-96, 0.0DF, "%He"},
  {__LINE__, 3.402819e+38, 3.402819e+38DF, "%He"},
  {__LINE__, 100000., 100000.0DF, "%He"},
  {__LINE__, 999999., 999999.0DF, "%He"},
  {__LINE__, 1.2e-38, 1.2e-38DF, "%He"},
  {__LINE__, 1.1e-38, 1.1e-38DF, "%He"},
  {__LINE__, -1.2e-38, -1.2e-38DF, "%He"},
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
  {__LINE__, 12.345, 12.345000267028809DD,  "%De"},
  {__LINE__, 30.0, 30.0DD, "%De"},
  {__LINE__, 3.402819e+38f, 3.402819004255176e+38DD, "%De"},
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
  {__LINE__, 12.345, 12.3450002670288085937500000000000000DL,  "%DDe"},
  {__LINE__, 30.0, 30.0DL,  "%DDe"},
  {__LINE__, 3.402819e+38f, 3.402819004255175794749548566449886e+38DL,  "%DDe"},
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
  {__LINE__, -2.0, -2.0DD,  "%De"},
  {__LINE__, 1.79768e+308, 1.79768e+308DD,  "%De"},
  {__LINE__, 100000000000000, 100000000000000.0DD,  "%De"},
  {__LINE__, 999999999999999, 999999999999999.0DD,  "%De"},
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
  {__LINE__, 12.3456789, 12.3456788999999993450273905182257295DL,  "%DDe"},
  {__LINE__, -2.0, -2.0DL,  "%DDe"},
  {__LINE__, 1.79768e+308, 1.797679999999999908416854000763986e+308DL,  "%DDe"},
  {0,0,0,0 }
};

/* Test extendxftd () - long double float to quad decimal conversions,
 * i.e., long double -> _Decimal128.  (x86) */
typedef struct{
  int line;
  long double x;       /* tf 'long double float' value to convert.  */
  _Decimal128 e;  /* td 'quad decimal' converted value.  */
  const char *format; /* printf %DDe */
} xftd_type;

xftd_type xftd_tests[] =
{
  {__LINE__, 12.3456789, 12.3456788999999993450273905182257295DL,  "%DDe"},
  {__LINE__, -22.0, -22.0DL,  "%DDe"},
  {__LINE__, 100000000000000000, 100000000000000000.0DL,  "%DDe"},
  {__LINE__, 999999999999999999, 999999999999999999.0DL,  "%DDe"},
  {__LINE__, 100000000000000000000545460846592.0L, 100000000000000000000545460846592.0DL,  "%DDe"},
  {__LINE__, 999999999999999999970270236377088.0L, 999999999999999999970270236377088.0DL,  "%DDe"},
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
  {__LINE__, 12.3456789, 12.3456788999999993450273905182257295DL,  "%DDe"},
  {__LINE__, -22.0, -22.0DL,  "%DDe"},
  {__LINE__, 100000000000000000, 100000000000000000.0DL,  "%DDe"},
  {__LINE__, 999999999999999999, 999999999999999999.0DL,  "%DDe"},
  {__LINE__, 100000000000000000000000000000000.0L, 100000000000000000000000000000000.0DL,  "%DDe"},
#if __LDBL_MANT_DIG__ < 113
  /* IEEE128 long double has more mantissa precision than _Decimal128.  */
  {__LINE__, 999999999999999999999999999999999.0L, 1000000000000000000000000000000000.0DL,  "%.33DDe"},
#else
  {__LINE__, 999999999999999999999999999999999.0L,  999999999999999999999999999999999.0DL,  "%.33DDe"},
#endif
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
  {__LINE__, 2.0DF, 2.0,  "%e"},
  {__LINE__, 3.402819e+38DF, 3.402819e+38, "%e"},
  {__LINE__, 9.999998e96DF, 9.999998e96,  "%e"},
  {__LINE__, 100000.0DF, 100000,  "%e"},
  {__LINE__, 999999.0DF, 999999,  "%e"},
  {__LINE__, 1.e-39DF, 1.e-39,  "%e"},
  {__LINE__, -1.e-39DF, -1.e-39,  "%e"},
  {__LINE__, 1.2e-38DF, 1.2e-38,  "%e"},
  {__LINE__, 1.e-40DF, 1.e-40,  "%e"},
  {__LINE__, 8.e-44DF, 8.e-44,  "%e"},
  {__LINE__, 9.e-44DF, 9.e-44,  "%e"},
  {__LINE__, 8.e-46DF, 8.e-46,  "%e"},
  {__LINE__, -9.e-44DF, -9.e-44,  "%e"},
  {__LINE__, -8.e-46DF, -8.e-46,  "%e"},
  {0,0,0,0 }
};

/* Test extenddddf () - Double decimal to double float conversions,
 * i.e., _Decimal64 -> double.  */
typedef struct{
  int line;
  _Decimal64 x;	/* dd 'double decimal' value to convert.  */
  double e;	/* df 'double float' converted value.  */
  const char *format; /* printf %e */
} dddf_type;

dddf_type dddf_tests[] =
{
  {__LINE__, -7.0DD, -7.0,  "%De"},
  {__LINE__, 3.402819e+38DD, 3.402819e+38,  "%De"},
  {__LINE__, 1.79768e+308DD, 1.79768e+308,  "%De"},
  {__LINE__, 100000000000000.0DD, 100000000000000,  "%De"},
  {__LINE__, 999999999999999.0DD, 999999999999999,  "%De"},
  {__LINE__, 1.e290DD, 1.e290,  "%De"},
  {__LINE__, 1.e291DD, 1.e291,  "%De"},
  {__LINE__, 1.e308DD, 1.e308,  "%De"},
  {__LINE__, 1.e-308DD, 1.e-308,  "%De"},
  {__LINE__, 1.e-291DD, 1.e-291,  "%De"},
  {__LINE__, 1.e-290DD, 1.e-290,  "%De"},
  {__LINE__, 1.e-39DD, 1.e-39,  "%De"},
  {__LINE__, -1.e-39DD, -1.e-39,  "%De"},
  {0,0,0,0 }
};

#if __LDBL_MANT_DIG__ == 64
/* Test extendsdxf () - Single decimal to long double float conversions,
 * i.e., _Decimal32 -> long double.  */
typedef struct{
  int line;
  _Decimal32 x;	/* sd 'single decimal' value to convert.  */
  const char *e;/* string with expected converted value.  */
} sdxf_type;

sdxf_type sdxf_tests[] =
{
  {__LINE__, 12.345DF, "12.345000000000000000242861286636753"},
  {__LINE__, 2.0DF, "2.000000000000000000000000000000000"},
  {0,0,0 }
};

/* Test extendddxf () - Double decimal to long double float conversions,
 * i.e., _Decimal64 -> long double.  */
typedef struct{
  int line;
  _Decimal64 x;	/* dd 'double decimal' value to convert.  */
  const char *e;/* string with expected converted value.  */
} ddxf_type;

ddxf_type ddxf_tests[] =
{
  {__LINE__, 12.3456789DD, "12.345678899999999999885502699470408"},
  {__LINE__, -7.0DD, "-7.000000000000000000000000000000000"},
  {0,0,0 }
};

#else /* __LDBL_MANT_DIG__ == 106 or 113 */
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
#if __LDBL_MANT_DIG__ == 113
  {__LINE__, 12.345DF, 12.345L,  "%Le"},
#else
  {__LINE__, 12.345DF, 12.345,  "%Le"},
#endif
  {__LINE__, 2.0DF, 2.0, "%Le"},
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
#if __LDBL_MANT_DIG__ == 113
  {__LINE__, 12.3456789DD, 12.3456789L,  "%Le"},
#else
  {__LINE__, 12.3456789DD, 12.3456789,  "%Le"},
#endif
  {__LINE__, -7.0DD, -7.0,  "%Le"},
  {0,0,0,0 }
};

#endif

#if __LDBL_MANT_DIG__ == 106
/* There is a lack of precision in division with IBM long double on PowerPC. */
union precision_expected{
  long double ld;
  double d[2];
};
#endif

/* Test extendtdtf () - Double decimal to long double float conversions,
 * i.e., _Decimal128 -> long double. */
typedef struct{
  int line;
  _Decimal128 x; /* td 'quad decimal' value to convert.  */
  long double e;/* tf 'long double float' converted value.  */
  double ppce1; /* df 'double float' part of the expected value for IBM long double. */
  double ppce2; /* df 'double float' part of the expected value for IBM long double. */
  const char *format; /* printf %DDe */
} tdtf_type;

tdtf_type tdtf_tests[] =
{
  {__LINE__, 30.0DL, 30.0, 30, 0, "%DDe"},
  /* 2**(-11) = 0.00048828125. */
  {__LINE__, 0.00048828125DL, 0.00048828125L, 0.00048828125, 0, "%DDe"},
  /* 2**(-25) = 0.298023223876953125E-7.  */
  {__LINE__, 2.98023223876953125E-8DL, 2.98023223876953125E-8, 2.9802322387695312e-08, 0, "%DDe"},
  {__LINE__, 3.402819e+38DL, 3.402819e+38L, 3.4028189999999998e+38, 1.7176206731396606e+22, "%DDe"},
  {__LINE__, 100000000000000000.0DL, 100000000000000000, 1e+17, 0, "%DDe"},
  {__LINE__, 999999999999999999.0DL, 999999999999999999, 1e+18, -1, "%DDe"},
  {__LINE__, 100000000000000000000000000000000.0DL, 100000000000000000000000000000000.L, 1.0000000000000001e+32, -5366162204393472, "%DDe"},
  {__LINE__, 999999999999999999999999999999999.0DL, 999999999999999999999999999999999.L, 9.9999999999999995e+32, 54424769012957184, "%DDe"},
  {__LINE__, 1.e290DL, 1.e290L, 1.0000000000000001e+290, -6.1727833527867157e+273, "%DDe"},
  {__LINE__, 1.e-290DL, 1.e-290L, 1.0000000000000001e-290, -6.9127868599625479e-307, "%DDe"},
  {__LINE__, 1.e-308DL, 1.e-308L, 9.9999999999999991e-309, 0, "%DDe"},
  {__LINE__, 1.e-291DL, 1.e-291L, 9.9999999999999996e-292, 3.7675676608720188e-308, "%DDe"},
  {__LINE__, 3.e-309DL, 3.e-309L, 3.0000000000000007e-309, 0, "%DDe"},
  {__LINE__, -3.e-309DL, -3.e-309L, -3.0000000000000007e-309, 0, "%DDe"},
  {0,0,0,0,0,0 }
};

int main (void)
{
  sfsd_type *sfsdp;
  sfdd_type *sfddp;
  sftd_type *sftdp;
  dfdd_type *dfddp;
  dftd_type *dftdp;
  sddf_type *sddfp;
  dddf_type *dddfp;
#if defined(__DECIMAL_BID_FORMAT__) && __LDBL_MANT_DIG__ == 64
  xftd_type *xftdp;
  sdxf_type *sdxfp;
  ddxf_type *ddxfp;
#else
  tftd_type *tftdp;
  sdtf_type *sdtfp;
  ddtf_type *ddtfp;
#endif
  tdtf_type *tdtfp;

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

  for (dddfp = dddf_tests; dddfp->line; dddfp++)
    {
      /* This will force the conversion and result in the hidden call to
       * __dpd_extenddddf ().  */
      double retval = dddfp->x;
      /* Broken into two because printf has a bug when you do %Hf and %f in the
       * same printf statement.  */
      fprintf(stdout, "%e = (double) ", retval);
      fprintf(stdout, "%De; /* _Decimal64 */ in: %s: %d\n", dddfp->x,__FILE__,__LINE__-4);
      _VC_P(__FILE__,dddfp->line, dddfp->e, retval, dddfp->format);
    }

/* Intel long double has a different precision than PowerPC long double.
 * Intel converts to xf (80 bits) and PowerPC converts to tf (128 bits).*/
#if defined(__DECIMAL_BID_FORMAT__) && __LDBL_MANT_DIG__ == 64
#define DECIMAL_PRINTF_BUF_SIZE 65	/* ((DECIMAL128_PMAX + 14) * 2) + 1 */
  for (xftdp = xftd_tests; xftdp->line; xftdp++)
    {
      /* This will force the conversion and result in the hidden call to
       * __dpd_extendxftd ().  */
      _Decimal128 retval = xftdp->x;
      /* Broken into two because printf has a bug when you do %Hf and %f in the
       * same printf statement.  */
      fprintf(stdout, "%DDe = (_Decimal128) ", retval);
      fprintf(stdout, "%Le; /* long double */ in: %s: %d\n", xftdp->x,__FILE__,__LINE__-4);
      fprintf(stdout, "expected: %s\n", decoded128(xftdp->e,decodebuf));
      fprintf(stdout, "retval:   %s\n", decoded128(retval,decodebuf));
      _VC_P(__FILE__,xftdp->line, xftdp->e, retval, xftdp->format);
    }

  for (sdxfp = sdxf_tests; sdxfp->line; sdxfp++)
    {
      /* This will force the conversion and result in the hidden call to
       * __bid_extendsdxf ().  */
      long double retval = sdxfp->x;
      /* Broken into two because printf has a bug when you do %Hf and %f in the
       * same printf statement.  */
      char value[DECIMAL_PRINTF_BUF_SIZE];
      sprintf(value, "%.33Lf", retval);
      fprintf(stdout, "%Le = (long double) ", retval);
      fprintf(stdout, "%He; /* _Decimal32 */ in: %s: %d\n", sdxfp->x,__FILE__,__LINE__-4);
      _SC_P (__FILE__,sdxfp->line, sdxfp->e, &value[0]);
    }

  for (ddxfp = ddxf_tests; ddxfp->line; ddxfp++)
    {
      /* This will force the conversion and result in the hidden call to
       * __bid_extendddxf ().  */
      long double retval = ddxfp->x;
      /* Broken into two because printf has a bug when you do %Hf and %f in the
       * same printf statement.  */
      char value[DECIMAL_PRINTF_BUF_SIZE];
      sprintf(value, "%.33Lf", retval);
      fprintf(stdout, "%Le = (long double) ", retval);
      fprintf(stdout, "%De; /* _Decimal64 */ in: %s: %d\n", ddxfp->x,__FILE__,__LINE__-4);
      _SC_P (__FILE__,ddxfp->line, ddxfp->e, &value[0]);
    }

  for (tdtfp = tdtf_tests; tdtfp->line; tdtfp++)
    {
      /* This will force the conversion and result in the hidden call to
       * __dpd_extendtdtf ().  */
      long double retval = tdtfp->x;
      /* Broken into two because printf has a bug when you do %Hf and %f in the
       * same printf statement.  */
      fprintf(stdout, "%Le = (long double) ", retval);
      fprintf(stdout, "%DDe; /* _Decimal128 */ in: %s: %d\n", tdtfp->x,__FILE__,__LINE__-4);
      _VC_P(__FILE__,tdtfp->line, tdtfp->e, retval, tdtfp->format);
    }
#else
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

/* Intel long double has a different precision than PowerPC long double.
 * Intel converts to xf (80 bits) and PowerPC converts to tf (128 bits).*/
#if defined(__DECIMAL_BID_FORMAT__) && __LDBL_MANT_DIG__ == 64
#define DECIMAL_PRINTF_BUF_SIZE 65	/* ((DECIMAL128_PMAX + 14) * 2) + 1 */
  for (sdxfp = sdxf_tests; sdxfp->line; sdxfp++)
    {
      /* This will force the conversion and result in the hidden call to
       * __bid_extendsdxf ().  */
      long double retval = sdxfp->x;
      /* Broken into two because printf has a bug when you do %Hf and %f in the
       * same printf statement.  */
      char value[DECIMAL_PRINTF_BUF_SIZE];
      sprintf(value, "%.33Lf", retval);
      fprintf(stdout, "%Le = (long double) ", retval);
      fprintf(stdout, "%He; /* _Decimal32 */ in: %s: %d\n", sdxfp->x,__FILE__,__LINE__-4);
      _SC_P (__FILE__,sdxfp->line, sdxfp->e, &value[0]);
    }

  for (ddxfp = ddxf_tests; ddxfp->line; ddxfp++)
    {
      /* This will force the conversion and result in the hidden call to
       * __bid_extendddxf ().  */
      long double retval = ddxfp->x;
      /* Broken into two because printf has a bug when you do %Hf and %f in the
       * same printf statement.  */
      char value[DECIMAL_PRINTF_BUF_SIZE];
      sprintf(value, "%.33Lf", retval);
      fprintf(stdout, "%Le = (long double) ", retval);
      fprintf(stdout, "%De; /* _Decimal64 */ in: %s: %d\n", ddxfp->x,__FILE__,__LINE__-4);
      _SC_P (__FILE__,ddxfp->line, ddxfp->e, &value[0]);
    }
#else
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
#endif

  for (tdtfp = tdtf_tests; tdtfp->line; tdtfp++)
    {
      /* This will force the conversion and result in the hidden call to
       * __dpd_extendtdtf ().  */
      long double retval = tdtfp->x;

      /* IBM long double has a lack of precision in division with long double, so
       * the expected value needs to be adjusted. */
#if __LDBL_MANT_DIG__ == 106
      union precision_expected e = {.d = {tdtfp->ppce1, tdtfp->ppce2}};
      tdtfp->e = e.ld;
#endif
	
      /* Broken into two because printf has a bug when you do %Hf and %f in the
       * same printf statement.  */
      fprintf(stdout, "%Le = (long double) ", retval);
      fprintf(stdout, "%DDe; /* _Decimal128 */ in: %s: %d\n", tdtfp->x,__FILE__,__LINE__-8);
      _VC_P(__FILE__,tdtfp->line, tdtfp->e, retval, tdtfp->format);
    }
#endif

  _REPORT();

  /* fail comes from scaffold.c  */
  return fail;
}
