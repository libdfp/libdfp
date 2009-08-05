/* s390 fenv macros.

   Copyright (C) 2000, 2008, 2009 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Andreas Krebbel  <Andreas.Krebbel@de.ibm.com>

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

#ifndef _FENV_LIBDFP_H
#define _FENV_LIBDFP_H    1

/* Based on the binary floating point variants contributed
   by Denis Joseph Barrow (djbarrow@de.ibm.com).  */

#include <fenv.h>

/* Definitions from asm/s390-regs-common.h that are needed in glibc.  */


#define FPC_EXCEPTION_MASK	0xF8000000
#define FPC_FLAGS_MASK		0x00F80000
#define FPC_DXC_MASK		0x0000FF00
#define FPC_RM_MASK		0x00000003
#define FPC_DFP_RM_MASK		0x00000070
#define FPC_VALID_MASK		((FPC_EXCEPTION_MASK|FPC_FLAGS_MASK| \
				 FPC_DXC_MASK|FPC_BFP_RM_MASK|FPC_DFP_RM_MASK))

#define FPC_EXCEPTION_MASK_SHIFT	24
#define FPC_FLAGS_SHIFT			16
#define FPC_DXC_SHIFT			8
#define FPC_NOT_FPU_EXCEPTION		0x300

#endif /* _FENV_LIBDFP_H */
