/* Test the locale radix character handling of strtod[32|64|128] and
   wcstod[32|64|128].

   This file is part of the Decimal Floating Point C Library.

   The Decimal Floating Point C Library is free software; you can
   redistribute it and/or modify it under the terms of the GNU Lesser
   General Public License version 2.1.

   The Decimal Floating Point C Library is distributed in the hope that
   it will be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
   the GNU Lesser General Public License version 2.1 for more details.

   You should have received a copy of the GNU Lesser General Public
   License version 2.1 along with the Decimal Floating Point C Library;
   if not, write to the Free Software Foundation, Inc., 51 Franklin
   Street, Fifth Floor, Boston, MA 02110-1301 USA.

   Please see libdfp/COPYING.txt for more information.  */

/* strtodN() finds the radix character with nl_langinfo(RADIXCHAR) and matches
   it against the input one byte at a time, whereas wcstodN() matches the
   single wide character from _NL_NUMERIC_DECIMAL_POINT_WC.  Those byte-wise
   comparisons, and the pointer adjustments which step over the radix
   character, are only observable in a locale whose radix character is longer
   than one byte.

   test-strtod.c already covers parsing, rounding, errno and endptr in the C
   locale.  This test adds only inputs whose outcome depends on the radix
   character, one per distinct path through the parser, run in a locale whose
   radix character is "." (C), a single byte which is not "." (de_DE), and a
   multibyte UTF-8 sequence (ps_AF).  */

#ifndef __STDC_WANT_DEC_FP__
# define __STDC_WANT_DEC_FP__ 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <math.h>
#include <locale.h>
#include <langinfo.h>

#define _WANT_VC 1

#include "scaffold.c"

/* U+066B ARABIC DECIMAL SEPARATOR, the radix character of the ps_AF locale.
   It occupies two bytes, 0xd9 0xab, in UTF-8.  */
#define RADIX	"٫"
/* Its leading byte on its own.  That is not a valid multibyte character, so
   an input containing it has no wide-character form.  */
#define RADIX_HI "\xd9"

/* Give both the multibyte and the wide-character form of an input.  The empty
   wide literal widens the concatenation without needing the input to be a
   single token, so that RADIX may appear anywhere in it.  */
#define INPUT(s)	s, L"" s
/* ... for an input which is not a valid multibyte string.  */
#define INPUT_MB(s)	s, NULL

#define LOC_C	"C"
#define LOC_DE	"de_DE.UTF-8"
#define LOC_PS	"ps_AF.UTF-8"
#define LOC_TR	"tr_TR.UTF-8"

typedef enum {
  TEST_D32 = 1 << 0,
  TEST_D64 = 1 << 1,
  TEST_D128 = 1 << 2,
#define TEST_ALL (TEST_D32 | TEST_D64 | TEST_D128)
} test_type_flags;

typedef struct {
  int line;
  const char *locale;	/* Installed with setlocale (LC_ALL, ...).  */
  const char *input;
  const wchar_t *winput;	/* NULL if there is no wide-character form.  */
  size_t nbytes;	/* Bytes strtodN is expected to consume.  */
  size_t nwchars;	/* Wide characters wcstodN is expected to consume.  */
  _Decimal128 expect;	/* Expected value.  Unless an entry is restricted to a
			   single width it is exactly representable as a
			   _Decimal32, so all three widths share it.  */
  int qexp;		/* Expected quantum exponent.  Ignored if EXPECT is
			   not finite.  */
  test_type_flags types;
} strtod_locale_test;

strtod_locale_test tests[] =
{
  /* The C locale, for reference: "." is the radix character, and U+066B is
     just a character (a pair of stray bytes, for strtodN) which ends the
     number.  */
  {__LINE__, LOC_C, INPUT ("1.25"), 4, 4, 1.25DL, -2, TEST_ALL},
  {__LINE__, LOC_C, INPUT ("1" RADIX "25"), 1, 1, 1.DL, 0, TEST_ALL},

  /* A single-byte radix character which is not ".".  */
  {__LINE__, LOC_DE, INPUT ("1,25"), 4, 4, 1.25DL, -2, TEST_ALL},
  {__LINE__, LOC_DE, INPUT ("1.25"), 1, 1, 1.DL, 0, TEST_ALL},

  /* A multibyte radix character.  Integer digits, radix, fractional digits:
     the radix is matched after the integer part, and is later stepped over in
     the middle of the coefficient by the digit accumulation loop.  */
  {__LINE__, LOC_PS, INPUT ("1" RADIX "25"), 5, 4, 1.25DL, -2, TEST_ALL},

  /* A leading radix character is accepted only when a digit follows it, which
     is decided by looking decimal_len bytes ahead before any digit has been
     read ...  */
  {__LINE__, LOC_PS, INPUT (RADIX "25"), 4, 3, 0.25DL, -2, TEST_ALL},
  /* ... and otherwise no conversion is performed, which must leave endptr at
     the start of the string rather than after the radix character.  */
  {__LINE__, LOC_PS, INPUT (RADIX "e5"), 0, 0, 0.DL, 0, TEST_ALL},

  /* A trailing radix character with no fractional digits ("american style"):
     endptr has to advance over the whole radix character.  */
  {__LINE__, LOC_PS, INPUT ("12" RADIX), 4, 3, 12.DL, 0, TEST_ALL},

  /* No integer digits and a zero fraction.  The parser rescans the string for
     the radix character to find the first significant digit; check the sign
     of the resulting zero while here.  */
  {__LINE__, LOC_PS, INPUT ("-" RADIX "0"), 4, 3, -0.0DL, -1, TEST_ALL},

  /* The same rescan with leading fractional zeroes, which places the start of
     the coefficient at radix position + leading zeroes + decimal_len; a wrong
     length shifts the coefficient.  The number of significant digits is
     exactly _Decimal32's precision, so no rounding is involved.  */
  {__LINE__, LOC_PS, INPUT ("0" RADIX "001234567"), 12, 11, 1.234567E-3DL, -9,
   TEST_ALL},

  /* Radix character directly followed by an exponent.  */
  {__LINE__, LOC_PS, INPUT ("1" RADIX "5e2"), 6, 5, 1.5E2DL, 1, TEST_ALL},

  /* A second radix character ends the number, with endptr on its first byte.  */
  {__LINE__, LOC_PS, INPUT ("1" RADIX "2" RADIX "3"), 4, 3, 1.2DL, -1, TEST_ALL},

  /* "." is an ordinary character here, and ends the number.  */
  {__LINE__, LOC_PS, INPUT ("1.25"), 1, 1, 1.DL, 0, TEST_ALL},

  /* The digit accumulation loop steps over the radix character by testing
     each byte for "not a digit", and must do so without disturbing the count
     of digits read.  Placing the radix character exactly at _Decimal32's
     precision boundary makes that skip happen between the last coefficient
     digit and the rounding digit.  The wider formats hold the value
     exactly.  */
  {__LINE__, LOC_PS, INPUT ("1234567" RADIX "89"), 11, 10, 1234568.DL, 0,
   TEST_D32},
  {__LINE__, LOC_PS, INPUT ("1234567" RADIX "89"), 11, 10, 1234567.89DL, -2,
   TEST_D64 | TEST_D128},

  /* A fragment of the radix character must never match it.  These reach the
     three separate byte-wise comparisons: after the integer digits, after the
     leading zeroes (where the comparison also has to stop at the terminating
     NUL instead of reading past it), and before any digit has been seen.  */
  {__LINE__, LOC_PS, INPUT_MB ("1" RADIX_HI "25"), 1, 0, 1.DL, 0, TEST_ALL},
  {__LINE__, LOC_PS, INPUT_MB ("0" RADIX_HI), 1, 0, 0.DL, 0, TEST_ALL},
  {__LINE__, LOC_PS, INPUT_MB (RADIX_HI "25"), 0, 0, 0.DL, 0, TEST_ALL},

  /* Infinities and NaNs, for reference in the C locale.  */
  {__LINE__, LOC_C, INPUT ("inf"), 3, 3, DEC_INFINITY, 0, TEST_ALL},
  {__LINE__, LOC_C, INPUT ("-INFINITY"), 9, 9, -DEC_INFINITY, 0, TEST_ALL},
  {__LINE__, LOC_C, INPUT ("NaN"), 3, 3, DEC_NAN, 0, TEST_ALL},
  {__LINE__, LOC_C, INPUT ("nan(1_a)"), 8, 8, DEC_NAN, 0, TEST_ALL},

  /* Neither the value nor the length of the radix character may affect
     them.  */
  {__LINE__, LOC_DE, INPUT ("Inf"), 3, 3, DEC_INFINITY, 0, TEST_ALL},
  {__LINE__, LOC_DE, INPUT ("-infinity"), 9, 9, -DEC_INFINITY, 0, TEST_ALL},
  {__LINE__, LOC_DE, INPUT ("nan"), 3, 3, DEC_NAN, 0, TEST_ALL},
  {__LINE__, LOC_PS, INPUT ("INF"), 3, 3, DEC_INFINITY, 0, TEST_ALL},
  {__LINE__, LOC_PS, INPUT ("-Infinity"), 9, 9, -DEC_INFINITY, 0, TEST_ALL},
  {__LINE__, LOC_PS, INPUT ("NAN"), 3, 3, DEC_NAN, 0, TEST_ALL},

  /* The radix character is not a digit, so it neither starts a number nor
     extends one, and it is not part of an n-char-sequence.  */
  {__LINE__, LOC_PS, INPUT ("inf" RADIX), 3, 3, DEC_INFINITY, 0, TEST_ALL},
  {__LINE__, LOC_PS, INPUT (RADIX "inf"), 0, 0, 0.DL, 0, TEST_ALL},
  {__LINE__, LOC_PS, INPUT ("nan(" RADIX ")"), 3, 3, DEC_NAN, 0, TEST_ALL},
  {__LINE__, LOC_DE, INPUT ("nan(1,5)"), 3, 3, DEC_NAN, 0, TEST_ALL},

  /* "inf" and "nan" are matched ignoring case, which must follow the C
     locale's case mapping rather than the current locale's: tr_TR maps "I" to
     U+0131 DOTLESS I, not to "i".  */
  {__LINE__, LOC_TR, INPUT ("inf"), 3, 3, DEC_INFINITY, 0, TEST_ALL},
  {__LINE__, LOC_TR, INPUT ("INF"), 3, 3, DEC_INFINITY, 0, TEST_ALL},
  {__LINE__, LOC_TR, INPUT ("Inf"), 3, 3, DEC_INFINITY, 0, TEST_ALL},
  {__LINE__, LOC_TR, INPUT ("+INFINITY"), 9, 9, DEC_INFINITY, 0, TEST_ALL},
  {__LINE__, LOC_TR, INPUT ("iNfInItY"), 8, 8, DEC_INFINITY, 0, TEST_ALL},
  {__LINE__, LOC_TR, INPUT ("nan"), 3, 3, DEC_NAN, 0, TEST_ALL},
  {__LINE__, LOC_TR, INPUT ("NAN"), 3, 3, DEC_NAN, 0, TEST_ALL},
  {__LINE__, LOC_TR, INPUT ("nan(I)"), 6, 6, DEC_NAN, 0, TEST_ALL},

  /* U+0131 uppercases to "I" in tr_TR, but it is still not "i".  */
  {__LINE__, LOC_TR, INPUT ("ınf"), 0, 0, 0.DL, 0, TEST_ALL},

  {0, NULL, NULL, 0, 0, 0, 0, 0, 0}
};

/* Render S into OUT with everything but printable ASCII escaped, so that
   failure messages stay readable whatever the input encoding is.  */
static const char *
quote (char *out, size_t outlen, const char *s)
{
  size_t o = 0;

  for (; *s != '\0' && o + 5 < outlen; ++s)
    {
      unsigned char c = (unsigned char) *s;

      if (c >= 0x20 && c < 0x7f)
	out[o++] = (char) c;
      else
	o += sprintf (out + o, "\\x%02x", c);
    }
  out[o] = '\0';
  return out;
}

static void
check_int (int line, const char *what, int got, int expected)
{
  if (got != expected)
    {
      fprintf (stdout, "%-3d Error: %s is %d, expected %d\n",
	       testnum, what, got, expected);
      fprintf (stdout, "in: %s:%d.\n\n", __FILE__, line);
      ++fail;
    }
}

/* Convert IN with PFX ## tod ## WID and check the value, quantum exponent,
   sign and endptr.  WANT is in bytes for strtodN, wide characters for
   wcstodN.  A non-finite result has no quantum exponent, and the sign of a
   NaN is not locale dependent, so both are only checked where they apply.  */
#define RUN_ONE(pfx, ctype, wid, in, want, fmt)				      \
  do {									      \
    ctype *ep = NULL;							      \
    int fail0 = fail;							      \
    _Decimal ## wid res = pfx ## tod ## wid ((in), &ep);		      \
									      \
    _VC_P (__FILE__, t->line, (_Decimal ## wid) t->expect, res, fmt);	      \
    if (isfinite (t->expect))						      \
      check_int (t->line, #pfx "tod" #wid " quantum exponent",		      \
		 llquantexpd ## wid (res), t->qexp);			      \
    if (!isnan (t->expect))						      \
      check_int (t->line, #pfx "tod" #wid " sign", !!signbit (res),	      \
		 !!signbit ((_Decimal ## wid) t->expect));		      \
    check_int (t->line, #pfx "tod" #wid " endptr offset",		      \
	       (int) (ep - (in)), (int) (want));			      \
    if (fail != fail0)							      \
      fprintf (stdout, "    locale \"%s\", input \"%s\"\n\n",		      \
	       t->locale, printable);					      \
  } while (0)

#define RUN_WIDTH(wid, fmt)						      \
  do {									      \
    if (t->types & TEST_D ## wid)					      \
      {									      \
	RUN_ONE (str, char, wid, t->input, t->nbytes, fmt);		      \
	if (t->winput != NULL)						      \
	  RUN_ONE (wcs, wchar_t, wid, t->winput, t->nwchars, fmt);	      \
      }									      \
  } while (0)

static void
run_test (const strtod_locale_test *t)
{
  char quoted[256];
  const char *printable = quote (quoted, sizeof (quoted), t->input);

  if (verbose)
    fprintf (stdout, "%s: \"%s\"\n", t->locale, printable);

  RUN_WIDTH (32, "%.6He");
  RUN_WIDTH (64, "%.15De");
  RUN_WIDTH (128, "%.33DDe");
}

int
main (int argc, char *argv[])
{
  const strtod_locale_test *t;

  verbose = 0; /* Make passing results quiet by default.  */

  for (int i = 1; i < argc; i++)
    if (strcmp (argv[i], "-v") == 0 || strcmp (argv[i], "--verbose") == 0)
      verbose = 1;

  for (t = tests; t->input != NULL; t++)
    {
      if (setlocale (LC_ALL, t->locale) == NULL)
	{
	  /* A locale which is not installed is warned about and skipped
	      rather than failed.  */
	  fprintf (stderr, "Warning: locale \"%s\" is not installed, "
		   "its tests did not run.\n", t->locale);
	  continue;
	}

      run_test (t);
    }

  setlocale (LC_ALL, "C");

  _REPORT ();

  /* fail comes from scaffold.c  */
  return fail;
}
