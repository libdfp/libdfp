/* Test log10d[32|64|128].

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

#include "decode.h"
#define _WANT_VC 1 /* Pick up the _VC_P(x,y,fmt) macro.  */

#include "scaffold.c" /* Pick up the _VC_P(x,y,fmt) macro.  */

typedef struct{
  int line;
  _Decimal128 x;  /* Value to test  */
  _Decimal128 e;  /* Result should be this.  */
  const char *format; /* printf %DfDD */
  char vorq;  /* value compare or printf compare (quantize equivalent)  */
} d128_type;

d128_type printf_d128s[] =
{
  /* This value revealed a typo in the dpd_to_char array in 
   * sysdeps/dpd/dpd-private.c which is used as a lookup table.  What is now
   * '774' was erroneously '974' and was resulting in incorrect display of the
   * expected value.
   *
   * echo 'scale = 33; l(0.0000000011)/l(10)' | bc -l
   * == -8.958607314841774959249800028756976  */
  {__LINE__, 1.1E-9DL, -8.958607314841774959249800028756976DL, "%.34DDfDL",'v'},
  {0,0,0,0,0 }
};

int main (void)
{
  d128_type *d128ptr;

  for (d128ptr = printf_d128s; d128ptr->line; d128ptr += sizeof(_Decimal128 *))
    {
      _Decimal128 retval = log10d128(d128ptr->x);
      fprintf(stdout,"%.34DDfDL = log10d128(%.34DDfDL) in: %s:%d\n", retval, d128ptr->x,__FILE__,__LINE__-1);
      if(d128ptr->vorq == 'v')
        _VC_P(__FILE__,d128ptr->line, d128ptr->e,retval,d128ptr->format);
      if(d128ptr->e != retval)
        {
	  static char rbuf[CHAR_MAX];
	  static char pbuf[CHAR_MAX];
	  fprintf(stderr,"decoded128(retval) [%s] != decoded128(expected) [%s]\n", decoded128(retval, &rbuf[0]), decoded128(d128ptr->e, &pbuf[0]));
	}
    }

  _REPORT();

  /* fail comes from scaffold.c  */
  return fail;
}


