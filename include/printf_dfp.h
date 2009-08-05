/* Function prototypes to convert DFP values to strings

   Copyright (C) 2006, 2009 IBM Corporation.
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

#ifndef	_PRINTF_DFP_H

#define	_PRINTF_DFP_H	1
#include <printf.h>

#define DFP_FLAG_DECIMAL_32	(1<<16)
#define DFP_FLAG_DECIMAL_64	(1<<17)
#define DFP_FLAG_DECIMAL_128	(1<<18)

/* Internal entry point for formatting DFP.  */
extern int __printf_dfp (FILE *, const struct printf_info *,
			 const void *const *);

hidden_proto (__printf_dfp)

extern void __d128_va (void *mem, va_list *ap);
hidden_proto (__d128_va)
extern void __d64_va (void *mem, va_list *ap);
hidden_proto (__d64_va)
extern void __d32_va (void *mem, va_list *ap);
hidden_proto (__d32_va)

extern int __d128_ais (const struct printf_info *info, size_t n, int *argtype, int *size);
hidden_proto (__d128_ais)
extern int __d64_ais (const struct printf_info *info, size_t n, int *argtype, int *size);
hidden_proto (__d64_ais)
extern int __d32_ais (const struct printf_info *info, size_t n, int *argtype, int *size);
hidden_proto (__d32_ais)

extern int __register_printf_dfp (void);
hidden_proto(__register_printf_dfp)

#endif
