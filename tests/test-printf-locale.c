/* Test the ' (grouping) flag for decimal printf conversions.

   This file is part of the Decimal Floating Point C Library.

   The Decimal Floating Point C Library is free software; you can
   redistribute it and/or modify it under the terms of the GNU Lesser
   General Public License version 2.1.

   The Decimal Floating Point C Library is distributed in the hope
   that it will be useful, but WITHOUT ANY WARRANTY; without even the
   implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
   PURPOSE.  See the GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the Decimal Floating Point C Library; if not,
   write to the Free Software Foundation, Inc., 51 Franklin Street,
   Fifth Floor, Boston, MA 02110-1301 USA.

   Please see libdfp/COPYING.txt for more information.  */

/* For the ' flag, the C specification requires the integer part of the
   conversion to be grouped with the locale's thousands separator (every
   three digits in en_US).  A separator is inserted between digit groups,
   never before the first digit, so a three-digit integer part gets no
   separator at all.  The field width counts the separators.  */

#ifndef __STDC_WANT_DEC_FP__
# define __STDC_WANT_DEC_FP__ 1
#endif

#include <locale.h>
#include <stdio.h>
#include <string.h>

#include "scaffold.c"

typedef struct {
  int line;
  const char *expect;
  const char *format;
  _Decimal32 d;
} d32_type;

typedef struct {
  int line;
  const char *expect;
  const char *format;
  _Decimal64 d;
} d64_type;

typedef struct {
  int line;
  const char *expect;
  const char *format;
  _Decimal128 d;
} d128_type;

/* Instantiate the same cases for a decimal type.
     SFX: literal suffix, DF, DD or DL.
     MOD: printf length modifier, H, D or DD.  */
#define GROUP_CASES(SFX, MOD)						\
  {__LINE__, "123.000000", "%'" MOD "f", 123.##SFX},			\
  {__LINE__, "1,234.000000", "%'" MOD "f", 1234.##SFX},			\
  {__LINE__, "12,345.000000", "%'" MOD "f", 12345.##SFX},		\
  {__LINE__, "123,456.000000", "%'" MOD "f", 123456.##SFX},		\
  {__LINE__, "1,234,567.000000", "%'" MOD "f", 1234567.##SFX},		\
  {__LINE__, "-1,234.500000", "%'" MOD "f", -1234.5##SFX},		\
  /* The field width counts the separators.  */				\
  {__LINE__, "         123.00", "%'15.2" MOD "f", 123.##SFX},		\
  {__LINE__, "     123,456.00", "%'15.2" MOD "f", 123456.##SFX},	\
  {__LINE__, "123,456.00     ", "%'-15.2" MOD "f", 123456.##SFX},	\
  {__LINE__, "000000123,456.00", "%'016.2" MOD "f", 123456.##SFX},	\
  {__LINE__, "1,234", "%'" MOD "g", 1234.##SFX},			\
  {__LINE__, "123,456", "%'" MOD "g", 123456.##SFX},			\
  {__LINE__, "        123,456", "%'15" MOD "g", 123456.##SFX},

d32_type cases_d32[] =
{
  GROUP_CASES (DF, "H")
  {0, NULL, NULL, 0}
};

d64_type cases_d64[] =
{
  GROUP_CASES (DD, "D")
  {0, NULL, NULL, 0}
};

d128_type cases_d128[] =
{
  GROUP_CASES (DL, "DD")
  {0, NULL, NULL, 0}
};

static char buf[1024];

static void
run_d32 (void)
{
  d32_type *p;

  for (p = cases_d32; p->line; p++)
    {
      memset (buf, '\0', sizeof buf);
      snprintf (buf, sizeof buf, p->format, p->d);
      _SC_P (__FILE__, p->line, p->expect, buf);
    }
}

static void
run_d64 (void)
{
  d64_type *p;

  for (p = cases_d64; p->line; p++)
    {
      memset (buf, '\0', sizeof buf);
      snprintf (buf, sizeof buf, p->format, p->d);
      _SC_P (__FILE__, p->line, p->expect, buf);
    }
}

static void
run_d128 (void)
{
  d128_type *p;

  for (p = cases_d128; p->line; p++)
    {
      memset (buf, '\0', sizeof buf);
      snprintf (buf, sizeof buf, p->format, p->d);
      _SC_P (__FILE__, p->line, p->expect, buf);
    }
}

int
main (int argc, char *argv[])
{
  int i;

  /* Make passing results quiet by default.  */
  verbose = 0;

  for (i = 1; i < argc; i++)
    if (strcmp (argv[i], "-v") == 0 || strcmp (argv[i], "--verbose") == 0)
      verbose = 1;

  /* A locale whose LC_NUMERIC grouping is every three digits with a comma
     separator; the C locale has no grouping at all.  */
  if (setlocale (LC_ALL, "en_US.UTF-8") == NULL)
    {
      fprintf (stdout, "warning: en_US.UTF-8 locale is not installed, "
	       "skipping.\n");
      return 0;
    }

  run_d32 ();
  run_d64 ();
  run_d128 ();

  setlocale (LC_ALL, "C");

  _REPORT ();

  return fail;
}
