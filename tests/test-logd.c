/* Test logd[32|64|128].

   Copyright (C) 2010-2014 Free Software Foundation, Inc.

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

#include <stdio.h>
#include <math.h>
#include <string.h>

#define _WANT_VC 1		/* Pick up the _VC_P(x,y,fmt) macro.  */
#define _WANT_QC 1		/* Pick up the _QC_P(x,y,fmt) macro.  */
#define _WANT_DC 1		/* Pick up the _DC_P(x,y,fmt) macro.  */

#include "scaffold.c"		/* Pick up the _VC_P(x,y,fmt) macro.  */

typedef struct
{
  int line;
  _Decimal128 x;		/* Value to test  */
  _Decimal128 e;		/* Result should be this.  */
  const char *format;		/* printf %DfDD */
  char vorq;			/* value compare or printf compare (quantize equivalent)  */
} d128_type;

d128_type printf_d128s[] = {
  {__LINE__, 9.956441e-01DL, -4.3654145721409025130657501205664800e-03DL,
   "%.34DDeDL", 'v'},
  {__LINE__, 9.828010e-01DL, -1.7348620834613154684590310253185030e-02DL,
   "%.34DDeDL", 'v'},
  {__LINE__, 1.000000e+00DL, 0.0000000000000000000000000000000000e+00DL,
   "%.34DDeDL", 'v'},
  {__LINE__, 1.020000e+00DL, 1.9802627296179713026029066885100390e-02DL,
   "%.34DDeDL", 'v'},
  {__LINE__, 3.140000e+00DL, 1.1442227999201619988056944484929230e+00DL,
   "%.34DDeDL", 'v'},
#ifndef _ARCH_PWR6		/* This returns NaN in the hard-DFP case.  */
  {__LINE__, __DEC64_MAX__, 886.4952608027076DL, "%DeDL", 'v'},
#endif
  {0, 0, 0, 0, 0}
};

typedef struct
{
  int line;
  _Decimal64 x;			/* Value to test  */
  _Decimal64 e;			/* Result should be this.  */
  const char *format;		/* printf %DfDD */
  char vorq;			/* value compare or printf compare (quantize equivalent)  */
} d64_type;

d64_type printf_d64s[] = {
  {__LINE__, 9.956441e-01DD, -4.365414572140903e-03DD, "%.15DeDD", 'v'},
  {__LINE__, 9.828010e-01DD, -1.734862083461315e-02DD, "%.15DeDD", 'v'},
  {__LINE__, 1.000000e+00DD,  0.000000000000000e+00DD, "%.15DeDD", 'v'},
  {__LINE__, 1.020000e+00DD,  1.980262729617971e-02DD, "%.15DeDD", 'v'},
  {__LINE__, 3.140000e+00DD,  1.144222799920162e+00DD, "%.15DeDD", 'v'},
#ifndef _ARCH_PWR6		/* This returns NaN in the hard-DFP case.  */
  {__LINE__, __DEC64_MAX__, 886.4952608027076DD, "%DeDD", 'v'},
#endif
  {0, 0, 0, 0, 0}
};

typedef struct
{
  int line;
  _Decimal32 x;			/* Value to test  */
  _Decimal32 e;			/* Result should be this.  */
  const char *format;		/* printf %DfDD */
  char vorq;			/* value compare or printf compare (quantize equivalent)  */
} d32_type;

d32_type printf_d32s[] = {
  {__LINE__, 9.956441e-01DF, -4.365415e-03DF, "%.6HeDF", 'v'},
  {__LINE__, 9.828010e-01DF, -1.734862e-02DF, "%.6HeDF", 'v'},
  {__LINE__, 1.000000e+00DF, 0.000000e+00DF, "%.6HeDF", 'v'},
  {__LINE__, 1.020000e+00DF, 1.980263e-02DF, "%.6HeDF", 'v'},
  {__LINE__, 3.140000e+00DF, 1.144223e+00DF, "%.6HeDF", 'v'},
#ifndef _ARCH_PWR6		/* This returns NaN in the hard-DFP case.  */
  {__LINE__, __DEC32_MAX__, 886.4952608027076DD, "%.6HeDF", 'v'},
#endif
  {0, 0, 0, 0, 0}
};

typedef struct
{
  int line;
  _Decimal64 x;			/* Value to test  */
  const char *expect;
} d64_decode_type;

const char DECLET32_NAN[] = "+0,000,000E-101";
const char DECLET64_NAN[] = "+0,000,000,000,000,000E-398";
const char DECLET128_NAN[] =
  "+0,000,000,000,000,000,000,000,000,000,000,000E-6176";

d64_decode_type decode_d64s[] = {
  /* DEC_NAN is +0,000,000,000,000,000E-398 so test against that
   * since you can't compare DEC_NAN to DEC_NAN.  */
  {__LINE__, -1.0DD, DECLET64_NAN},
#ifdef _ARCH_PWR6		/* This returns NaN in the hard-DFP case.  */
  {__LINE__, __DEC64_MAX__, DECLET64_NAN},
#endif
  {0, 0, 0}
};

int
main (void)
{
  d128_type *d128ptr;
  d64_type *d64ptr;
  d32_type *d32ptr;
  d64_decode_type *d64dptr;

  for (d128ptr = printf_d128s; d128ptr->line; d128ptr++)
    {
      _Decimal128 retval = logd128 (d128ptr->x);
      fprintf (stdout, "%.34DDeDL = logd128(%.34DDeDL) in: %s:%d\n", retval,
	       d128ptr->x, __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d128ptr->line, d128ptr->e, retval, d128ptr->format);
    }

  for (d64ptr = printf_d64s; d64ptr->line; d64ptr++)
    {
      _Decimal64 retval = logd64 (d64ptr->x);
      fprintf (stdout, "%.15DeDD = logd64(%.15DeDD) in: %s:%d\n", retval,
	       d64ptr->x, __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d64ptr->line, d64ptr->e, retval, d64ptr->format);
    }

  for (d32ptr = printf_d32s; d32ptr->line; d32ptr++)
    {
      _Decimal32 retval = logd32 (d32ptr->x);
      fprintf (stdout, "%.6HeDF = logd32(%.6HeDF) in: %s:%d\n", retval,
	       d32ptr->x, __FILE__, __LINE__ - 1);
      _VC_P (__FILE__, d32ptr->line, d32ptr->e, retval, d32ptr->format);
    }

  for (d64dptr = decode_d64s; d64dptr->line; d64dptr++)
    {
      static char rbuf[CHAR_MAX];
      //_Decimal64 retval = logd64(d64dptr->x);
      _Decimal64 retval = logd64 (DEC_NAN);
      fprintf (stdout, "%DfDD = logd64(%DfDD) in: %s:%d\n", retval,
	       d64dptr->x, __FILE__, __LINE__ - 1);
      _DC_P (__FILE__, d64dptr->line, d64dptr->expect, retval);
      if (strcmp (d64dptr->expect, decoded64 (retval, &rbuf[0])))
	{
	  static char dbuf[CHAR_MAX];
	  fprintf (stderr,
		   "decoded64(__DEC64_MAX__)[%s], decoded64(retval)[%s], decoded64(d64dptr->e)[%s]\n",
		   decoded64 (__DEC64_MAX__, &dbuf[0]), decoded64 (retval,
								   &rbuf[0]),
		   d64dptr->expect);
	}
    }
  _REPORT ();

  /* fail comes from scaffold.c  */
  return fail;
}
