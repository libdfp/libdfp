/* Convert unsigned int to Decimal Floating Point 128-bit.

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
/* _Decimal128 __dpd_floatunssitd (unsigned int x)  */
ENTRY (__dpd_floatunssitd)
	clrldi	r0,r3,32	/* extend unsigned.  */
	std	r0,-8(r1)
	ori	r1,r1,0		/* Special group ending nop.  */
	lfd	fp0,-8(r1)
	dcffixq	fp2,fp0 /* Convert x to _Decimal128 floatuns.  */
	blr
	END (__dpd_floatunssitd)
