/* Prototype for getting the number of digits in a _Decimal[32|64|128]

   Copyright (C) 2009 IBM Corporation.
   Copyright (C) 2009 Free Software Foundation, Inc.

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

#ifndef _GET_DIGITS_H
#define _GET_DIGITS_H 1

void __get_digits_d32 (_Decimal32, char*, int*, int*, int *, int *);
void __get_digits_d64 (_Decimal64, char*, int*, int*, int *, int *);
void __get_digits_d128 (_Decimal128, char*, int*, int*, int *, int *);

#endif /* _GET_DIGITS_H  */
