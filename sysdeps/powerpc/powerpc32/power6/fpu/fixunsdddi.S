/* Decimal Floating Point convert 64-bit to unsigned long.  P6 PPC32 version.

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


	.section	.rodata.cst16,"aM",@progbits,32
	.align	3
.LC1:	/* 9223372036854775808.0DL */
	.long	0x2207c000
	.long	0x00000003
	.long	0xa4cfa07a
	.long	0x2c7f600a
.LC2:	/* 18446744073709551616.0DL */
	.long	0x2207c000
	.long	0x0000000c
	.long	0xa99e40ed
	.long	0xc5ba58e0
	.section	".text"
	.machine	"power6"
/* unsigned long long __dpd_fixunsdddi (_Decimal64 x)  */
ENTRY (__dpd_fixunsdddi)
	stwu	r1,-16(r1)
	cfi_adjust_cfa_offset (16)
#ifdef SHARED
	mflr	r11
	cfi_register(lr,r11)
# ifdef HAVE_ASM_PPC_REL16
	bcl	20,31,1f
1:	mflr	r9
	addis	r9,r9,.LC1-1b@ha
	addi	r9,r9,.LC1-1b@l
# else
	bl	_GLOBAL_OFFSET_TABLE_@local-4
	mflr	r10
	lwz	r9,.LC1@got(10)
# endif
	mtlr	r11
	cfi_same_value (lr)
	lfd	fp10,0(r9)
	lfd	fp11,8(r9)
	lfd	fp8,.LC2-.LC1(r9)
	lfd	fp9,.LC2-.LC1+8(r9)
#else
	lis	r9,.LC1@ha
	lis	r10,.LC2@ha
	addi	r9,r9,.LC1@l
	addi	r10,r10,.LC2@l
	lfd	fp10,0(r9)
	lfd	fp11,8(r9)
	lfd	fp8,0(r10)
	lfd	fp9,8(r10)
#endif
	dsubq	fp6,fp10,fp10   /* Generate 0.0 constant for later.  */
	dctqpq	fp2,fp1		/* Convert param to _Decimal128.  */
	drintnq 0,fp2,fp2,1	/* Preround to toward zero.  */
	dcmpuq  cr1,fp2,fp6     /* Check for less than zero.  */
	dcmpuq	cr0,fp2,fp10	/* check if > 2**63-1 */
	blt	cr1,L(negative)
	ble	cr0,L(poslong)
/* The return is an unsigned long, but "DFP convert to fixed" takes
   signed long, so we need to pre-adjust (-18446744073709551616.0DD)
   to get the final (signed) conversion result to be the correct	
   unsigned result.  */
	dsubq	fp2,fp2,fp8
	dcmpuq	cr0,fp2,fp10	/* if x still > 2**63-1 */
	li	r3,-1		/* return max_unsigned_long */
	bgt	cr0,L(done)
L(poslong):
	dctfixq	fp0,fp2 /* Convert x to integer.  */
	stfd	fp0,8(r1)
	ori	r1,r1,0		/* Special group ending nop.  */
	lwz	r3,8(r1)
	lwz	r4,12(r1)
	addi	r1,r1,16
	blr
L(negative):
	li	r3,0	/* return 0 for negative floats.  */
L(done):
	mr	r4,r3
	addi	r1,r1,16
	blr
	END (__dpd_fixunsdddi)

