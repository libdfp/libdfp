/* Convert _Decimal32 to string (strfromd32).

   Copyright (C) 2020 Free Software Foundation, Inc.

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

   Please see libdfp/COPYING.txt for more information.  */

#include <stdlib.h>
#include <stdio.h>
#include <printf.h>
#include <errno.h>
#include <ctype.h>

#include "printf_dfp.h"

#ifndef FUNC_NAME
#define FUNC_NAME strfromd32
#define DECIMALN  _Decimal32
#endif

int
FUNC_NAME (char * __restrict s, size_t n, const char * __restrict format, DECIMALN fp)
{
  /* format = %{[.][0-9]*[aAeEfFgG], else behavior undefined.
     Reuse our printf hook to do this.  */
  FILE * f = fmemopen (s, n, "w");
  const void * const args = &fp;
  const void * const * argsp = &args;
  const void * const * argp = (const void * const *)&argsp;
  struct printf_info info = { 0 };
  int num = 0;

  /* TODO: this is probably better than crashing.  The standard isn't so clear
     about this.  */
  if (!f)
    {
    /* errno set by fmemopen.  */
    return 0;
    }

  info.user = sizeof (fp) == 4 ? mod_H : (sizeof (fp) == 8 ? mod_D : mod_DD);
  info.prec = -1;
  info.spec = 'a';

  if (format[0] == '%') {
    format++;
    if (format[0] == '.') {
      format++;
      info.prec = 0;
      while (isdigit (format[0])) {
	info.prec = info.prec * 10 + format[0] - '0';
	format++;
      }
    }

    switch (format[0]) {
      case 'a':
      case 'A':
      case 'e':
      case 'E':
      case 'f':
      case 'F':
      case 'g':
      case 'G':
	info.spec = format[0];
        num = __printf_dfp (f, &info, argp);
	break;
    }
  }

  fclose (f);
  return num;
}
