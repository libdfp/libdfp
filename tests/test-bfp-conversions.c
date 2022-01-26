/* Test bfp -> dfp and dfp -> bfp conversions

   Copyright (C) 2011-2015 Free Software Foundation, Inc.

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

/* Notes for future readers. GCC internally maps various C float types
   to "modes".  Most of the time, this is sensible, sometimes is it
   not.

   DF is generally always _Float64.
   XF is generally always the Intel 96 long double format.
   SF is generally always _Float32.

   KF is only exists on ppc64le, and is _Float128.

   TF is a crapshoot. on x86, it is _Float128.
                      on ppc64{,le}, it is ibm128 format.
                      on others... maybe something else yet.

   For a given target, we need to test the cross product of
   {*F modes} and {SD,DD,TD modes}. */

#ifndef __STDC_WANT_DEC_FP__
#define __STDC_WANT_DEC_FP__
#endif

/* For _Float128 support.  */
#ifndef __STDC_WANT_IEC_60559_TYPES_EXT__
#define __STDC_WANT_IEC_60559_TYPES_EXT__
#endif

#define _WANT_VC 1 /* Pick up the _VC_P(x,y,fmt) macro.  */
#include "scaffold.c" /* Pick up the _VC_P(x,y,fmt) macro.  */

/* Inspired by GLIBC stdio-common/tfformat.c  */

#include <stdlib.h>

#if defined __i386__ || defined __x86_64__
# define HAVE_XFMODE 1
# define TF_SUF(x) x ## f128
#elif defined (__powerpc64__) || defined (__powerpc__)
# if defined _ARCH_PWR8 && defined __LITTLE_ENDIAN__ && (_CALL_ELF == 2)
#  define HAVE_KFMODE 1
# endif
# if __LDBL_MANT_DIG__ == 106
#  define TF_MODE_IBM128 1
#  define TF_MODE_FLT128 0
# endif
#endif

#ifndef TF_MODE_IBM128
#define TF_MODE_FLT128 1
#define TF_MODE_IBM128 0
#endif

#ifndef HAVE_XFMODE
#define HAVE_XFMODE 0
#endif

#ifndef HAVE_KFMODE
#define HAVE_KFMODE 0
#endif

#ifndef TF_SUF
#define TF_SUF(x) x ## L
#endif

/* Make the mess of TFmode slightly more readable here.  */
#define TEST_TF(x,y,...) TEST(TF_SUF (x), y, ##__VA_ARGS__)
#define TEST2_TF(x,y,...) TEST(x, TF_SUF (y), ##__VA_ARGS__)

/* Note, we special case IBM128 here because it is truly
   a special case. */

/* Special flags */
#define F_NONE (0)
#define F_TF_IBM128 (1<<0)
#define F_TF_FLT128 (1<<1)
/* This is painful, but GCC won't parse variable length
   ibm128 values. */
#define F_IBM128_SPECIAL (1<<2)

#define _DECL_STRUCT(from, to, ...) \
	struct from ## to ## _tests {			\
	  int line;					\
	  float in __attribute__((mode (from)));	\
	  float out __attribute__((mode (to)));		\
	  const char *strname;				\
	  struct {					\
	    int flags;					\
	    double ibm128[2];				\
	  } extra;					\
	} from ## to[] = {				\
	    __VA_ARGS__					\
	    {0}						\
	  };

#define DECL_STRUCT(from, to, ...) _DECL_STRUCT (from, to, __VA_ARGS__)

/* SF -> {S,D,T}D mode tests */

#define TEST(x,y,...) _TEST(x, y,  ##__VA_ARGS__)
#define _TEST(x,y,...) {__LINE__, x, y, #y " = (%s) " #x "\n", {__VA_ARGS__}}

DECL_STRUCT(SF, SD,
  TEST(12.345, 12.345DF),
  TEST(12.345678, 12.345678DF),
  TEST(123456.7, 123456.7DF),
  TEST(999999.9, 999999.9DF),
  TEST(2.0, 2.0DF),
  TEST(30.0, 30.0DF),
  TEST(1.0e-20, 1.0e-20DF),
  TEST(1.0e-96, 0.0DF),
  TEST(3.402819e+38, 3.402819e+38DF),
  TEST(100000., 100000.0DF),
  TEST(999999., 999999.0DF),
  TEST(1.2e-38, 1.2e-38DF),
  TEST(1.1e-38, 1.1e-38DF),
  TEST(-1.2e-38, -1.2e-38DF),
)


DECL_STRUCT(SF, DD,
  TEST(12.345, 12.345000267028809DD),
  TEST(30.0, 30.0DD),
  TEST(3.402819e+38f, 3.402819004255176e+38DD),
)

DECL_STRUCT(SF, TD,
  TEST(12.345, 12.3450002670288085937500000000000000DL),
  TEST(30.0, 30.0DL),
  TEST(3.402819e+38f, 3.402819004255175794749548566449886e+38DL),
)

DECL_STRUCT(DF, DD,
  TEST(12.3456789, 12.3456789DD),
  TEST(-2.0, -2.0DD),
  TEST(1.79768e+308, 1.79768e+308DD),
  TEST(100000000000000, 100000000000000.0DD),
  TEST(999999999999999, 999999999999999.0DD),
)

DECL_STRUCT(DF, TD,
  TEST(12.3456789, 12.3456788999999993450273905182257295DL),
  TEST(-2.0, -2.0DL),
  TEST(1.79768e+308, 1.797679999999999908416854000763986e+308DL),
)

DECL_STRUCT(SD, DF,
  TEST(12.345DF, 12.345),
  TEST(2.0DF, 2.0),
  TEST(3.402819e+38DF, 3.402819e+38),
  TEST(9.999998e96DF, 9.999998e96),
  TEST(100000.0DF, 100000),
  TEST(999999.0DF, 999999),
  TEST(1.e-39DF, 1.e-39),
  TEST(-1.e-39DF, -1.e-39),
  TEST(1.2e-38DF, 1.2e-38),
  TEST(1.e-40DF, 1.e-40),
  TEST(8.e-44DF, 8.e-44),
  TEST(9.e-44DF, 9.e-44),
  TEST(8.e-46DF, 8.e-46),
  TEST(-9.e-44DF, -9.e-44),
  TEST(-8.e-46DF, -8.e-46),
);

DECL_STRUCT(DD, DF,
  TEST(-7.0DD, -7.0),
  TEST(3.402819e+38DD, 3.402819e+38),
  TEST(1.79768e+308DD, 1.79768e+308),
  TEST(100000000000000.0DD, 100000000000000),
  TEST(999999999999999.0DD, 999999999999999),
  TEST(1.e290DD, 1.e290),
  TEST(1.e291DD, 1.e291),
  TEST(1.e308DD, 1.e308),
  TEST(1.e-308DD, 1.e-308),
  TEST(1.e-291DD, 1.e-291),
  TEST(1.e-290DD, 1.e-290),
  TEST(1.e-39DD, 1.e-39),
  TEST(-1.e-39DD, -1.e-39),
)

#if HAVE_XFMODE
DECL_STRUCT(SD, XF,
  TEST(12.345DF, 12.345000000000000000242861286636753L),
  TEST(2.0DF, 2.0L),
)

DECL_STRUCT(DD, XF,
  TEST(12.3456789DD, 12.345678899999999999885502699470408L),
  TEST(-7.0DD, -7.0L),
)

DECL_STRUCT(XF, TD,
  TEST(0xc.587e69926ad1p0L, 1.234567889999999934502739051822572947e1DL),
  TEST(-22.0L, -22.0DL),
  TEST(100000000000000000, 100000000000000000.0DL),
  TEST(999999999999999999, 999999999999999999.0DL),
  TEST(100000000000000000000545460846592.0L, 100000000000000000000545460846592.0DL),
  TEST(999999999999999999970270236377088.0L, 999999999999999999970270236377088.0DL),
);
#endif

#define TFTD_TESTS \
  TEST_TF(0xc.587e69926ad1p0, 12.3456788999999993450273905182257295DL), \
  TEST_TF(-22.0, -22.0DL), \
  TEST_TF(100000000000000000.0, 100000000000000000.0DL), \
  TEST_TF(999999999999999999.0, 999999999999999999.0DL), \
  TEST_TF(100000000000000000000000000000000.0, 100000000000000000000000000000000.0DL), \
  TEST_TF(999999999999999999999999999999999.0, 1000000000000000000000000000000000.0DL, F_TF_IBM128, {}), \
  TEST_TF(999999999999999999999999999999999.0,  999999999999999999999999999999999.0DL, F_TF_FLT128, {}),

DECL_STRUCT(TF, TD, TFTD_TESTS)

#define SDTF_TESTS \
  TEST2_TF(12.345DF, 12.345, F_TF_FLT128, {}), \
  TEST2_TF(12.345DF, 0x1.8b0a3d70a3d71p3, F_TF_IBM128, {}), \
  TEST2_TF(2.0DF, 2.0),

DECL_STRUCT(SD, TF, SDTF_TESTS)

#define DDTF_TESTS \
  TEST2_TF(12.3456789DD, 12.3456789, F_TF_FLT128, {}), \
  TEST2_TF(12.3456789DD, 0x1.8b0fcd324d5a2p3, F_TF_IBM128, {}), \
  TEST2_TF(-7.0DD, -7.0),

DECL_STRUCT(DD, TF, DDTF_TESTS)

#define TDTF_TESTS \
  TEST2_TF(30.0DL, 30.0), \
  TEST2_TF(0.00048828125DL, 0.00048828125), \
  TEST2_TF(2.98023223876953125E-8DL, 2.98023223876953125E-8), \
  TEST2_TF(3.402819e+38DL, 3.402819e+38), \
  TEST2_TF(100000000000000000.0DL, 100000000000000000.0), \
  TEST2_TF(999999999999999999.0DL, 999999999999999999.0), \
  TEST2_TF(100000000000000000000000000000000.0DL, 100000000000000000000000000000000.0), \
  TEST2_TF(999999999999999999999999999999999.0DL, 999999999999999999999999999999999.0), \
  TEST2_TF(1.e290DL, 1.e290), \
  TEST2_TF(1.e-290DL, 1.e-290, F_IBM128_SPECIAL, {1.0000000000000001e-290, -6.9127868599625479e-307}), \
  TEST2_TF(1.e-308DL, 1.e-308), \
  TEST2_TF(1.e-291DL, 1.e-291, F_IBM128_SPECIAL, {9.9999999999999996e-292, 3.7675676608720188e-308}), \
  TEST2_TF(3.e-309DL, 3.e-309), \
  TEST2_TF(-3.e-309DL, -3.e-309),

DECL_STRUCT(TD, TF, TDTF_TESTS)


#if __GNUC__ <= 4 && __GNUC_MINOR <= 8
/* This does not support _Float128 or non-trivial long double. */
#define __dfp_fmt_b(x) \
     (sizeof (x) == sizeof (float) \
      ? "float" \
      : sizeof (x) == sizeof (double) \
	? "double" \
	: "long double")

#define __dfp_fmt_d(x) \
     (sizeof (x) == sizeof (_Decimal32) \
      ? "_Decimal32" \
      : sizeof (x) == sizeof (_Decimal64) \
	? "_Decimal64" \
	: "_Decimal128")

#define FORMAT_NAME(x) \
	(__dfp_compatible(x) ? __dfp_fmt_d(x) : __dfp_fmt_b(x))

#define __dfp_dec_b(x,y) \
     (sizeof (x) == sizeof (float) \
      ? decodebin32(x,y) \
      : sizeof (x) == sizeof (double) \
	? decodebin64(x,y) \
	: decodebinl(x,y))

#define __dfp_dec_d(x,y) \
     (sizeof (x) == sizeof (_Decimal32) \
      ? decoded32(x,y) \
      : sizeof (x) == sizeof (_Decimal64) \
	? decoded64(x,y) \
	: decoded128(x,y))

#define DECODE(x,y) \
	(__dfp_compatible(x) ? __dfp_dec_d(x,y) : __dfp_dec_b(x,y))

/* strfroml/strfromd do not exist here either. */
static char * decodebin32(float d, char *c)      { sprintf(c,"%.6a",d); return c;}
static char * decodebin64(double d, char *c)     { sprintf(c,"%.14a",d); return c;}
static char * decodebinl(long double d, char *c) { sprintf(c,"%.29La",d); return c;}

#else

/* ppc/ppc64 do no support _Float128. */
#if TF_MODE_FLT128 || HAVE_KFMODE
# define HAVE_FLT128(x) x,
inline char * decodebin128(_Float128 d, char *c) { strfromf128(c,128,"%.29a",d); return c;}
#else
# define HAVE_FLT128(x)
#endif

#define FORMAT_NAME(x) \
	_Generic((x), \
		float: "float", \
		double: "double", \
		long double: "long double", \
		HAVE_FLT128(_Float128: "_Float128") \
		_Decimal32: "_Decimal32", \
		_Decimal64: "_Decimal64", \
		_Decimal128: "_Decimal128")

#define DECODE(x,y) \
	_Generic((x), \
		float: decodebin32, \
		double: decodebin64, \
		long double: decodebinl, \
		HAVE_FLT128(_Float128: decodebin128) \
		_Decimal32: decoded32, \
		_Decimal64: decoded64, \
		_Decimal128: decoded128)(x,y)

inline char * decodebin32(float d, char *c)      { strfromf(c,128,"%.6a",d); return c;}
inline char * decodebin64(double d, char *c)     { strfromd(c,128,"%.14a",d); return c;}
inline char * decodebinl(long double d, char *c) { strfroml(c,128,"%.29a",d); return c;}
#endif

#include "decode.h"

/* Technically part of TS 18661-2.  This is probably not a compliant implementation.  */

/* Replace snprintf with DECODE. _Float128 support *printf */
#undef _VC_P
#define _VC_P(f,l,x,y,fmt) do { \
  ++testnum;                                                    \
  memset(bufx,'\0',CHAR_MAX);                                   \
  memset(bufy,'\0',CHAR_MAX);                                   \
  DECODE(x, bufx);                                              \
  DECODE(y, bufy);                                              \
  if ((isnan(x) && isnan(y)) ||                                 \
      (x == y))                                                 \
    {                                                           \
      fprintf (stdout, "%-3d Success: Expected: \"%s\"\n",      \
        testnum, bufx);                                         \
      fprintf (stdout, "             Result:   \"%s\"\n",       \
        bufy);                                                  \
      fprintf (stdout, "in: %s:%i\n\n", f, l);                  \
    }                                                           \
  else                                                          \
    {                                                           \
      fprintf (stderr, "%-3d Error:   Expected: \"%s\"\n",      \
        testnum, bufx);                                         \
      fprintf (stderr, "             Result:   \"%s\"\n",       \
        bufy);                                                  \
      fprintf (stderr, "in: %s:%i\n\n", f, l);                  \
      ++fail;                                                   \
    }                                                           \
} while (0)

#define CONTINUE_IF(cond) if (cond) continue;

/* Fixup variable precision IBM128 values.  Whether these should
   be allowed or not is debatable, but is the current behavior. */
#if TF_MODE_IBM128
# define REWRITE_IBM128_EXPECTED(tp) \
  if(tp->extra.flags & F_IBM128_SPECIAL) \
    tp->out = __builtin_pack_longdouble (tp->extra.ibm128[0], \
					 tp->extra.ibm128[1]);
#else
# define REWRITE_IBM128_EXPECTED(tp)
#endif

#define DO_TEST(from, to, func) _DO_TEST(from, to, func)
#define _DO_TEST(from, to, func) \
  for (int i = 0; (from ## to)[i].line; i++) \
    { \
      __typeof(&((from ## to)[0])) tp = &(from ## to)[i]; \
      CONTINUE_IF(!TF_MODE_IBM128 && (tp->extra.flags & F_TF_IBM128)); \
      CONTINUE_IF(!TF_MODE_FLT128 && (tp->extra.flags & F_TF_FLT128)); \
      REWRITE_IBM128_EXPECTED(tp); \
      __typeof(tp->out) retval = func (tp->in); \
      fprintf(stdout, tp->strname, FORMAT_NAME(tp->out)); \
      _VC_P(__FILE__,tp->line, tp->out, retval, "%e"); \
    }

int main (void)
{
  DO_TEST(SD,DF,);
  DO_TEST(DD,DF,);
  DO_TEST(TF,TD,);

  DO_TEST(SD,TF,);
  DO_TEST(DD,TF,);
  DO_TEST(TD,TF,);

#if  HAVE_XFMODE
  DO_TEST(SD,XF,);
  DO_TEST(DD,XF,);
  DO_TEST(XF,TD,);
#endif

/* KFmode is kludgey.  Just rename and redeclare the KFmode bits
   as if they were float128.  */
#if HAVE_KFMODE
  #undef TF_MODE_IBM128
  #undef TF_MODE_FLT128
  #undef TF_SUF
  #undef REWRITE_IBM128_EXPECTED
  #define TF_MODE_IBM128 0
  #define TF_MODE_FLT128 1
  #define TF_SUF(x) x ## f128
  #define REWRITE_IBM128_EXPECTED(tp)
  DECL_STRUCT(KF, TD, TFTD_TESTS)
  DECL_STRUCT(SD, KF, SDTF_TESTS)
  DECL_STRUCT(DD, KF, DDTF_TESTS)
  DECL_STRUCT(TD, KF, TDTF_TESTS)

  DO_TEST(KF,TD,);
  DO_TEST(SD,KF,);
  DO_TEST(DD,KF,);
  DO_TEST(TD,KF,);
#endif

  _REPORT();
  /* fail comes from scaffold.c  */
  return fail;
}
