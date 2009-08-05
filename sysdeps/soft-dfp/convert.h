/* Handle conversion from binary integers, floats and decimal types
   Copyright (C) 2007,2008 IBM Corporation.

   Author(s): Pete Eberlein <eberlein@us.ibm.com>

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


#include <math.h>
#include <stdlib.h>

/* decimal source */
#if defined DECIMAL_TO_INTEGER || defined DECIMAL_TO_BINARY || defined DECIMAL_TO_DECIMAL

#if SRC==32
# define SRC_TYPE _Decimal32
# define IEEE_SRC_TYPE decimal32
# define SRC_LITERAL(n) n##DF
# define SRC_KIND sd
# define SIGNBIT __signbitd32
# define CLASSIFY __fpclassifyd32
#endif

#if SRC==64
# define SRC_TYPE _Decimal64
# define IEEE_SRC_TYPE decimal64
# define SRC_LITERAL(n) n##DD
# define SRC_KIND dd
# define SIGNBIT __signbitd64
# define CLASSIFY __fpclassifyd64
#endif

#if SRC==128
# define SRC_TYPE _Decimal128
# define IEEE_SRC_TYPE decimal128
# define SRC_LITERAL(n) n##DL
# define SRC_KIND td
# define SIGNBIT __signbitd128
# define CLASSIFY __fpclassifyd128
#endif

extern int SIGNBIT (SRC_TYPE);
extern int CLASSIFY (SRC_TYPE);


#define DECIMAL_SIZE SRC

#endif //DECIMAL_TO_INTEGER || DECIMAL_TO_BINARY || DECIMAL_TO_DECIMAL


/* float source */
#if defined BINARY_TO_DECIMAL

#if SRC==32
#define SRC_TYPE float
#define SRC_LITERAL(n) n##f
#define SRC_KIND sf
#define SIGNBIT __builtin_signbitf
//extern int __dfp_classify_sf (float a);
#define CLASSIFY __dfp_classify_sf
#endif

#if SRC==64
#define SRC_TYPE double
#define SRC_LITERAL(n) n##d
#define SRC_KIND df
#define SIGNBIT __builtin_signbit
//extern int __dfp_classify_df (double a);
#define CLASSIFY __dfp_classify_df
#endif

#if SRC==128
#define SRC_TYPE long double
#define SRC_LITERAL(n) n##l
#define SRC_KIND tf
#define SIGNBIT __builtin_signbitl
//extern int __dfp_classify_df (double a);
/* For IBM 128-bit long double we use the double version.  */
#define CLASSIFY __dfp_classify_df
#endif

extern int SIGNBIT (SRC_TYPE);
extern int CLASSIFY (SRC_TYPE);


#endif // BINARY_TO_DECIMAL


/* integer source */
#if defined INTEGER_TO_DECIMAL

#if SRC==32
#if defined UNSIGNED
#define SRC_TYPE unsigned int
#define INT_FMT "%u"
#define CAST_FOR_FMT(A) (unsigned int)A
#else
#define SRC_TYPE int
#define INT_FMT "%d"
#define CAST_FOR_FMT(A) (int)A
#endif
#define SRC_LITERAL(n) n
#define SRC_KIND si
#endif

#if SRC==64
#if defined UNSIGNED
#define SRC_TYPE unsigned long long
#define INT_FMT "%llu"
#define CAST_FOR_FMT(A) (unsigned long long)A
#else
#define SRC_TYPE long long
#define INT_FMT "%lld"
#define CAST_FOR_FMT(A) (long long)A
#endif
#define SRC_LITERAL(n) n
#define SRC_KIND di
#endif

#endif // INTEGER_TO_DECIMAL



/* decimal dest */
#if defined DECIMAL_TO_DECIMAL || defined INTEGER_TO_DECIMAL || defined BINARY_TO_DECIMAL

#if DEST==32
#define DEST_TYPE _Decimal32
#define IEEE_DEST_TYPE decimal32
#define DEST_LITERAL(n) n##DF
#define DEST_KIND sd
#endif

#if DEST==64
#define DEST_TYPE _Decimal64
#define IEEE_DEST_TYPE decimal64
#define DEST_LITERAL(n) n##DD
#define DEST_KIND dd
#endif

#if DEST==128
#define DEST_TYPE _Decimal128
#define IEEE_DEST_TYPE decimal128
#define DEST_LITERAL(n) n##DL
#define DEST_KIND td
#endif

#define DEST_INFINITY DEC_INFINITY
#define DEST_NAN DEC_NAN

#if defined DECIMAL_SIZE
#if DECIMAL_SIZE < DEST
#undef DECIMAL_SIZE
#define DECIMAL_SIZE DEST
#endif
#endif

#define CONTEXT_INIT PASTE(DEC_INIT_DECIMAL,DEST)

#endif //DECIMAL_TO_DECIMAL || INTEGER_TO_DECIMAL || BINARY_TO_DECIMAL


/* float dest */
#if defined DECIMAL_TO_BINARY

#if DEST==32
#define DEST_TYPE float
#define DEST_LITERAL(n) n##F
#define DEST_KIND sf
#endif

#if DEST==64
#define DEST_TYPE double
#define DEST_LITERAL(n) n
#define DEST_KIND df
#endif

#if DEST==128
#define DEST_TYPE long double
#define DEST_LITERAL(n) n##L
#define DEST_KIND tf
#endif

#define DEST_INFINITY INFINITY
#define DEST_NAN NAN


#endif //DECIMAL_TO_BINARY


/* integer dest */
#if defined DECIMAL_TO_INTEGER

#if DEST==32
#if defined UNSIGNED
#define DEST_TYPE unsigned int
#define STR_TO_INT strtoul
#else
#define DEST_TYPE int
#define STR_TO_INT strtol
#endif
#define DEST_LITERAL(n) n
#define DEST_KIND si
#endif

#if DEST==64
#if defined UNSIGNED
#define DEST_TYPE unsigned long long
#define STR_TO_INT strtoull
#else
#define DEST_TYPE long long
#define STR_TO_INT strtoll
#endif
#define DEST_LITERAL(n) n
#define DEST_KIND di
#endif

#endif //DECIMAL_TO_INTEGER


extern long double binpowof10[];
extern const _Decimal128 decpowof2[];

#define BINPOWOF10 binpowof10
#define BINPOWOF10_LIMIT 308
#define DECPOWOF2 decpowof2

#define FREXPSF		__builtin_frexpf
#define FREXPDF		__builtin_frexp
#define FREXPTF		__builtin_frexpl
#define FREXPD32	__frexpd32
#define FREXPD64	__frexpd64
#define FREXPD128	__frexpd128



#ifndef PASTE
# define PASTE(a,b) PASTE2(a,b)
# define PASTE2(x,y) x##y
#endif
#define PASTE4(a,b,c,d) PASTE(PASTE(a,b),PASTE(c,d))
#define PASTE5(a,b,c,d,e) PASTE(PASTE4(a,b,c,d),e)

#if defined DECIMAL_TO_DECIMAL
#define FUNCTION_NAME PASTE4(NAME,SRC_KIND,DEST_KIND,2)
#else
#define FUNCTION_NAME PASTE4(NAME,SRC_KIND,DEST_KIND,)
#endif

/* Functions will need to be prefixed with __bid_ or __dpd_ depending on 
 * how GCC was configured. --enable-decimal-float=[bid,dpd] */
#if __DECIMAL_BID_FORMAT__==1
#define PREFIXED_FUNCTION_NAME  PASTE(__bid_,FUNCTION_NAME)
#else
/* #define PREFIXED_FUNCTION_NAME  PASTE(__dfp_,FUNCTION_NAME) */
#define PREFIXED_FUNCTION_NAME  PASTE(__dpd_,FUNCTION_NAME)
#endif

#if (SRC == 128 && (defined DECIMAL_TO_DECIMAL || defined DECIMAL_TO_INTEGER)) || \
    (DEST == 128 && (defined DECIMAL_TO_DECIMAL || defined INTEGER_TO_DECIMAL))
#include "decimal128.h"
#include <dfptypeconv128.h>
#endif
#if (SRC == 64 && (defined DECIMAL_TO_DECIMAL || defined DECIMAL_TO_INTEGER)) || \
    (DEST == 64 && (defined DECIMAL_TO_DECIMAL || defined INTEGER_TO_DECIMAL))
#include "decimal64.h"
#include <dfptypeconv64.h>
#endif
#if (SRC == 32 && (defined DECIMAL_TO_DECIMAL || defined DECIMAL_TO_INTEGER)) || \
    (DEST == 32 && (defined DECIMAL_TO_DECIMAL || defined INTEGER_TO_DECIMAL))
#include "decimal32.h"
#include <dfptypeconv32.h>
#endif

#define DFP_EXCEPTIONS_ENABLED 1

#define DFP_IEEE_FLAGS(status) (					\
      (((status) & DEC_IEEE_854_Division_by_zero) ? FE_DIVBYZERO : 0) |	\
      (((status) & DEC_IEEE_854_Inexact) ? FE_INEXACT : 0) |		\
      (((status) & DEC_IEEE_854_Invalid_operation) ? FE_INVALID : 0) |	\
      (((status) & DEC_IEEE_854_Overflow) ? FE_OVERFLOW : 0) |		\
      (((status) & DEC_IEEE_854_Underflow) ? FE_UNDERFLOW : 0))

#include <fenv_libdfp.h>

#ifdef fegetenv_register
# define DFP_TEST_EXCEPTIONS(status)	({	\
	fenv_union_t u; 			\
	u.fenv = fegetenv_register(); 		\
	u.l[1] & (status);			\
	})
# define DFP_CLEAR_EXCEPTIONS(status)	{	\
	fenv_union_t u;				\
	u.fenv = fegetenv_register();		\
	u.l[1] &= ~status;			\
	fesetenv_register(u.fenv);		\
	}
#else
/* Non-register targets might want to use the standard functions.
 * Note that it is necessary to include these symbols in libdfp
 * to avoid libm dependencies.  */
# define DFP_TEST_EXCEPTIONS(status)	fetestexcept (status)
# define DFP_CLEAR_EXCEPTIONS(status)	feclearexcept (status)
#endif

#define DFP_HANDLE_EXCEPTIONS(status)	feraiseexcept(status)

#define CONVERT_WRAPPER(...)			\
DEST_TYPE					\
PREFIXED_FUNCTION_NAME (SRC_TYPE a)		\
{						\
	DEST_TYPE result = DEST_LITERAL(0.);	\
						\
	switch (CLASSIFY (a)) {			\
		case FP_ZERO:			\
			result = SIGNBIT(a) ? DEST_LITERAL(-0.) : DEST_LITERAL(0.);	\
			break;			\
		case FP_INFINITE: 		\
			result = SIGNBIT(a) ? -DEST_INFINITY : DEST_INFINITY;	\
			break;			\
		case FP_NAN:			\
			result = DEST_NAN;	\
			break;			\
		default: {			\
			__VA_ARGS__		\
			}			\
	}					\
	return result;				\
}
