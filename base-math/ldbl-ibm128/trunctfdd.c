/* Handle conversion from binary long double (128) to Decimal64

   Copyright (C) 2007, 2008 IBM Corporation.
   Copyright (C) 2008-2015 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Pete Eberlein <eberlein@us.ibm.com>

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

#define BINARY_TO_DECIMAL
#define SRC 128
#define DEST 64
#define NAME trunc

extern _Decimal64 __extenddfdd(double);

#include "trunctfsd.c"
