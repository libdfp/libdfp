/* Decimal Floating Point convert 32-bit to unsigned int.

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
/* unsigned int __dpd_fixunssdsi (_Decimal32 x)  */
ENTRY (__dpd_fixunssdsi)
	dctdp	fp1,fp1	/* Convert x to _Decimal64.  */
	drintn	0,fp1,fp1,1	/* Preround to toward zero.  */
	dctfix	fp0,fp1 /* Convert x to integer.  */
	stfd	fp0,-8(r1)
	ori	r1,r1,0		/* Special group ending nop.  */
	oris	r4,0,0xffff /* 0x00000000ffff0000  */
	ori	r4,r4,0xffff /* 0x00000000ffffffff  */
	ld	r3,-8(r1)
/* The return is an unsigned int, but we have long long, so we need to force
   overflow if there are any significant bits in the high word or return
   zero if negative.  */
	cmpld	cr6,r3,r4
	blt	cr1,L(negative)
	blelr	cr6
L(posoverflow):
	mr	r3,r4	/* return max unsigned int.  */
	blr
L(negative):
	li	r3,0	/* return 0 for negative floats.  */
	blr
	END (__dpd_fixunssdsi)
