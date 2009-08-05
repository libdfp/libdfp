/* Convert unsigned long to Decimal Floating Point 64-bit.

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

	.section	".toc","aw"
.LC1:	/* 9223372036854775808.0DL */
	.tc FT_2207c000_3_a4cfa07a_2c7f600a[TC],0x2207c00000000003,0xa4cfa07a2c7f600a
	.section	".text"

	.machine	"power6"
/* _Decimal64 __dpd_floatunsdidd (unsigned long x)       */
/* _Decimal64 __dpd_floatunsdidd (unsigned long long x)  */
ENTRY (__dpd_floatunsdidd)
	cmpdi	cr0,r3,0
	std	r3,-8(r1)
	ori	r1,r1,0		/* Special group ending nop.  */
	lfd	fp0,-8(r1)
	blt	cr0,L(negative)
	dcffixq	fp2,fp0 /* Convert x to _Decimal128 float.  */
	drdpq	fp0,fp2 /* Round result to _Decimal64.  Result in fp0.  */
	fmr	fp1,fp0
	blr
/* we need to do more here. The function converts a unsigned long, but
   the dcffix instruction provides a signed conversion.  So we force
   the binary to be positive by clearing the sign bit before we convert
   to decimal.  Then we add 9223372036854775808.0DL to the result of the
   dcffix.  This gives the decimal equivalent to the full 64-bit value.  */
L(negative):
	fabs	fp0,fp0
	lfd	fp10,.LC1@toc(2)
	lfd	fp11,.LC1@toc+8(2)
	dcffixq	fp2,fp0 /* Convert x to _Decimal128 float.  */
	daddq	fp2,fp2,fp10
	drdpq	fp0,fp2		/* Round result to _Decimal64.  */
	fmr	fp1,fp0
	blr
	END (__dpd_floatunsdidd)
