/* Number of digits functions.

   Copyright (C) 2014, Free Software Foundation, Inc.

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

   Please see dfp/COPYING.txt for more information.  */

/* Allow this file to be included more than once, so don't use the usual
   include guards.  */

#define NUMDIGITS_SUPPORT 1

#ifndef DEC_TYPE
#error DEC_TYPE must be declared
#endif

#ifndef _DECIMAL_SIZE
#error _DECIMAL_SIZE must be declared
#endif

#ifndef PASTE
# define PASTE(x,y) PASTE2(x,y)
# define PASTE2(x,y) x##y
#endif

#ifndef FUNC_D
# define FUNC_D(x)		PASTE(x,PASTE(d,_DECIMAL_SIZE))
#endif

static inline int
FUNC_D (getexp) (DEC_TYPE x)
{
  // TODO
  return 0;
}
#define SETEXP_NAME	PASTE(setexp,PASTE(d,_DECIMAL_SIZE))

static inline DEC_TYPE
SETEXP_NAME (DEC_TYPE x, int exp)
{
  // TODO
  return x;
}

static inline DEC_TYPE
FUNC_D (setdigits) (DEC_TYPE x, char *str)
{
  // TODO
  return x;
}

static inline int
FUNC_D (numdigits) (DEC_TYPE x)
{
  // TODO
  return 0;
}

static inline DEC_TYPE
FUNC_D (left_justify) (DEC_TYPE x)
{
  // TODO
  return x;
}
