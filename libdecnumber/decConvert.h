/* Conversion routines between _DecimalN types.
   Copyright (C) 2020 Free Software Foundation, Inc.

   This file is basically a wrapper around part of GCC.

   GCC is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 3, or (at your option) any later
   version.

   GCC is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */

#ifndef _DECCONVERT_H
#define _DECCONVERT_H

#include <decNumber.h>

#ifdef __cplusplus
extern "C" {
#endif

decNumber * decimal128ToNumber(const decimal128 *, decNumber *);
decNumber * decimal64ToNumber(const decimal64 *, decNumber *);
decNumber * decimal32ToNumber(const decimal32 *, decNumber *);

_Decimal128 * decimal128FromNumber(_Decimal128 *, const decNumber *,
				    decContext *);
_Decimal64 * decimal64FromNumber(_Decimal64 *, const decNumber *,
				    decContext *);
_Decimal32 * decimal32FromNumber(_Decimal32 *, const decNumber *,
				    decContext *);
#ifdef __cplusplus
}
#endif


#endif /* _DECCONVERT_H */
