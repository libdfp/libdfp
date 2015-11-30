/* 128b integer constants used for conversions.

   Copyright (C) 2015 Free Software Foundation, Inc.

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

   Please see dfp/COPYING.txt for more information.  */

#define QI(_x) ((__int128)(_x##ULL))
#define ONEe19QI QI(10000000000000000000)

__int128 const exp10_ti[] = {
  1ULL,				/* 1e0 */
  10ULL,
  100ULL,
  1000ULL,
  10000ULL,
  100000ULL,			/* 1e5 */
  1000000ULL,
  10000000ULL,
  100000000ULL,
  1000000000ULL,
  10000000000ULL,		/* 1e10 */
  100000000000ULL,
  1000000000000ULL,
  10000000000000ULL,
  100000000000000ULL,
  1000000000000000ULL,		/* 1e15 */
  10000000000000000ULL,
  100000000000000000ULL,
  1000000000000000000ULL,
  10000000000000000000ULL,
  /* Use the compiler to precompute bigger numbers. */
  QI (10) * ONEe19QI,		/* 1e20 */
  QI (100) * ONEe19QI,
  QI (1000) * ONEe19QI,
  QI (10000) * ONEe19QI,
  QI (100000) * ONEe19QI,
  QI (1000000) * ONEe19QI,
  QI (10000000) * ONEe19QI,
  QI (100000000) * ONEe19QI,
  QI (1000000000) * ONEe19QI,
  QI (10000000000) * ONEe19QI,
  QI (100000000000) * ONEe19QI,	/* 1e30 */
  QI (1000000000000) * ONEe19QI,
  QI (10000000000000) * ONEe19QI,
  QI (100000000000000) * ONEe19QI,
  QI (1000000000000000) * ONEe19QI,
  QI (10000000000000000) * ONEe19QI,	/* 1e35 */
  QI (100000000000000000) * ONEe19QI,
  QI (1000000000000000000) * ONEe19QI,
  QI (10000000000000000000) * ONEe19QI,	/* 1e38 */
};
