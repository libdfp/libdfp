/* Inline floating-point environment handling functions for powerpc.

   Copyright (C) 1995, 1996, 1997, 1998, 1999, 2006, 2009
   Free Software Foundation, Inc.

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



#if defined __GNUC__ && !defined _SOFT_FLOAT && !defined __NO_MATH_INLINES

/* Adapted from GLIBC's powerpc/fpu/bits/fenvinline.h  */

/* NOT USED IN LIBDFP since this comes from elsewhere.  */
/* Inline definition for fegetround.  */
/*# define fegetround() \
  (__extension__  ({ int __fegetround_result;				      \
		     __asm__ __volatile__				      \
		       ("mcrfs 7,7 ; mfcr %0"				      \
			: "=r"(__fegetround_result) : : "cr7");		      \
		     __fegetround_result & 3; }))
     */

/* The weird 'i#*X' constraints on the following suppress a gcc
   warning when __excepts is not a constant.  Otherwise, they mean the
   same as just plain 'i'.  */

/* Inline definition for feraiseexcept.  */
# define feraiseexcept(__excepts) \
  ((__builtin_constant_p (__excepts)					      \
    && ((__excepts) & ((__excepts)-1)) == 0				      \
    && (__excepts) != FE_INVALID)					      \
   ? ((__excepts) != 0							      \
      ? (__extension__ ({ __asm__ __volatile__				      \
			  ("mtfsb1 %s0"					      \
			   : : "i#*X"(__builtin_ffs (__excepts)));	      \
			  0; }))					      \
      : 0)								      \
   : (feraiseexcept) (__excepts))

/* Inline definition for feclearexcept.  */
# define feclearexcept(__excepts) \
  ((__builtin_constant_p (__excepts)					      \
    && ((__excepts) & ((__excepts)-1)) == 0				      \
    && (__excepts) != FE_INVALID)					      \
   ? ((__excepts) != 0							      \
      ? (__extension__ ({ __asm__ __volatile__				      \
			  ("mtfsb0 %s0"					      \
			   : : "i#*X"(__builtin_ffs (__excepts)));	      \
			  0; }))					      \
      : 0)								      \
   : (feclearexcept) (__excepts))

#endif /* __GNUC__ && !_SOFT_FLOAT */
