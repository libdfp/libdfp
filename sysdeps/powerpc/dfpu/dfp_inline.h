/* Internal ieee754r function prototypes used by libdfp
   for powerpc

   Copyright (C) 2019 Free Software Foundation, Inc.

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

#ifndef _DFP_INLINE_H
#define _DFP_INLINE_H

inline _Decimal128
___quantized128 (_Decimal128 x, _Decimal128 y)
{
  _Decimal128 result;
  asm ("dquaq %0,%1,%2,3\n\t" : "=f" (result) : "f" (y), "f" (x));
  return result;
};

inline _Decimal64
___quantized64 (_Decimal64 x, _Decimal64 y)
{
  _Decimal64 result;
  asm ("dqua %0,%1,%2,3\n\t" : "=f" (result) : "f" (y), "f" (x));
  return result;
};

#define __quantized64(x, y) ___quantized64(x, y)
#define __quantized128(x, y) ___quantized128(x, y)

#endif
