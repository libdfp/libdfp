/*  Copyright (C) 2014-2018 Free Software Foundation, Inc.

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
   if not, write to the Free Software Foundation, Inc., 59 Temple Place,
   Suite 330, Boston, MA 02111-1307 USA.

   Please see libdfp/COPYING.txt for more information.  */

/* Test flags (e.g. possible exceptions) */
#define NO_EXTRA_FLAG			0x0
#define INVALID_EXCEPTION		0x1
#define OVERFLOW_EXCEPTION		0x4
#define INEXACT_EXCEPTION		0x10
#define NO_INEXACT_EXCEPTION		0x200
#define NO_OVERFLOW_EXCEPTION		0x400
#define ERRNO_UNCHANGED			0x8000
#define ERRNO_ERANGE			0x20000
#define IGNORE_RESULT			0x40000
#define ONLY_ON_4BYTE_LONG		0x100000
#define ONLY_ON_8BYTE_LONG		0x200000


/* Common setup for an individual test.  */
#define COMMON_TEST_SETUP(FUNC_NAME, ARG_STR)                           \
  char *test_name;                                                      \
  if (asprintf (&test_name, "%s (%s)", #FUNC_NAME, (ARG_STR)) == -1)    \
    abort ();								\
  feclearexcept (FE_ALL_EXCEPT);					\
  errno = 0;

/* Common cleanup after an individual test.  */
#define COMMON_TEST_CLEANUP                     \
  free (test_name)

/* Skip test in case of invalid characteristics.  */
#define CHECK_SKIP_TEST(EXTRAFLAGS)			\
  if (((EXTRAFLAGS & ONLY_ON_4BYTE_LONG) != 0		\
       && sizeof(long int) != 4)			\
      || ((EXTRAFLAGS & ONLY_ON_8BYTE_LONG) != 0	\
	  && sizeof(long int) != 8))			\
    break;


/* Run an individual test, including any required setup and checking
   of results, or loop over all tests in an array.  */
#define RUN_TEST_f_f(FUNC_NAME, ARG_STR, ARG, EXPECTED, EXTRAFLAGS)	\
  do {                                                                  \
   CHECK_SKIP_TEST (EXTRAFLAGS);					\
   COMMON_TEST_SETUP (FUNC_NAME, ARG_STR);                              \
   check_float (test_name, FUNC (FUNC_NAME) (ARG), EXPECTED,		\
		EXTRAFLAGS);						\
   COMMON_TEST_CLEANUP;                                                 \
  } while (0)

#define RUN_TEST_ff_f(FUNC_NAME, ARG_STR, ARG1, ARG2, EXPECTED, EXTRAFLAGS) \
  do {                                                                  \
   CHECK_SKIP_TEST (EXTRAFLAGS);					\
   COMMON_TEST_SETUP (FUNC_NAME, ARG_STR);                              \
   check_float (test_name, FUNC (FUNC_NAME) (ARG1, ARG2), EXPECTED,	\
		EXTRAFLAGS);						\
   COMMON_TEST_CLEANUP;                                                 \
  } while (0)

#define RUN_TEST_f_i(FUNC_NAME, ARG_STR, ARG, EXPECTED, EXTRAFLAGS)	\
  do {                                                                  \
   CHECK_SKIP_TEST (EXTRAFLAGS);					\
   COMMON_TEST_SETUP (FUNC_NAME, ARG_STR);                              \
   check_int (test_name, FUNC (FUNC_NAME) (ARG), EXPECTED, EXTRAFLAGS);	\
   COMMON_TEST_CLEANUP;                                                 \
  } while (0)

#define RUN_TEST_f_l(FUNC_NAME, ARG_STR, ARG, EXPECTED, EXTRAFLAGS)	\
  do {                                                                  \
   CHECK_SKIP_TEST (EXTRAFLAGS);					\
   COMMON_TEST_SETUP (FUNC_NAME, ARG_STR);                              \
   check_long (test_name, FUNC (FUNC_NAME) (ARG), EXPECTED,		\
	       EXTRAFLAGS);						\
   COMMON_TEST_CLEANUP;                                                 \
  } while (0)

#define RUN_TEST_f_L(FUNC_NAME, ARG_STR, ARG, EXPECTED, EXTRAFLAGS)	\
  do {                                                                  \
   CHECK_SKIP_TEST (EXTRAFLAGS);					\
   COMMON_TEST_SETUP (FUNC_NAME, ARG_STR);                              \
   check_longlong (test_name, FUNC (FUNC_NAME) (ARG), EXPECTED,		\
		   EXTRAFLAGS);						\
   COMMON_TEST_CLEANUP;                                                 \
  } while (0)

#define RUN_TEST_f_b(FUNC_NAME, ARG_STR, ARG1, EXPECTED, EXTRAFLAGS) \
  do {                                                                  \
   CHECK_SKIP_TEST (EXTRAFLAGS);					\
   COMMON_TEST_SETUP (FUNC_NAME, ARG_STR);                              \
   check_bool (test_name, FUNC (FUNC_NAME) (ARG1), EXPECTED,		\
	       EXTRAFLAGS);						\
   COMMON_TEST_CLEANUP;                                                 \
  } while (0)

#define RUN_TEST_ff_b(FUNC_NAME, ARG_STR, ARG1, ARG2,  EXPECTED, EXTRAFLAGS) \
  do {                                                                  \
   CHECK_SKIP_TEST (EXTRAFLAGS);					\
   COMMON_TEST_SETUP (FUNC_NAME, ARG_STR);                              \
   check_bool (test_name, FUNC (FUNC_NAME) (ARG1, ARG2), EXPECTED,	\
	       EXTRAFLAGS);						\
   COMMON_TEST_CLEANUP;                                                 \
  } while (0)

#define RUN_TEST_ff_i(FUNC_NAME, ARG_STR, ARG1, ARG2,  EXPECTED, EXTRAFLAGS) \
  do {                                                                  \
   CHECK_SKIP_TEST (EXTRAFLAGS);					\
   COMMON_TEST_SETUP (FUNC_NAME, ARG_STR);                              \
   check_int (test_name, FUNC (FUNC_NAME) (ARG1, ARG2), EXPECTED,	\
	      EXTRAFLAGS);						\
   COMMON_TEST_CLEANUP;                                                 \
  } while (0)


#define MANT_DIG CHOOSE ((DEC128_MANT_DIG-1), (DEC64_MANT_DIG-1), (DEC32_MANT_DIG-1))
#define MIN_EXP CHOOSE ((DEC128_MIN_EXP-1), (DEC64_MIN_EXP-1), (DEC32_MIN_EXP-1))

#define PRINTF_EXPR CHOOSE ("DDe", "De", "He")
#define PRINTF_NEXPR CHOOSE ("DDf", "Df", "Hf")
#define PRINTF_XEXPR CHOOSE (".33DDe", ".15De", ".7He")

/* TODO: Add set parse options.  */
#define ulps_file_name "ULPs"   /* Name of the ULPs file.  */
static FILE *ulps_file;         /* File to document difference.  */
static int output_ulps;         /* Should ulps printed?  */

static int noErrors;            /* number of errors */
static int noTests;             /* number of tests (without testing exceptions) */

static int verbose = 99;
static int output_points = 1;   /* Should the single function results printed?  */


/* Compare KEY (a string, with the name of a test or a function) with
   ULP (a pointer to a struct ulp_data structure), returning a value
   less than, equal to or greater than zero for use in bsearch.  */

static int
compare_ulp_data (const void *key, const void *ulp)
{
  const char *keystr = key;
  const struct ulp_data *ulpdata = ulp;
  return strcmp (keystr, ulpdata->name);
}

/* Return the ulps for NAME in array DATA with NMEMB elements, or 0 if
   no ulps listed.  */
static FLOAT
find_ulps (const char *name, const struct ulp_data *data, size_t nmemb)
{
  const struct ulp_data *entry = bsearch (name, data, nmemb, sizeof (*data),
                                          compare_ulp_data);
  if (entry == NULL)
    return 0;
  else
    return entry->max_ulp;
}

/* Return the ulps for test NAME.  */
static FLOAT
find_test_ulps (const char *name)
{
  return find_ulps (name, test_ulps,
                    sizeof (test_ulps) / sizeof (test_ulps[0]));
}


/* Returns the size of an ulp for VALUE.  */
static FLOAT
ulp (FLOAT value)
{
  FLOAT ulp;

  switch (fpclassify (value))
    {
      case FP_ZERO:
        /* We compute the distance to the next FP which is the same as the
           value of the smallest subnormal number. Previously we used
           10^(-MANT_DIG) which is too large a value to be useful. Note that we
           can't use ilogb(0), since that isn't a valid thing to do.  */
        /* Fall through...  */
      case FP_SUBNORMAL:
        /* The next closest subnormal value is a constant distance away.  */
        /* TODO: check it  */
        ulp = FUNC(ldexp) (1.0DF, MIN_EXP - MANT_DIG);
        break;

      case FP_NORMAL:
        ulp = FUNC(ldexp) (1.0DF, FUNC(ilogb) (value) - (MANT_DIG) - 1);
        break;

      default:
        /* It should never happen. */
        abort ();
        break;
    }
  return ulp;
}

static void
print_ulps (const char *test_name, FLOAT ulp)
{
  if (output_ulps)
    {
      fprintf (ulps_file, "Test \"%s\":\n", test_name);
      fprintf (ulps_file, "%s: %.0" PRINTF_NEXPR "\n",
               CHOOSE("decimal128", "decimal64", "decimal32"),
               FUNC(ceil) (ulp));
    }
}


/* Should the message print to screen?  This depends on the verbose flag,
   and the test status.  */
static int
print_screen (int ok)
{
  if (output_points
      && (verbose > 1
          || (verbose == 1 && ok == 0)))
    return 1;
  return 0;
}

/* Print a FLOAT.  */
static void
print_float (FLOAT f)
{
  /* As printf doesn't differ between a sNaN and a qNaN, do this manually.  */
  if (issignaling (f))
    printf ("sNaN\n");
  else if (isnan (f))
    printf ("qNaN\n");
  else
    printf ("% .20" PRINTF_EXPR "  %" PRINTF_XEXPR "\n", f, f);
}

/* Update statistic counters.  */
static void
update_stats (int ok)
{
  ++noTests;
  if (!ok)
    ++noErrors;
}


/* Returns the number of ulps that GIVEN is away from EXPECTED.  */
#define ULPDIFF(given, expected) \
        (FUNC(fabs) ((given) - (expected)) / ulp (expected))

/* Test whether a given exception was raised.  */
static void
test_single_exception (const char *test_name, int exception, int exc_flag,
		       int fe_flag, const char *flag_name)
{
  int ok = 1;
  if (exception & exc_flag)
    {
      if (fetestexcept (fe_flag))
	{
	  if (print_screen (1))
	    printf ("Pass: %s: Exception \"%s\" set\n", test_name, flag_name);
	}
      else
	{
	  ok = 0;
	  if (print_screen (0))
	    printf ("Failure: %s: Exception \"%s\" not set\n",
		    test_name, flag_name);
	}
    }
  else
    {
      if (fetestexcept (fe_flag))
	{
	  ok = 0;
	  if (print_screen (0))
	    printf ("Failure: %s: Exception \"%s\" set\n",
		    test_name, flag_name);
	}
      else
	{
	  if (print_screen (1))
	    printf ("%s: Exception \"%s\" not set\n", test_name,
		    flag_name);
	}
    }
  if (!ok)
    ++noErrors;
}

static void
test_exceptions (const char *test_name, int exception)
{
#ifdef FE_INEXACT
  if ((exception & (INEXACT_EXCEPTION | NO_INEXACT_EXCEPTION)) != 0)
    test_single_exception (test_name, exception, INEXACT_EXCEPTION,
			   FE_INEXACT, "Inexact");
#endif
#ifdef FE_INVALID
      if ((exception & INVALID_EXCEPTION) != 0)
	test_single_exception (test_name, exception,
			       INVALID_EXCEPTION, FE_INVALID,
			       "Invalid");
#endif
#ifdef FE_OVERFLOW
      if ((exception & (OVERFLOW_EXCEPTION  | NO_OVERFLOW_EXCEPTION)) != 0)
	test_single_exception (test_name, exception,
			       OVERFLOW_EXCEPTION, FE_OVERFLOW,
			       "Overflow");
#endif
}

/* Test whether errno for TEST_NAME, set to ERRNO_VALUE, has value
   EXPECTED_VALUE (description EXPECTED_NAME).  */
static void
test_single_errno (const char *test_name, int errno_value,
		   int expected_value, const char *expected_name)
{
  if (errno_value == expected_value)
    {
      if (print_screen (1))
	printf ("Pass: %s: errno set to %d (%s)\n", test_name, errno_value,
		expected_name);
    }
  else
    {
      ++noErrors;
      if (print_screen (0))
	printf ("Failure: %s: errno set to %d, expected %d (%s)\n",
		test_name, errno_value, expected_value, expected_name);
    }
}

/* Test whether errno (value ERRNO_VALUE) has been for TEST_NAME set
   as required by EXCEPTIONS.  */
static void
test_errno (const char *test_name, int errno_value, int exceptions)
{
  if (exceptions & ERRNO_UNCHANGED)
    test_single_errno (test_name, errno_value, 0, "unchanged");
  if (exceptions & ERRNO_ERANGE)
    test_single_errno (test_name, errno_value, ERANGE, "ERANGE");
}

static void
__attribute__((used))
check_float (const char *test_name, FLOAT computed, FLOAT expected,
	     int extraflags)
{
  int ok = 0;
  int print_diff = 0;
  FLOAT diff = 0;
  FLOAT ulps = 0;
  int errno_value = errno;

  test_exceptions (test_name, extraflags);
  test_errno (test_name, errno_value, extraflags);
  if (extraflags & IGNORE_RESULT)
    goto out;

  FLOAT max_ulp = find_test_ulps (test_name);
  if (issignaling (computed) && issignaling (expected))
    ok = 1;
  else if (issignaling (computed) || issignaling (expected))
    ok = 0;
  else if (isnan (computed) && isnan (expected))
    ok = 1;

  else if (isinf (computed) && isinf (expected))
    ok = 1;
  /* Don't calculate ULPs for infinities or any kind of NaNs.  */
  else if (isinf (computed) || isnan (computed)
           || isinf (expected) || isnan (expected))
    ok = 0;
  else
    {
      diff = FUNC(fabs) (computed - expected);
      ulps = ULPDIFF (computed, expected);
      print_diff = 1;
      //if ((exceptions & IGNORE_ZERO_INF_SIGN) == 0
      if (signbit(computed) != signbit (expected))
        ok = 0;
      //else if (ulps <= 0.5 || (ulps <= max_ulp && !ignore_max_ulp))
      else if ((ulps <= 0.5DF) || (ulps <= max_ulp))
        ok = 1;
      else
        {
          ok = 0;
          print_ulps (test_name, ulps);
        }

    }
  if (print_screen (ok))
    {
      if (!ok)
        printf ("Failure: ");
      printf ("Test: %s\n", test_name);
      printf ("Result:\n");
      printf (" is:         ");
      print_float (computed);
      printf (" should be:  ");
      print_float (expected);
      if (print_diff)
        {
          printf (" difference: % .20" PRINTF_EXPR "  %" PRINTF_XEXPR
                  "\n", diff, diff);
          printf (" ulp       : % .4" PRINTF_NEXPR "\n", ulps);
          printf (" max.ulp   : % .4" PRINTF_NEXPR "\n", max_ulp);
        }
    }
  update_stats (ok);

 out:
  errno = 0;
}

/* Check that computed and expected values are equal (int values).  */
static void
__attribute__((used))
check_int (const char *test_name, int computed, int expected, int extraflags)
{
  int ok = 0;
  int errno_value = errno;

  test_exceptions (test_name, extraflags);
  test_errno (test_name, errno_value, extraflags);
  if (extraflags & IGNORE_RESULT)
    goto out;
  noTests++;
  if (computed == expected)
    ok = 1;

  if (print_screen (ok))
    {
      if (!ok)
        printf ("Failure: ");
      printf ("Test: %s\n", test_name);
      printf ("Result:\n");
      printf (" is:         %d\n", computed);
      printf (" should be:  %d\n", expected);
    }

  update_stats (ok);
 out:
  errno = 0;
}

/* check that computed and expected values are equal (long int values) */
static void
__attribute__((used))
check_long (const char *test_name, long int computed, long int expected,
	    int extraflags)
{
  int ok = 0;
  int errno_value = errno;

  test_exceptions (test_name, extraflags);
  test_errno (test_name, errno_value, extraflags);
  if (extraflags & IGNORE_RESULT)
    goto out;
  noTests++;
  if (computed == expected)
    ok = 1;

  if (print_screen (ok))
    {
      if (!ok)
        printf ("Failure:");
      printf ("Test: %s\n", test_name);
      printf ("Result:\n");
      printf (" is:         %ld\n", computed);
      printf (" should be:  %lxd\n", expected);
    }

  update_stats (ok);
 out:
  errno = 0;
}

/* check that computed and expected values are equal (long int values) */
static void
__attribute__((used))
check_longlong (const char *test_name, long long int computed,
		long long int expected, int extraflags)
{
  int ok = 0;
  int errno_value = errno;

  test_exceptions (test_name, extraflags);
  test_errno (test_name, errno_value, extraflags);
  if (extraflags & IGNORE_RESULT)
    goto out;
  noTests++;
  if (computed == expected)
    ok = 1;

  if (print_screen (ok))
    {
      if (!ok)
        printf ("Failure:");
      printf ("Test: %s\n", test_name);
      printf ("Result:\n");
      printf (" is:         %lld\n", computed);
      printf (" should be:  %lld\n", expected);
    }

  update_stats (ok);
 out:
  errno = 0;
}

/* Check that computed value is true/false.  */
static void
__attribute__((used))
check_bool (const char *test_name, int computed, int expected, int extraflags)
{
  int ok = 0;
  int errno_value = errno;

  test_exceptions (test_name, extraflags);
  test_errno (test_name, errno_value, extraflags);
  if (extraflags & IGNORE_RESULT)
    goto out;
  noTests++;
  if ((computed == 0) == (expected == 0))
    ok = 1;

  if (print_screen (ok))
    {
      if (!ok)
        printf ("Failure: ");
      printf ("Test: %s\n", test_name);
      printf ("Result:\n");
      printf (" is:         %d\n", computed);
      printf (" should be:  %d\n", expected);
    }

  update_stats (ok);
 out:
  errno = 0;
}
