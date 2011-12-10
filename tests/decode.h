/* Prototypes for decode functions, which output the DPD declets.

   Copyright (C) 2009, 2010 Free Software Foundation, Inc.

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

#ifndef _DFP_DECODE_H
#define _DFP_DECODE_H

#include <features.h>

__BEGIN_DECLS

/* char * should ref a 14 byte char array, +0,000,000E+0\0  */
extern char * decoded32 (_Decimal32, char*);
/* char * should ref a 26 byte char array, +0,000,000,000,000,000E+0\0  */
extern char * decoded64 (_Decimal64, char*);
/* char * should ref a 50 byte char array, *
 * +0,000,000,000,000,000,000,000,000,000,000,000E+0\0  */
extern char * decoded128 (_Decimal128, char*);

__END_DECLS

#endif /* _DFP_DECODE_H  */
