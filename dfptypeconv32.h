/* DFP_C_TYPE to/from IEEE DFP type conversion routine prototypes

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2009 Free Software Foundation.

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

   Please see libdfp/COPYING.txt for more information.  */

/*
   The decNumber library utilizes the IEEE 754r proposed types of
   decimal32, decimal64, and decimal128 which are structs of byte
   arrays.  The GLIBC supported types are based upon the ANSI C
   Standards body WG14 1176 Technical Report and are types
   _Decimal32, _Decimal64, _Decimal128.  This header files provides
   prototypes for converting to and from both types.  */

#ifndef _DFP_TYPE_CONV32_H
#define _DFP_TYPE_CONV32_H 1

#include <decimal32.h>

/* These shouldn't be exposed externally.  The users shouldn't
   care about the IEEE 754r types.  */
extern void ___host_to_ieee_32 (_Decimal32 *, decimal32 *);
extern void ___ieee_32_to_host (decimal32 *, _Decimal32 *);

hidden_proto(___host_to_ieee_32)
hidden_proto(___ieee_32_to_host)

#endif /* _DFP_TYPE_CONV32_H  */
