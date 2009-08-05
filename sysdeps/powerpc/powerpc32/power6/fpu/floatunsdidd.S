/* Convert unsigned int to Decimal Floating Point 64-bit.

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


	.section        .rodata.cst16,"aM",@progbits,16
	.align  3
.LC1:   /* 18446744073709551616.0DL (ULLONG_MAX) */
	.long   0x2207c000
	.long   0x0000000c
	.long   0xa99e40ed
	.long   0xc5ba58e0
	.section        ".text"
	.machine	"power6"
/* _Decimal64 __dpd_floatunsdidd (unsigned long long x)  */
ENTRY (__dpd_floatunsdidd)
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
	lfd	fp8,0(r9)
	lfd	fp9,8(r9)
#else
	lis	r9,.LC1@ha
	addi	r9,r9,.LC1@l
	lfd	fp8,0(r9)
	lfd	fp9,8(r9)
#endif
	stw	r3,8(r1)
	stw	r4,12(r1)	/* ULL passed in consecutive gprs.  */
	ori	r1,r1,0		/* Special group ending nop.  */
	lfd	fp0,8(r1)
/* Since dcffixq requires 'signed' input parameters if the value of the r3-r4
   pair is an unsigned long long between ULLONG_MAX and LLONG_MAX then the
   value is converted to a negative signed  _Decimal128 and needs to be
   adjusted to get the unsigned equivalent.  */
	dcffixq	fp2,fp0		/* Convert signed x to _Decimal128.  */
/* Set cr0 if r3 is negative (where unsigned > LLONG_MAX).  */
	mr.	r3,r3		
	bge	cr0,L(noadjust)	/* No adjust if orig wasn't neg (unsigned).  */
	daddq	fp2,fp2,fp8	/* Unbias to get unsigned result.  */
L(noadjust):
	drdpq	fp0,fp2		/* Round result to __dpd_Decimal64.  */
	fmr	fp1,fp0
	addi	r1,r1,16
	blr
	END (__dpd_floatunsdidd)
