/* Prototype for converting a _Decimal* number into a string

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2007, 2009 Free Software Foundation, Inc.

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

#ifndef _FMT_DFP_H
#define _FMT_DFP_H 1

#include <features.h>
#include <printf.h>

__BEGIN_DECLS

extern char * __fmt_d32 ( const struct printf_info *info,
			  const void *const *args,
			  char * str,
			  int slen);

extern char * __fmt_d64 ( const struct printf_info *info,
			  const void *const *args,
			  char * str,
			  int slen);

extern char * __fmt_d128 ( const struct printf_info *info,
			   const void *const *args,
			   char * str,
			   int slen);

hidden_proto(__fmt_d32)
hidden_proto(__fmt_d64)
hidden_proto(__fmt_d128)

__END_DECLS

#endif
