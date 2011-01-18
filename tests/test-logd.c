/* Test logd[32|64|128].

   Copyright (C) 2010 Free Software Foundation, Inc.

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

#define _WANT_VC 1 /* Pick up the _VC_P(x,y,fmt) macro.  */
#define _WANT_QC 1 /* Pick up the _QC_P(x,y,fmt) macro.  */
#define _WANT_DC 1 /* Pick up the _DC_P(x,y,fmt) macro.  */

#include "scaffold.c" /* Pick up the _VC_P(x,y,fmt) macro.  */

/* Inspired by GLIBC stdio-common/tfformat.c  */
typedef struct{
  int line;
  _Decimal64 x;  /* Value to test  */
  _Decimal64 e;  /* Result should be this.  */
  const char *format; /* printf %DfDD */
  char vorq;  /* value compare or printf compare (quantize equivalent)  */
} d64_type;

d64_type printf_d64s[] =
{
  {__LINE__, 0.9956440572495333DD, -0.004365457509639973DD,  "%DfDD",'v'},
  {__LINE__, 0.9828009828009828DD, -0.01734863833461300DD, "%DfDD",'v'},
  {__LINE__, 1.0DD, 0.0DD, "%DfDD",'v'},
  {__LINE__, 1.02DD, 0.019803DD, "%.6DfDD",'q'},
  {__LINE__, 1.02DD, 0.01980262729617971DD, "%DfDD",'v'},
  {__LINE__, 3.14DD, 1.144222799920162DD, "%DfDD",'v'},
#ifndef _ARCH_PWR6 /* This returns NaN in the hard-DFP case.  */
  {__LINE__, __DEC64_MAX__, 886.4952608027076DD, "%DfDD",'v'},
#endif
  {0,0,0,0,0 }
};

typedef struct{
  int line;
  _Decimal64 x;  /* Value to test  */
  const char *expect;
} d64_decode_type;

const char DECLET32_NAN[] = "+0,000,000E-101";
const char DECLET64_NAN[] = "+0,000,000,000,000,000E-398";
const char DECLET128_NAN[] = "+0,000,000,000,000,000,000,000,000,000,000,000E-6176";

d64_decode_type decode_d64s[] =
{
  /* DEC_NAN is +0,000,000,000,000,000E-398 so test against that
   * since you can't compare DEC_NAN to DEC_NAN.  */
  {__LINE__, -1.0DD, DECLET64_NAN},
#ifdef _ARCH_PWR6 /* This returns NaN in the hard-DFP case.  */
  {__LINE__, __DEC64_MAX__, DECLET64_NAN},
#endif
  {0,0,0 }
};

int main (void)
{
  d64_type *d64ptr;
  d64_decode_type *d64dptr;

  for (d64ptr = printf_d64s; d64ptr->line; d64ptr++)
    {
      _Decimal64 retval = logd64(d64ptr->x);
      fprintf(stdout,"%DfDD = logd64(%DfDD) in: %s:%d\n", retval, d64ptr->x,__FILE__,__LINE__-1);
      if(d64ptr->vorq == 'v')
        _VC_P(__FILE__,d64ptr->line, d64ptr->e,retval,d64ptr->format);
      if(d64ptr->e != retval)
        {
	  static char rbuf[CHAR_MAX];
	  static char pbuf[CHAR_MAX];
	  fprintf(stderr,"decoded64(retval) [%s] != decoded64(expected) [%s]\n", decoded64(retval, &rbuf[0]), decoded64(d64ptr->e, &pbuf[0]));
	}
    }

  for (d64dptr = decode_d64s; d64dptr->line; d64dptr++)
    {
	  static char rbuf[CHAR_MAX];
      //_Decimal64 retval = logd64(d64dptr->x);
      _Decimal64 retval = logd64(DEC_NAN);
      fprintf(stdout,"%DfDD = logd64(%DfDD) in: %s:%d\n", retval, d64dptr->x,__FILE__,__LINE__-1);
      _DC_P(__FILE__,d64dptr->line, d64dptr->expect,retval);
      if(strcmp(d64dptr->expect,decoded64(retval,&rbuf[0])))
        {
	  static char dbuf[CHAR_MAX];
	  fprintf(stderr,"decoded64(__DEC64_MAX__)[%s], decoded64(retval)[%s], decoded64(d64dptr->e)[%s]\n", decoded64(__DEC64_MAX__, &dbuf[0]), decoded64(retval, &rbuf[0]), d64dptr->expect);
	}
    }
  _REPORT();

  /* fail comes from scaffold.c  */
  return fail;
}


