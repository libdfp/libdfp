/* Common fix*ti and fixsdti implementation.

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
   if not, write to the Free Software Foundation, Inc., 59 Temple Place,
   Suite 330, Boston, MA 02111-1307 USA.

   Please see libdfp/COPYING.txt for more information.  */

#ifndef SRC_SIZE
#  define SRC_SIZE 32
#  define FUNC fixsdti
#  define SHIFT_EXP 7
#  define DSUFF DF
#  define SIGNED 1
#endif

#define _DECIMAL_SIZE SRC_SIZE

#include <dfpacc.h>
#include <stdint.h>
#include <dfp/math.h>
#include <dfpmacro.h>
#include <ieee754r_private.h>

#if SIGNED == 1
#  if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#    define MAX_RET_VAL {.di = {0x7FFFFFFFFFFFFFFFLL, 0xFFFFFFFFFFFFFFFFLL}}
#    define MIN_RET_VAL {.di = {0x8000000000000000LL,                  0LL}}
#  else
#    define MAX_RET_VAL {.di = {0xFFFFFFFFFFFFFFFFLL, 0x7FFFFFFFFFFFFFFFLL}}
#    define MIN_RET_VAL {.di = {                 0LL, 0x8000000000000000LL}}
#  endif
#  if SRC_SIZE == 32
#    define MIN_VAL (-170141100000000000000000000000000000000.DF)
#    define MAX_VAL ( 170141100000000000000000000000000000000.DF)
#  elif SRC_SIZE == 64
#    define MIN_VAL (-170141183460469200000000000000000000000.DD)
#    define MAX_VAL ( 170141183460469200000000000000000000000.DD)
#  else
#    define MIN_VAL (-170141183460469231731687303715884100000.DL)
#    define MAX_VAL ( 170141183460469231731687303715884100000.DL)
#  endif
#  define RET_TYPE __int128
#  define QUICK_CONVERT(_x) ({ long long int _a_di = _x; _a_di;})
#else
#  define MAX_RET_VAL {.di = {0xFFFFFFFFFFFFFFFFLL, 0xFFFFFFFFFFFFFFFFLL}}
#  define MIN_RET_VAL {.di = {0LL, 0LL}}
#  define MIN_VAL PASTE(0., DSUFF)
#  if SRC_SIZE == 32
#    define MAX_VAL (340282300000000000000000000000000000000.DF)
#  elif SRC_SIZE == 64
#    define MAX_VAL (340282366920938400000000000000000000000.DD)
#  else
#    define MAX_VAL (340282366920938463463374607431768200000.DL)
#  endif
#  define RET_TYPE unsigned __int128
#  define QUICK_CONVERT(_x) ({ unsigned long long int _a_di = _x; _a_di;})
#endif

#define TRUNC(_x) (PASTE(__truncd, SRC_SIZE)(_x))
#define FREXP(_x,_y) (PASTE(__frexpd,SRC_SIZE)(_x,_y))
#define CLASSIFY(_x) (PASTE(__fpclassifyd,SRC_SIZE)(_x))
#define SHIFT_EXP_VAL PASTE(PASTE(1e,SHIFT_EXP),PASTE(DSUFF,))
#define SIGNBIT(_x) (PASTE(__signbitd,SRC_SIZE)(_x))

#if SIGNED == 1
#  define FABS(_x) (PASTE(__fabsd,SRC_SIZE)(_x))
#else
#  define FABS(_x) _x
#endif

extern const __int128 exp10_ti[39];

RET_TYPE __BACKEND_(FUNC) (DEC_TYPE a)
{
  union ret_type
    {
    RET_TYPE ti;
    uint64_t di[2];
    };
  union ret_type min = MIN_RET_VAL;
  union ret_type max = MAX_RET_VAL;

  switch (CLASSIFY (a))
  {
    case FP_ZERO:
      return 0;

    case FP_INFINITE:
      DFP_EXCEPT (FE_INVALID);
      return SIGNBIT(a) ? min.ti : max.ti;

    case FP_NAN:
      DFP_EXCEPT (FE_INVALID);
      return min.ti;
  }

  if(a < MIN_VAL || a > MAX_VAL)
  {
    DFP_EXCEPT (FE_INVALID);
    return (a < MIN_VAL) ? min.ti : max.ti;
  }

  /* A 64 bit mantissa is sufficient for dd/sd conversions. */
#if SRC_SIZE <= 64
  RET_TYPE result;
  int e;

  if (FABS(a) < SHIFT_EXP_VAL)
  {
    return (RET_TYPE) QUICK_CONVERT (a);
  }

  DEC_TYPE a_int  = TRUNC (a);
  DEC_TYPE a_norm = FREXP (a_int, &e);


  a_norm *= SHIFT_EXP_VAL;
  e -= SHIFT_EXP;
  /* Force a 64b conversion of the mantissa */
  result = ((int64_t) a_norm);
  if (e > 0)
    return result * (RET_TYPE) exp10_ti[e];
  else if (e < 0)
    return result / (RET_TYPE) exp10_ti[-e];
  else
    return result;
#else
  /* This requires specific knowledge of the encoding.  Punt it.  */
  return D128_CONVERSION(a);
#endif
}

hidden_def (__BACKEND_(FUNC))
