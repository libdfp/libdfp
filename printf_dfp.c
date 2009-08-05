/* Function definition to convert DFP values to strings

   Copyright (C) 2006, 2009 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Ryan S. Arnold <rsa@us.ibm.com>
              Pete Eberlein <eberlein@us.ibm.com>

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

/* Based on GLIBC stdio-common/printf_fp.c by:
   Ulrich Drepper <drepper@gnu.ai.mit.edu>  */


#include "printf_dfp.h"
#include <unistd.h>
/* wchar.h has to be included BEFORE stdio.h or it loses function
 * definitions when dfp/wchar.h uses #include_next <wchar.h>.  */
#include <wchar.h>
#include <stdio.h>

#include <string.h>
//#include <libioP.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

//#include <fmt_dfp.h>

#include <fenv.h>

#include <langinfo.h>
#include <wctype.h>
#include <limits.h> /* For CHAR_MAX  */
#include <stdio.h>

#include "get_digits.h"

//#include "numdigits.h"
//
//extern void __get_dpd_digits (int, const void *const *, char*, int*, int*, int*,
//int*);


/* This defines make it possible to use the same code for GNU C library and
   the GNU I/O library.	 */
//#define PUT(f, s, n) _IO_sputn (f, s, n)

static size_t
wpadn (FILE *fp, wint_t pad, size_t count)
{
  unsigned int i;
  size_t written = 0;

  for (i=0;i<count;i++)
      if(putwc(pad,fp) != WEOF)
	written++;

  return written;
}

static size_t
padn (FILE *fp, int pad, size_t count)
{
  unsigned int i;
  size_t written = 0;

  for (i=0;i<count;i++)
      if(putc(pad,fp) != EOF)
	written++;

  return written;
}

#ifdef COMPILE_WIDE
# ifndef L
#  define L(c) L##c
# endif
# ifndef CHAR_T
#  define CHAR_T wchar_t
# endif
# ifndef PUT
#  define PUT(x,y) putwc(x,y)
# endif
#else
# ifndef L
#  define L(c) c
# endif
# ifndef CHAR_T
#  define CHAR_T char
# endif
# ifndef PUT
#  define PUT(x,y) putc(x,y)
# endif
#endif

#define PAD(f, c, n) (wide ? wpadn (f, c, n) : padn (f, c, n))

#define size_t     _IO_size_t
#define FILE	     _IO_FILE

/* Macros for doing the actual output.  */

#define outchar(ch)							      \
  do									      \
    {									      \
      register const int outc = (ch);					      \
      if (PUT(outc, fp) == EOF)						      \
	{								      \
	  return -1;							      \
	}								      \
      ++done;								      \
    } while (0)

#define PADN(ch, len)							      \
  do									      \
    {									      \
      if (PAD (fp, ch, len) != len)					      \
	{								      \
	  return -1;							      \
	}								      \
      done += len;							      \
    }									      \
  while (0)

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



#define DECIMAL_PRINTF_BUF_SIZE 65 /* ((DECIMAL128_PMAX + 14) * 2) + 1  */

/* fe_decround.c will initialize this function pointer to fe_decgetround */
int (*__printf_dfp_getround_callback)(void) = NULL;

int
__printf_dfp (FILE *fp,
	      const struct printf_info *info,
	      const void *const *args)
{
	int wide = info->wide;
//	wchar_t *wbuffer = NULL;
//	int buffer_malloced = 0;  /* PRINT macro uses this.  */
	/* Counter for number of written characters.	*/
	int done = 0;
	int len = 0;

  /* Locale-dependent representation of decimal point.	*/

#ifndef OPTION_EGLIBC_LOCALE_CODE
  char decimald;
#endif
  const char *decimal;

  union { const char *mb; unsigned int wc; } decimalwc;

  char spec = tolower(info->spec);

  /* Locale-dependent thousands separator and grouping specification.  */
  const char *thousands_sep = NULL;
  wchar_t thousands_sepwc = 0;
  const char * thousands_sepmb;

  const char *grouping;

//  mbstate_t mbstate;
 // memset(&mbstate,0,sizeof(mbstate));

#ifdef OPTION_EGLIBC_LOCALE_CODE
  if (info->extra == 0)
    {
      //decimal = _NL_CURRENT (LC_NUMERIC, DECIMAL_POINT);
      decimal = nl_langinfo (__DECIMAL_POINT);
      //decimalwc = _NL_CURRENT_WORD (LC_NUMERIC, _NL_NUMERIC_DECIMAL_POINT_WC);
      decimalwc.mb = nl_langinfo (_NL_NUMERIC_DECIMAL_POINT_WC);
      //mbrtowc(&decimalwc,decimalmb, CHAR_MAX, NULL);
      //mbrtowc(&decimalwc,&decimalmb, 1, NULL);
    }
  else
    {
     // decimal = _NL_CURRENT (LC_MONETARY, MON_DECIMAL_POINT);
      decimal = nl_langinfo (__MON_DECIMAL_POINT);
      if (*decimal == '\0')
	//decimal = _NL_CURRENT (LC_NUMERIC, DECIMAL_POINT);
	decimal = nl_langinfo (__DECIMAL_POINT);
      //decimalwc = _NL_CURRENT_WORD (LC_MONETARY,
      //			    _NL_MONETARY_DECIMAL_POINT_WC);
      decimalwc.mb = nl_langinfo (_NL_MONETARY_DECIMAL_POINT_WC);
      //mbrtowc(&decimalwc,decimalmb, CHAR_MAX, NULL);
      if (decimalwc.wc == L'\0')
	{
	//decimalwc = _NL_CURRENT_WORD (LC_NUMERIC,
	//			      _NL_NUMERIC_DECIMAL_POINT_WC);
	  decimalwc.mb = nl_langinfo (_NL_NUMERIC_DECIMAL_POINT_WC);
	  //mbrtowc(&decimalwc,decimalmb, CHAR_MAX, NULL);
	}
    }
  /* The decimal point character must not be zero.  */
  assert (*decimal != '\0');
  //assert (decimalwc != L'\0');
  assert (decimalwc.wc != L'\0');
#else
  /* Hard-code values from 'C' locale.  */
  decimald = ".";
  decimal = &decimald;
  decimalwc.wc = L'.';
#endif


#ifdef OPTION_EGLIBC_LOCALE_CODE
  if (info->group)
    {
      if (info->extra == 0)
	//grouping = _NL_CURRENT (LC_NUMERIC, GROUPING);
	grouping = nl_langinfo (__GROUPING);
      else
	//grouping = _NL_CURRENT (LC_MONETARY, MON_GROUPING);
	grouping = nl_langinfo (__MON_GROUPING);

      if (*grouping <= 0 || *grouping == CHAR_MAX)
	grouping = NULL;
      else
	{
	  /* Figure out the thousands separator character.  */
	  if (wide)
	    {
	      if (info->extra == 0)
		{
		  thousands_sepmb = nl_langinfo (_NL_NUMERIC_THOUSANDS_SEP_WC);
		  mbrtowc(&thousands_sepwc,thousands_sepmb, CHAR_MAX, NULL);
		  //_NL_CURRENT_WORD (LC_NUMERIC, _NL_NUMERIC_THOUSANDS_SEP_WC);
		}
	      else
		{
		  thousands_sepmb = nl_langinfo (_NL_MONETARY_THOUSANDS_SEP_WC);
		  mbrtowc(&thousands_sepwc,thousands_sepmb, CHAR_MAX, NULL);
		  //_NL_CURRENT_WORD (LC_MONETARY,
		  //		    _NL_MONETARY_THOUSANDS_SEP_WC);
		}
	    }
	  else
	    {
	      if (info->extra == 0)
		thousands_sep = nl_langinfo (__THOUSANDS_SEP);
		//thousands_sep = _NL_CURRENT (LC_NUMERIC, THOUSANDS_SEP);
	      else
		thousands_sep = nl_langinfo (__MON_THOUSANDS_SEP);
		//thousands_sep = _NL_CURRENT (LC_MONETARY, MON_THOUSANDS_SEP);
	    }

	  if ((wide && thousands_sepwc == L'\0')
	      || (! wide && *thousands_sep == '\0'))
	    grouping = NULL;
	  else if (thousands_sepwc == L'\0')
	    /* If we are printing multibyte characters and there is a
	       multibyte representation for the thousands separator,
	       we must ensure the wide character thousands separator
	       is available, even if it is fake.  */
	    thousands_sepwc = 0xfffffffe;
	}
    }
  else
    grouping = NULL;
#else
  grouping = NULL;
#endif


{
    char digits[DECIMAL_PRINTF_BUF_SIZE];
    int exp, /* the exponent */
     is_neg, /* is negative */
     is_nan, /* is not a number */
     is_inf, /* is infinite */
     sig,    /* number of significant digits */
     decpt,  /* decimal point offset into digits[] */
     prec,   /* number of digits that follow the decimal point, or number of significant digits for %g */
     default_prec = 0, /* default precision, if none given */
     n;      /* current digit offset into digits[] */

     //width,  /* width of the field */
    unsigned int width;  /* width of the field */
    digits[0] = '0'; /* need an extra digit for rounding up */

//    __get_dpd_digits (
//      (info->is_short) ?        32: /* %H */
//      (info->is_long_double) ? 128: /* %DD */
//                                64, /* %D */
//	args, digits+1, &exp, &is_neg, &is_nan, &is_inf);

    //if (info->is_short)
    if (info->user & mod_H)
      __get_digits_d32 (**((_Decimal32**)args[0]), digits+1, &exp, &is_neg,
			&is_nan, &is_inf);
    else if (info->user & mod_DD)
      __get_digits_d128 (**((_Decimal128**)args[0]), digits+1, &exp, &is_neg,
			 &is_nan, &is_inf);
    else
      __get_digits_d64 (**((_Decimal64**)args[0]), digits+1, &exp, &is_neg,
			&is_nan, &is_inf);

    width = info->width;
    prec = info->prec;
    
    if (is_nan || is_inf) {
      width -= 3;
      /*if (is_nan) is_neg = 0;*/
      if (is_neg || info->showsign || info->space) width--;
      
      if (!info->left && width > 0)
        PADN (' ', width);

      if (is_neg)
        outchar ('-');
      else if (info->showsign)
        outchar ('+');
      else if (info->space)
        outchar (' ');
      
      if (is_nan)
        if (isupper(info->spec))
	  { outchar ('N'); outchar ('A'); outchar ('N'); }
	else
	  { outchar ('n'); outchar ('a'); outchar ('n'); }
      else
        if (isupper(info->spec))
	  { outchar ('I'); outchar ('N'); outchar ('F'); }
	else
	  { outchar ('i'); outchar ('n'); outchar ('f'); }
	
      if (info->left && width > 0)
        PADN (' ', width);

      return 0;
    }
    
   
    n = 0;
    while (digits[n] == '0') n++;
    sig = strlen(digits+n);
    if (sig == 0) { sig = 1; n--; } /* coefficient is zero */
    len = n + sig;
    
    switch (spec)
      {
        case 'a': /* fall thru */
	case 'g': default_prec = sig; break;
	case 'f': default_prec = (exp < 0) ? -exp : 0; break;
        case 'e': default_prec = sig-1; break;
      }
    
    /* if no precision is specified, use that of the decimal type */
    if (prec < 0)
      prec = default_prec;
    else if (prec < default_prec)
    /* do rounding if precision is less than the decimal type */
      {
        int index, roundmode = 0;
	//int do_round = 0;
	char rounddigit = '4';
	
        index = n + prec + sig - default_prec;

        /* FIXME: we should check rounding mode for %a */
	if (__printf_dfp_getround_callback) {
	  roundmode = (*__printf_dfp_getround_callback)();
	
/*	outchar('[');
	outchar('r');
	outchar('o');
	outchar('u');
	outchar('n');
	outchar('d');
	outchar('m');
	outchar('o');
	outchar('d');
	outchar('e');
	outchar('=');
	outchar('0'+roundmode);
	outchar(']');*/
	
	switch (roundmode) {
	  case FE_DEC_TONEAREST: rounddigit = '4'; break;
	  case FE_DEC_TOWARDZERO: rounddigit = '9'; break;
	  case FE_DEC_UPWARD: rounddigit = is_neg ? '9' : '0'-1; break;
	  case FE_DEC_DOWNWARD: rounddigit = is_neg ? '0'-1 : '9'; break;
	  case FE_DEC_TONEARESTFROMZERO: rounddigit = '4'; break;
	  case 5: rounddigit = '4'; break; /* nearest, ties toward zero */
	  case 6: rounddigit = '0'-1; break; /* away from zero */
	  case 7: rounddigit = '4'; break; /* round for shorter precision */
	  default: rounddigit = '4'; break;
	}
	
	}
	
        if (index < len && digits[index] > rounddigit)
          do { 
	    int trailzero = index+1;
	    if (digits[index] == rounddigit+1)
	      {
	        while (trailzero < len)
	          {
	            if (digits[trailzero] != '0')
		      {
		        trailzero = 0;
		        break;
		      }
		    ++trailzero;
		  }
		if (roundmode == FE_DEC_TONEAREST && trailzero &&
		  (digits[index-1] & 1) == 0) break;
		if (roundmode == FE_DEC_UPWARD && !trailzero) break;
		if (roundmode == FE_DEC_DOWNWARD && !trailzero) break;
		if (roundmode == 5 && trailzero) break;
		if (roundmode == 6 && trailzero) break;
	      }
	  
            while (digits[--index] == '9') digits[index] = '0';
            digits[index]++;
            if (index < n) { n--; sig++; }
          } while (0);
      }
    
    /* calculate decimal point, adjust prec and exp if necessary */
    if (spec == 'f')
      {
	decpt = n + sig + exp;
      }
    else if (spec == 'a' && -(prec+5) <= exp && exp <= 0 && exp+sig <= prec)
      {
	spec = 'f';
	prec -= exp+sig;
	decpt = n + sig + exp;
      }
    else if (spec == 'g' && -4 < exp+sig && exp+sig <= prec)
      {
	spec = 'f';
	prec -= exp+sig;
	decpt = n + sig + exp;
      }
    else
      {
        if (spec != 'e') prec--;
	exp += sig-1;
	decpt = n + 1;
      }

    /* remove trailing zeroes for %g */
    if (tolower(info->spec) == 'g')
      {
        while (prec > 0 && decpt+prec > len) prec--;
	while (prec > 0 && digits[decpt+prec-1] == '0') prec--;
      }

    /* remove trailing zeroes for %a, but only if they are not significant */
    if (tolower(info->spec) == 'a')
      {
        while (prec > 0 && decpt+prec > len) prec--;
	while (prec > 0 && decpt+prec > n+sig && digits[decpt+prec-1] == '0') prec--;
      }
      

    /* digits to the left of the decimal pt. */
    if (n < decpt)
      { 
        width -= decpt - n;
	if (grouping) width -= (decpt-n)/3;
      }
    else width--;  /* zero */
  
    /* digits to the right of the decimal pt. */
    if (prec > 0) width -= 1 + prec;
    else if (info->alt) width -= 1;
  
    if (spec != 'f')
      {
        width -= 3;
	if (0!=(exp/10) || spec!='a') --width;
	if (0!=(exp/100)) --width;
	if (0!=(exp/1000)) --width;
      }
  
    if (is_neg || info->showsign || info->space) width--;

    if (!info->left && info->pad != '0' && width > 0)
      PADN (info->pad, width);

    if (is_neg)
      outchar ('-');
    else if (info->showsign)
      outchar ('+');
    else if (info->space)
      outchar (' ');

    if (!info->left && info->pad == '0' && width > 0)
      PADN ('0', width);

  /* print zero, decimal point and leading zeroes if needed */
  if (decpt <= n)
    {
      outchar ('0');
      n = decpt;
      if (n < 0)
        {
          outchar (wide ? decimalwc.wc : *decimal);
          while (n < 0 && n < decpt + prec)
	    {
              outchar ('0');
              n++;
            }
        }
    }
  /* print digits */
  while (n < len && n < decpt + prec)
    {
      if (n == decpt) 
        outchar (wide ? decimalwc.wc : *decimal);
      else if (grouping && n < decpt && (decpt-n)%3 == 0)
        outchar (wide ? thousands_sepwc : *thousands_sep);
      outchar (digits[n]);
      n++;
    }
  /* print trailing zeroes */
  while (n < decpt + prec)
    {
      if (n == decpt) 
        outchar (wide ? decimalwc.wc : *decimal);
      else if (grouping && n < decpt && (decpt-n)%3 == 0)
        outchar (wide ? thousands_sepwc : *thousands_sep);
      outchar ('0');
      n++;
    }
  /* print decimal point, if needed */
  if (n == decpt && info->alt) outchar (wide ? decimalwc.wc : *decimal);
  
  
  if (spec != 'f')
   {
     outchar (isupper(info->spec) ? 'E' : 'e');
     if (exp < 0) 
       { outchar ('-'); n = -exp; }
     else
       { outchar ('+'); n = exp; }
     if (n >= 1000) outchar ('0'+((n/1000)%10)); 
     if (n >= 100) outchar ('0'+((n/100)%10));
     if (n >= 10 || spec!='a') outchar ('0'+((n/10)%10));
     outchar ('0'+(n%10));
 
   }
  if (info->left && width > 0)
    PADN (info->pad, width);
} 

   return 0;
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

  /* Currently GLIBC's printf hooks only support one override so we make it
   * _Decimal64 by registering last.  */
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
