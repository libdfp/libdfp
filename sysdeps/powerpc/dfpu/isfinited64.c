/* Returns non-zero if the _Decimal64 is finite

   Copyright (C) 2008, 2010 IBM Corporation.
   Copyright (C) 2014-2015 Free Software Foundation, Inc.

   Author(s): Pete Eberlein <eberlein@us.ibm.com>
              Ryan S. Arnold <rsa@us.ibm.com>

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
#include <ieee754r_private.h>

int
__isfinited64 (_Decimal64 x)
{
  int cr0;

  asm ("dtstdc cr0,%1,0x38\n"
       "mfcr   %0, 0\n"
    : "=r" (cr0)
    : "f" (x)
    : "cr0");

  return (cr0 & 0x20000000) ? 1 : 0;
}
hidden_def (__isfinited64)
weak_alias (__isfinited64, isfinited64)

/* We erroneously published a version of math.h which used 'finite' instead of
 * 'isfinite' and math.h contained a polymorphic 'isfinite()' function which
 * inlined calles to 'finited*' so we've created aliases for compatability.  */
strong_alias (__isfinited64, finited64)
strong_alias (__isfinited64, __finited64)
