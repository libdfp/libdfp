/* Generic implementations of conversion helper routines.

   Copyright (C) 2016 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Paul E. Murphy <murphyp@linux.vnet.ibm.com>

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

#ifndef _CONVERTHELPERS
#define _CONVERTHELPERS

_Decimal64
combine_and_truncd64 (_Decimal128 hi, _Decimal128 mid, _Decimal128 low);

_Decimal32
combine_and_truncd32 (_Decimal128 hi, _Decimal128 mid, _Decimal128 low);

/* This is a simplified version of frexpd* which instead
   returns frexpd*(x) * 10^num_digs.  mp is assumed to be
   10^num_digs.  */
#define GETMANTANDEXP(SIZE)				  \
static inline _Decimal ## SIZE				  \
getmantandexpd ## SIZE (_Decimal ## SIZE x,		  \
			int *y,				  \
			int shift __attribute((unused)),  \
			_Decimal ## SIZE mp)		  \
{							  \
  _Decimal ## SIZE result = __frexpd ## SIZE (x,y);	  \
  return result * mp;					  \
}

GETMANTANDEXP(32)
GETMANTANDEXP(64)
GETMANTANDEXP(128)


#endif /* _CONVERTHELPERS */
