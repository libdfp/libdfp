/* Functions to print the DPD declets for use with printf %Z

   Copyright (C) 2006, 2007 IBM Corporation.
   Copyright (C) 2007, 2009 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Steve Munroe <sjmunroe@us.ibm.com>
              Ryan S. Arnold <rsa@us.ibm.com>

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

#include <float.h>
#include <math.h>
#include <fenv.h>
#include <string.h>

#include <decode.h>
#include <dpd-private.h>

/* This version is used by all DPD implementations.  */

#undef DEBUG_PRINT
#define DEBUG_PRINT 0
char * __decoded32 (_Decimal32 a, char * str)
{
    union ieee754r_Decimal32 d;
    struct ieee754r_c_field c_f;
    int exp;
    char sign;
    char lmd;
    const char *cc0;
    const char *cc1;
    char * after_exp = NULL;

    d.sd = a;
    if (d.ieee.negative)
        sign = '-';
    else
        sign = '+';

    c_f =  c_decoder[d.ieee.c];
    exp = c_f.lm_exp << DECIMAL32_BEC_bits;
    exp += d.ieee.bec;
    exp -= DECIMAL32_Bias;
    lmd = '0' + c_f.lmd;

    cc0 = &dpd_to_char[d.ieee.cc0][0];
    cc1 = &dpd_to_char[d.ieee.cc1][0];

    str[0] = sign;
    str[1] = lmd;
    str[2] = ',';
    __dpd_cpydeclet(&str[3], cc0);
    str[6] = ',';
    __dpd_cpydeclet(&str[7], cc1);
    str[10] = 'E';
    after_exp = __dpd_exptostr(&str[11], exp);
    if (after_exp != NULL)
        *after_exp = '\0';

    return str;
}
weak_alias (__decoded32, decoded32)
hidden_def(__decoded32)

char * __decoded64 ( _Decimal64 a, char * str )
{
    union ieee754r_Decimal64 d;
    struct ieee754r_c_field c_f;
    int exp, i;
    char sign;
    char lmd;
    const char *cc0;
    const char *cc1;
    const char *cc2;
    const char *cc3;
    const char *cc4;
    char * after_exp = NULL;

    d.dd = a;
    if (d.ieee.negative)
        sign = '-';
    else
        sign = '+';

    c_f =  c_decoder[d.ieee.c];
    exp = c_f.lm_exp << DECIMAL64_BEC_bits;
    exp += d.ieee.bec;
    exp -= DECIMAL64_Bias;
    lmd = '0' + c_f.lmd;

    cc0 = &dpd_to_char[d.ieee.cc0][0];
    /* Packed fields crossing a word boundary require special handling. */
    i = d.ieee.cc1H8 << 2;
    i = i + d.ieee.cc1L2;
    cc1 = &dpd_to_char[i][0];
    cc2 = &dpd_to_char[d.ieee.cc2][0];
    cc3 = &dpd_to_char[d.ieee.cc3][0];
    cc4 = &dpd_to_char[d.ieee.cc4][0];

    str[0] = sign;
    str[1] = lmd;
    str[2] = ',';
    __dpd_cpydeclet(&str[3], cc0);
    str[6] = ',';
    __dpd_cpydeclet(&str[7], cc1);
    str[10] = ',';
    __dpd_cpydeclet(&str[11], cc2);
    str[14] = ',';
    __dpd_cpydeclet(&str[15], cc3);
    str[18] = ',';
    __dpd_cpydeclet(&str[19], cc4);
    str[22] = 'E';
    after_exp = __dpd_exptostr(&str[23], exp);
    if (after_exp != NULL)
        *after_exp = '\0';

    return str;
}
weak_alias (__decoded64, decoded64)
hidden_def(__decoded64)

char * __decoded128 ( _Decimal128 a, char * str )
{
    union ieee754r_Decimal128 d;
    struct ieee754r_c_field c_f;
    int exp;/*, i; */
    char sign;
    char lmd;
    const char *cc0;
    const char *cc1;
    const char *cc2;
    const char *cc3;
    const char *cc4;
    const char *cc5;
    const char *cc6;
    const char *cc7;
    const char *cc8;
    const char *cc9;
    const char *cc10;
    char * after_exp = NULL;

    d.td = a;
    if (d.ieee.negative)
        sign = '-';
    else
        sign = '+';

    c_f =  c_decoder[d.ieee.c];
    exp = c_f.lm_exp << DECIMAL128_BEC_bits;
    exp += d.ieee.bec;
    exp -= DECIMAL128_Bias;
    lmd = '0' + c_f.lmd;

    cc0 = &dpd_to_char[d.ieee.cc0][0];
    /* Packed fields crossing a word boundary require special handling. */
    cc1 = &dpd_to_char[((d.ieee.cc1H4<<6) + d.ieee.cc1L6)][0];
    cc2 = &dpd_to_char[d.ieee.cc2][0];
    cc3 = &dpd_to_char[d.ieee.cc3][0];
    cc4 = &dpd_to_char[((d.ieee.cc4H6<<4) + d.ieee.cc4L4)][0];
    cc5 = &dpd_to_char[d.ieee.cc5][0];
    cc6 = &dpd_to_char[d.ieee.cc6][0];
    cc7 = &dpd_to_char[((d.ieee.cc7H8<<2) + d.ieee.cc7L2)][0];
    cc8 = &dpd_to_char[d.ieee.cc8][0];
    cc9 = &dpd_to_char[d.ieee.cc9][0];
    cc10 = &dpd_to_char[d.ieee.cc10][0];

    str[0] = sign;
    str[1] = lmd;
    str[2] = ',';
    __dpd_cpydeclet(&str[3], cc0);
    str[6] = ',';
    __dpd_cpydeclet(&str[7], cc1);
    str[10] = ',';
    __dpd_cpydeclet(&str[11], cc2);
    str[14] = ',';
    __dpd_cpydeclet(&str[15], cc3);
    str[18] = ',';
    __dpd_cpydeclet(&str[19], cc4);
    str[22] = ',';
    __dpd_cpydeclet(&str[23], cc5);
    str[26] = ',';
    __dpd_cpydeclet(&str[27], cc6);
    str[30] = ',';
    __dpd_cpydeclet(&str[31], cc7);
    str[34] = ',';
    __dpd_cpydeclet(&str[35], cc8);
    str[38] = ',';
    __dpd_cpydeclet(&str[39], cc9);
    str[42] = ',';
    __dpd_cpydeclet(&str[43], cc10);
    str[46] = 'E';
    after_exp = __dpd_exptostr(&str[47], exp);
    if (after_exp != NULL)
        *after_exp = '\0';

    return str;
}
weak_alias (__decoded128, decoded128)
hidden_def(__decoded128)
