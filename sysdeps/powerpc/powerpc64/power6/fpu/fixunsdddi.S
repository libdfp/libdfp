/* Decimal Floating Point convert 64-bit to unsigned long.  P6 PPC64 version.

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
.LC1:	/* 9223372036854775808.0DD */
	.tc  FT_2207c000_3_a4cfa07a_2c7f600a[TC],0x2207c00000000003,0xa4cfa07a2c7f600a
.LC2:	/* 18446744073709551616.0DD */
	.tc FT_2207c000_c_a99e40ed_c5ba58e0[TC],0x2207c0000000000c,0xa99e40edc5ba58e0
	.section	".text"

	.machine	"power6"
/* unsigned long __dpd_fixunsdddi (_Decimal64 x)       */
/* unsigned long long __dpd_fixunsdddi (_Decimal64 x)  */
ENTRY (__dpd_fixunsdddi)
	dctqpq	fp2,fp1	/* Convert x to _Decimal128.  */
	lfd	fp10,.LC1@toc(2)
	lfd	fp11,.LC1@toc+8(2)
	dsubq	fp6,fp10,fp10	/* Generate 0.0 constant for later.  */
	drintnq	0,fp2,fp2,1	/* Preround to toward zero.  */
	dcmpuq	cr1,fp2,fp6	/* Check for less than zero.  */
	dcmpuq	cr0,fp2,fp10	/* check if > 2**63-1 */
	blt	cr1,L(negative)
	lfd	fp8,.LC2@toc(2)
	lfd	fp9,.LC2@toc+8(2)
	ble	cr0,L(poslong)
/* The return is an unsigned long, but "DFP convert to fixed" takes
   signed long, so we need to pre-adjust (-18446744073709551616.0DD)
   to get the final (signed) conversion result to be the correct	
   unsigned result.  */
	dsubq	fp2,fp2,fp8
	dcmpuq	cr0,fp2,fp10	/* if x still > 2**63-1 */
	li	r3,-1		/* return max_unsigned_long */
	bgelr	cr0		/* else */
L(poslong):
	dctfixq fp0,fp2 /* Convert x to long integer.  */
	stfd	fp0,-8(r1)
	ori	r1,r1,0		/* Special group ending nop.  */
	ld	r3,-8(r1)
	blr
L(negative):
	li	r3,0	/* return 0 for negative floats.  */
	blr
	END (__dpd_fixunsdddi)

