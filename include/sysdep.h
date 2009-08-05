/* Some constructs used by asm files.

   Copyright (C) 2009 IBM Corporation.
   Copyright (C) 2009 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Ryan S. Arnold <rsa@us.ibm.com>

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

   Please see dfp/COPYING.txt for more information.  */

#define cfi_startproc			.cfi_startproc
#define cfi_endproc			.cfi_endproc
#define cfi_def_cfa(reg, off)		.cfi_def_cfa reg, off
#define cfi_def_cfa_register(reg)	.cfi_def_cfa_register reg
#define cfi_def_cfa_offset(off)		.cfi_def_cfa_offset off
#define cfi_adjust_cfa_offset(off)	.cfi_adjust_cfa_offset off
#define cfi_offset(reg, off)		.cfi_offset reg, off
#define cfi_rel_offset(reg, off)	.cfi_rel_offset reg, off
#define cfi_register(r1, r2)		.cfi_register r1, r2
#define cfi_return_column(reg)		.cfi_return_column reg
#define cfi_restore(reg)		.cfi_restore reg
#define cfi_same_value(reg)		.cfi_same_value reg
#define cfi_undefined(reg)		.cfi_undefined reg
#define cfi_remember_state		.cfi_remember_state
#define cfi_restore_state		.cfi_restore_state
#define cfi_window_save			.cfi_window_save

#include_next <sysdep.h>
