/* Function definition to convert DFP values to strings

   Copyright (C) 2006, 2009 Free Software Foundation, Inc.

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

#warn "This is an incomplete implementation.  Please adapt sysdeps/dpd/printf_dfp.c for BID.  */
#include "printf_dfp.h"
#include <stdio.h>
#include <string.h>

#include <fmt_dfp.h>

static int pa_d128;
static int pa_d64;
static int pa_d32;

void
__d128_va (void *mem, va_list *ap)
{
  _Decimal128 d = va_arg (*ap, _Decimal128);
  memcpy (mem, &d, sizeof (d));
}
strong_alias(__d128_va, d128_va)
hidden_def(__d128_va)

void
__d64_va (void *mem, va_list *ap)
{
  _Decimal64 d = va_arg (*ap, _Decimal64);
  memcpy (mem, &d, sizeof (d));
}
strong_alias(__d64_va, d64_va)
hidden_def(__d64_va)

void
__d32_va (void *mem, va_list *ap)
{
  _Decimal32 d = va_arg (*ap, _Decimal32);
  memcpy (mem, &d, sizeof (d));
}
strong_alias(__d32_va, d32_va)
hidden_def(__d32_va)

int
__d128_ais (const struct printf_info *info __attribute__ ((unused)), size_t n __attribute__ ((unused)), int *argtype, int *size)
{
  argtype[0] = pa_d128;
  size[0] = sizeof (_Decimal128);
  /* Isn't this going to say it always handles this type?  */
  return 1;
}
strong_alias(__d128_ais, d128_ais)
hidden_def(__d128_ais)

int
__d64_ais (const struct printf_info *info __attribute__ ((unused)), size_t n __attribute__ ((unused)), int *argtype, int *size)
{
  argtype[0] = pa_d64;
  size[0] = sizeof (_Decimal64);
  return 1;
}
strong_alias(__d64_ais, d64_ais)
hidden_def(__d64_ais)

int
__d32_ais (const struct printf_info *info __attribute__ ((unused)), size_t n __attribute__ ((unused)), int *argtype, int *size)
{
  argtype[0] = pa_d32;
  size[0] = sizeof (_Decimal32);
  return 1;
}
strong_alias(__d32_ais, d32_ais)
hidden_def(__d32_ais)

static int mod_H;
static int mod_D;
static int mod_DD;

#define DECIMAL_PRINTF_BUF_SIZE 97 /* ((DECIMAL128_PMAX + 14) * 2) + 1  */

int
__printf_dfp (FILE *fp,
	      const struct printf_info *info,
	      const void *const *args)
{
	char str[DECIMAL_PRINTF_BUF_SIZE];
	int len = 0;

	if (info->user & mod_H) /* %H  */
	  {
	   __fmt_d32 (info, args, str, DECIMAL_PRINTF_BUF_SIZE);
	  }
	else if (info->user & mod_DD) /* %DD  */
	  {
	   __fmt_d128 (info, args, str, DECIMAL_PRINTF_BUF_SIZE);
	  }
	else /* %D  */
	  {
	   __fmt_d64 (info, args, str, DECIMAL_PRINTF_BUF_SIZE);
	  }
	len=strlen(str);

	len = fprintf(fp,"%*s", len, &str[0]);
	return len;
}
strong_alias (__printf_dfp, printf_dfp)
hidden_def (__printf_dfp)

int __register_printf_dfp (void)
{
  pa_d128 = register_printf_type (d128_va);
  pa_d32 = register_printf_type (d32_va);
  pa_d64 = register_printf_type (d64_va);

  mod_DD = register_printf_modifier (L"DD");
  mod_H = register_printf_modifier (L"H");
  mod_D = register_printf_modifier (L"D");

  register_printf_specifier ('f', printf_dfp, d128_ais);
  register_printf_specifier ('F', printf_dfp, d128_ais);
  register_printf_specifier ('e', printf_dfp, d128_ais);
  register_printf_specifier ('E', printf_dfp, d128_ais);
  register_printf_specifier ('g', printf_dfp, d128_ais);
  register_printf_specifier ('G', printf_dfp, d128_ais);
  register_printf_specifier ('a', printf_dfp, d128_ais);
  register_printf_specifier ('A', printf_dfp, d128_ais);

  register_printf_specifier ('f', printf_dfp, d32_ais);
  register_printf_specifier ('F', printf_dfp, d32_ais);
  register_printf_specifier ('e', printf_dfp, d32_ais);
  register_printf_specifier ('E', printf_dfp, d32_ais);
  register_printf_specifier ('g', printf_dfp, d32_ais);
  register_printf_specifier ('G', printf_dfp, d32_ais);
  register_printf_specifier ('a', printf_dfp, d32_ais);
  register_printf_specifier ('A', printf_dfp, d32_ais);

  register_printf_specifier ('f', printf_dfp, d64_ais);
  register_printf_specifier ('F', printf_dfp, d64_ais);
  register_printf_specifier ('e', printf_dfp, d64_ais);
  register_printf_specifier ('E', printf_dfp, d64_ais);
  register_printf_specifier ('g', printf_dfp, d64_ais);
  register_printf_specifier ('G', printf_dfp, d64_ais);
  register_printf_specifier ('a', printf_dfp, d64_ais);
  register_printf_specifier ('A', printf_dfp, d64_ais);

  return 0;
}
strong_alias (__register_printf_dfp, register_printf_dfp)
