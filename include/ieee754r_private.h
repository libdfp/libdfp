/* Internal ieee754r function prototypes used by libdfp.

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2007, 2009 Free Software Foundation, Inc.

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

   Please see dfp/COPYING.txt for more information.  */

#ifndef _DFP_IEEE754R_PRIVATE_H
#define _DFP_IEEE754R_PRIVATE_H 1
/* Maybe just have libdfp use the __builtin_<is*> functions in the future?  */

extern int __isgreaterd32(_Decimal32 x, _Decimal32 y);
extern int __isgreaterequald32(_Decimal32 x, _Decimal32 y);
extern int __islessd32(_Decimal32 x, _Decimal32 y);
extern int __islessequald32(_Decimal32 x, _Decimal32 y);
extern int __islessgreaterd32(_Decimal32 x, _Decimal32 y);
extern int __isunorderedd32 (_Decimal32 x, _Decimal32 y);

extern int __isgreaterd64(_Decimal64 x, _Decimal64 y);
extern int __isgreaterequald64(_Decimal64 x, _Decimal64 y);
extern int __islessd64(_Decimal64 x, _Decimal64 y);
extern int __islessequald64(_Decimal64 x, _Decimal64 y);
extern int __islessgreaterd64(_Decimal64 x, _Decimal64 y);
extern int __isunorderedd64 (_Decimal64 x, _Decimal64 y);

extern int __isgreaterd128(_Decimal128 x, _Decimal128 y);
extern int __isgreaterequald128(_Decimal128 x, _Decimal128 y);
extern int __islessd128(_Decimal128 x, _Decimal128 y);
extern int __islessequald128(_Decimal128 x, _Decimal128 y);
extern int __islessgreaterd128(_Decimal128 x, _Decimal128 y);
extern int __isunorderedd128 (_Decimal128 x, _Decimal128 y);

extern _Decimal32 __lgamma_rd32 (_Decimal32, int *);
extern _Decimal64 __lgamma_rd64 (_Decimal64, int *);
extern _Decimal128 __lgamma_rd128 (_Decimal128, int *);


extern int __isnormald32 (_Decimal32 x);
extern int __isnormald64 (_Decimal64 x);
extern int __isnormald128 (_Decimal128 x);

#endif /* _IEEE754R_PRIVATE_H  */
