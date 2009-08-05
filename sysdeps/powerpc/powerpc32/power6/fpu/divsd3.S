/* Decimal Floating Point 32-bit divide.  POWER6 PowerPC32 version.

   Copyright (C) 2006, 2008, 2009 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Steven J. Munroe  <sjmunroe@us.ibm.com>
              Ryan S. Arnold  <rsa@us.ibm.com>

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

#include <sysdep.h>


	.machine	"power6"
/* _Decimal32 __dpd_divsd3 (_Decimal32 x, _Decimal32 y)  */
ENTRY (__dpd_divsd3)
	dctdp	fp1,fp1	/* Convert x to _Decimal64. */
	dctdp	fp2,fp2	/* Convert y to _Decimal64. */
	ddiv	fp1,fp1,fp2
	drsp	fp1,fp1	/* Round result to _Decimal32. */
	blr
	END (__dpd_divsd3)
