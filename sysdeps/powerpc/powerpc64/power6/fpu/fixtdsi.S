/* Decimal Floating Point convert 128-bit to int.  P6 PowerPC64 version.

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
/* int __dpd_fixtdsi (_Decimal28 x)  */
ENTRY (__dpd_fixtdsi)
	drintnq	0,fp2,fp2,1	/* Preround to toward zero.  */
	dctfixq	fp0,fp2 /* Convert x to integer.  */
	stfd	fp0,-8(r1)
	ori	r1,r1,0		/* Special group ending nop.  */
	oris	r4,0,0x8000 /* 0x0000000080000000  */
	lis	r5,0x8000 /* 0xffffffff80000000  */
	ld	r3,-8(r1)
/* The return is an int, but we have long long, so we need to force overflow
   if there are any significant bits in the high word or overflow (incorrect
   sign) from the low word.  */

	cmpdi	cr6,r3,0
	cmpd	cr0,r3,r4
	cmpd	cr7,r3,r5
	blt	cr6,L(negative)
	bltlr	cr0
L(posoverflow):
	addi	r3,r4,-1	/* return max positive int.  */
	blr
L(negative):		/* If (result >= min negative int */
	bgelr	cr7	/* return result */
L(negoverflow):		/* else */
	mr	r3,r5	/* return min negative int.  */
	blr
	END (__dpd_fixtdsi)
