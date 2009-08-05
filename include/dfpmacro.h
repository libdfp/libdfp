/* Macros that are used as a set of wrappers for function utilities

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2007, 2009 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Joseph Kerian <jkerian@us.ibm.com>

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

/* Necessary to pull in inlined feraiseexcept */
#ifndef __OPTIMIZE__
# define __OPTIMIZE__ 1
#endif
#include <fenv.h>

/* When this file is #include'd, _DECIMAL_SIZE and FUNCTION_NAME
 * _DECIMAL_SIZE : 32, 64, 128 - Used for type definition
 * FUNCTION_NAME : fabs, ceil  - Does not include _DECIMAL_SIZE suffix
 * should already be #define'd */

/* There is a subtle bug that can occur if headers are not included in the
 * right order in d32's *.c files. The order needs to be:
 * #include <decimalX.h>
 * #include <decNumber.h> + decContext.h + decWhatever.h
 * #include "dfpmacro.h"
 *
 * The reason for this is that decNumber sets some type dependent sizes
 * according to values set in decimalX.h. dfptypeconv.h (included below)
 * includes all three of the decimalX.h headers
 */



#if !defined( _DECIMAL_SIZE ) || !defined (FUNCTION_NAME)
#error Including dfpmacro without defining _DECIMAL_SIZE and FUNCTION_NAME is invalid
#endif

#if _DECIMAL_SIZE == 128
  #include <dfptypeconv128.h>
#elif _DECIMAL_SIZE == 64
  #include <dfptypeconv64.h>
#else
  #include <dfptypeconv32.h>
#endif

/* If _IEEE_LIBDFP is defined, errno will not be set when errors occur in
 * math functions */
#ifdef _IEEE_LIBM
# define _IEEE_LIBDFP
#endif

/* Data type to use for the generic function implementations */
#define DEC_TYPE		PASTE(_Decimal,_DECIMAL_SIZE)

/* Use these "functions" for variable preperation */
/* They use functions provided in decimal32/64/128.c and dfptypeconv.c */
#define FUNC_CONVERT_TO_DN(dec,dn)			\
	__DECIMAL_TO_DECNUMBER(dec,dn,_DECIMAL_SIZE)

#define __DECIMAL_TO_DECNUMBER(dec,dn,SIZE)			\
do {								\
	PASTE(decimal,SIZE)	decimal;			\
	PASTE(___host_to_ieee_,SIZE) ((dec), &decimal);		\
	PASTE(PASTE(___decimal,SIZE),ToNumber) (&decimal,(dn));	\
}while (0)

#define FUNC_CONVERT_FROM_DN(dn,dec,context)				\
do {									\
	PASTE(decimal,_DECIMAL_SIZE)	decimal;			\
	PASTE(PASTE(___decimal,_DECIMAL_SIZE),FromNumber)		\
		(&decimal, (dn), (context));				\
	PASTE(PASTE(___ieee_,_DECIMAL_SIZE),_to_host) (&decimal, (dec));\
}while(0)

#define IEEE_DECIMAL_TO_STRING(host, str)				\
do {									\
	PASTE(decimal, _DECIMAL_SIZE)	decimal;			\
	PASTE(___host_to_ieee_,_DECIMAL_SIZE)(host, &decimal);		\
	PASTE(___decimal, PASTE(_DECIMAL_SIZE,ToString))		\
		(&decimal, str);					\
}while(0)

#define IEEE_DECIMAL_TO_ENG_STRING(host, str)				\
do {									\
	PASTE(decimal, _DECIMAL_SIZE)	decimal;			\
	PASTE(___host_to_ieee_,_DECIMAL_SIZE)(host, &decimal);		\
	PASTE(___decimal, PASTE(_DECIMAL_SIZE,ToEngString))		\
		(&decimal, str);						\
}while(0)

#define DEFAULT_CONTEXT		PASTE(DEC_INIT_DECIMAL,_DECIMAL_SIZE)

/* Use this as function internal and external names */
#define EXTERNAL_FUNCTION_NAME	FUNC_D(FUNCTION_NAME)
#define INTERNAL_FUNCTION_NAME	PASTE(__,EXTERNAL_FUNCTION_NAME)
/* For functions that have both errno wrappers and exception throwing
 * implementations, this version is the exception throwing version, while
 * the INTERNAL_FUNCTION_NAME will be the errno wrapper */
#define IEEE_FUNCTION_NAME	PASTE(__ieee743r_,EXTERNAL_FUNCTION_NAME)

/* Functions will need to be prefixed with __bid_ or __dpd_ depending on
 * how GCC was configured. --enable-decimal-float=[bid,dpd] */
#if __DECIMAL_BID_FORMAT__==1
#define PREFIXED_FUNCTION_NAME  PASTE(__bid_,ACTUAL_FUNCTION_NAME)
#else
/* #define PREFIXED_FUNCTION_NAME  PASTE(__dfp_,ACTUAL_FUNCTION_NAME)  */
#define PREFIXED_FUNCTION_NAME  PASTE(__dpd_,ACTUAL_FUNCTION_NAME)
#endif


/* Use this if you need to refer to the type appropriate function
 * elswhere. For example FUNC_D (fabs) (x) may be needed in asin
 * FUNC_D(fabs) (x) will resolve to fabsd32 (x) in the 32 bit version */
#define FUNC_D(x)		PASTE(x,PASTE(d,_DECIMAL_SIZE))


/* These can simply be called, and will check if it's appropriate
 * to throw the exception/errno before doing so */
/* If the compiler/glibc supports math_errhandling, we'll use it
 * in the future */
#define DFP_MATH_ERRHANDLING MATH_ERREXCEPT

#include <errno.h>
#define DFP_ERRNO(the_errno)	*__errno_location() = the_errno
#define DFP_EXCEPT(the_exception)			\
  do {							\
    if((DFP_MATH_ERRHANDLING & MATH_ERREXCEPT) > 0)	\
      feraiseexcept( (the_exception) );			\
  } while(0)


/* Useful for generating constants */
#if _DECIMAL_SIZE == 32
#define DEC_SUFFIX DF
#elif _DECIMAL_SIZE == 64
#define DEC_SUFFIX DD
#elif _DECIMAL_SIZE == 128
#define DEC_SUFFIX DL
#endif
#define DFP_CONSTANT(x)		PASTE(x,DEC_SUFFIX)
#define DFP_MIN			PASTE(__DEC,PASTE(_DECIMAL_SIZE,_MIN__))
#define DFP_EPSILON		PASTE(__DEC,PASTE(_DECIMAL_SIZE,_EPSILON__))
/* Some useful (falsely generic) constants */
#define DFP_HUGE_VAL		PASTE(HUGE_VAL_D,_DECIMAL_SIZE)
#define DFP_NAN			(DEC_TYPE)DEC_NAN

#ifndef PASTE
/* Ideally these shouldn't need to be used elsewhere outside of this file */
# define PASTE(x,y) PASTE2(x,y)
# define PASTE2(x,y) x##y
#endif
