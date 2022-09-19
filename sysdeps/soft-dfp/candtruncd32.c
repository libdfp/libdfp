/* Take 3 _Decimal128 numbers spanning 39 digits and
   convert them into one properly rounding _Decimal32 value.

   This is meant to be used internally to assist in rounding
   __int128 values to _Decimal32/64.

   Copyright (C) 2015 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Paul E. Murphy <murphyp@linux.vnet.ibm.com>

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

/* This will take at most 39 digits.  This should always be 39
   for this function.  It must defined before including this
   file as decimal*.h will attempt to define it.  */
#define DECNUMDIGITS 39

#ifndef _DECIMAL_SIZE
#  define _DECIMAL_SIZE 32
#  include <decimal32.h>
#  define FUNC_NAME combine_and_truncd32
#endif

#include "decimal128.h"
#include "dfpmacro.h"

#include "convert_helpers.h"

#include <stdio.h>

DEC_TYPE
FUNC_NAME (_Decimal128 hi, _Decimal128 mid, _Decimal128 low)
{
  DEC_TYPE result;

  /*  hi = m_hi *  10^34
   * mid = m_mid * 10^17
   * low = m_low * 10^0
   *
   * Note, m_hi is only at most 5 digits for int128.
   */
  decNumber dn_hi, dn_mid, dn_low, dn_result;
  decNumber dn_hi_s, dn_mid_s, dn_result_s;
  decNumber dn_17, dn_34;
  decContext context;

  decContextDefault (&context, DEC_INIT_DECIMAL128);
  /* Hack, we're using 39 digits here. */
  context.digits = 39;

  decNumberFromInt32(&dn_17, 17);
  decNumberFromInt32(&dn_34, 34);

  decimal128ToNumber((decimal128*)&hi, &dn_hi);
  decimal128ToNumber((decimal128*)&mid, &dn_mid);
  decimal128ToNumber((decimal128*)&low, &dn_low);

  /* Rotate addends into proper position.  */
  decNumberShift(&dn_hi_s,&dn_hi,&dn_34,&context);
  decNumberShift(&dn_mid_s,&dn_mid,&dn_17,&context);

  /* Sum the three components.  */
  decNumberAdd(&dn_result_s, &dn_hi_s, &dn_mid_s, &context);
  decNumberAdd(&dn_result, &dn_result_s, &dn_low, &context);

  /* Convert to the destination format.  */
  FUNC_CONVERT_FROM_DN (&dn_result, &result, &context);

  /* Don't care about exceptions here... I don't think.  */
  return result;
}
