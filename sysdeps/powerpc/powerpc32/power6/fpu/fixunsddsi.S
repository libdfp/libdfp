/* Decimal Floating Point convert 64-bit to unsigned int.

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
/* unsigned int __dpd_fixunsddsi (_Decimal64 x)  */
/* unsigned long __dpd_fixunsddsi (_Decimal64 x)  */
ENTRY (__dpd_fixunsddsi)
	stwu	r1,-16(r1)
	cfi_adjust_cfa_offset (16)
	drintn 	0,fp1,fp1,1	/* Preround to toward zero.  */
	dctfix	fp0,fp1 /* Convert x to integer.  */
	stfd	fp0,8(r1)
	ori	r1,r1,0		/* Special group ending nop.  */
	lwz	r0,8(r1)
	lwz	r3,12(r1)
/* The return is an int, but we have long long, so we need to force overflow
   if there are any significant bits in the high word or overflow (incorrect
   sign) from the low word.  */
	cmpwi	cr0,r0,0
	cmpli	cr6,r3,0 /* Logical form to cmp unsigned.  */
	blt	cr0,L(negative)
	bne	cr0,L(posoverflow)
	bge	cr6,L(done)
L(posoverflow):
	li	r3,-1	/* return max unsigned int.  */
	b	L(done)
L(negative):
	lis	r3,0	/* return 0 for negative floats.  */
L(done):
	addi	r1,r1,16
	blr
	END (__dpd_fixunsddsi)
