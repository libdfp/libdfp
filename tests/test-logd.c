/* Test islogd[32|64|128].

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

//#include <float.h> /* DEC_NAN definition.  */
//#include <dfp.h>
#include <stdio.h>
#include <math.h>

#define _WANT_VC 1 /* Pick up the _VC_P(x,y,fmt) macro.  */
#define _WANT_QC 1 /* Pick up the _QC_P(x,y,fmt) macro.  */
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
  {__LINE__, __DEC64_MAX__, 886.4952608027076DD, "%DfDD",'v'},
  {__LINE__, -1.0DD, DEC_NAN,  "%DfDD",'v'},
  {0,0,0,0,0 }
};

int main (void)
{
//  d128_type *d128ptr;
  d64_type *d64ptr;
 // d32_type *d32ptr;

  for (d64ptr = printf_d64s; d64ptr->line; d64ptr++)
    {
      _Decimal64 retval = logd64(d64ptr->x);
      fprintf(stdout,"%DfDD = logd64(%DfDD) in: %s:%d\n", retval, d64ptr->x,__FILE__,__LINE__-1);
      if(d64ptr->vorq == 'v')
        _VC_P(__FILE__,d64ptr->line, d64ptr->e,retval,d64ptr->format);
//      else
 //       _QC_P(__FILE__,d64ptr->line, d64ptr->e,retval,d64ptr->format);
    }

  _REPORT();

  /* fail comes from scaffold.c  */
  return fail;
}


