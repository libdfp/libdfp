/* Decimal Float fe_dec_getround and fe_dec_setround definitions.

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2007, 2009 Free Software Foundation, Inc.

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

   Please see dfp/COPYING.txt for more information.  */

#include <fenv.h>
#include <dfpfenv_private.h>

int __fe_dec_setround(int rounding_direction)
{
  /* The rounding modes are as follows:
   *
   * C Rounding Mode:		(DRN) Hardware Description
   * 				DecNumber Rounding Mode Equivalent
   *
   * FE_DEC_TONEAREST:		(000) Round to nearest, ties to even.
   * 				DEC_ROUND_HALF_EVEN
   *
   * FE_DEC_TOWARDZERO:		(001) Round toward zero.
   * 				DEC_ROUND_DOWN
   *
   * FE_DEC_UPWARD:		(010) Round toward +Infinity
   * 				DEC_ROUND_CEILING
   *
   * FE_DEC_DOWNWARD:		(011) Round toward -Infinity
   * 				DEC_ROUND_FLOOR
   *
   * FE_DEC_TONEARESTFROMZERO:	(100) Round to nearest, ties away from zero
   * 				DEC_ROUND_HALF_UP
   *
   * 5:				(101) Round to nearest, ties toward zero
   * 				DEC_ROUND_HALF_DOWN
   *
   * 6:				(110) Round away from zero
   * 				DEC_ROUND_UP
   *
   * 7:				(111) Round for prepare for shorter precision
   * 				Not supported by decNumber.  */

  /* High order bits 29-31 contain DFP Rounding control (DRN) and bit 28 is
   * reserved for future rounding control.  The new Power6[x] form of the mtfsfi
   * instruction can be used against these four bits to set the rounding mode.
   * Then new form of the mtfsfi instruction for Power6[x] adds the 'W' parameter:

       mtfsfi BF,U,W
	 BF: Field [0-7: Bits 28-31 are field 7 of the high order word]
	 U:  Bit Pattern [Bit 28 is reserved, use 0000-0111]
	 W:  Word [Implicit '0' for bits 32:63 {low order word.}]
		  ['1' indicates the bits 0:31 {high order word}.] */

  switch(rounding_direction)
    {
      case FE_DEC_TONEAREST:
	asm ("mtfsfi 7, 0, 1\n");
	break;
      case FE_DEC_TOWARDZERO:
	asm ("mtfsfi 7, 1, 1\n");
	break;
      case FE_DEC_UPWARD:
	asm ("mtfsfi 7, 2, 1\n");
	break;
      case FE_DEC_DOWNWARD:
	asm ("mtfsfi 7, 3, 1\n");
	break;
      case FE_DEC_TONEARESTFROMZERO:
	asm ("mtfsfi 7, 4, 1\n");
	break;
      case 5: /* Allow covert setting of this rounding mode.  */
	asm ("mtfsfi 7, 5, 1\n");
	break;
      case 6: /* Allow covert setting of this rounding mode.  */
	asm ("mtfsfi 7, 6, 1\n");
	break;
      case 7: /* Allow covert setting of this rounding mode.  */
	asm ("mtfsfi 7, 7, 1\n");
	break;
      default:
	return 1;
    }
  return 0;
}
strong_alias(__fe_dec_setround, fe_dec_setround)
hidden_def (__fe_dec_setround)

int __fe_dec_getround(void)
{
  union {
    double as_double;
    struct { unsigned int dummy, dummy2: 28, drn:3; };
  } fpscr;

  /* On Power6, read the fpscr into a double union using mffs
     instruction, then convert the DRN rounding mode to DEC spec. */
  asm ("mffs %0\n" : "=f" (fpscr.as_double) : );
  switch (fpscr.drn) {
    case 0: return FE_DEC_TONEAREST;
    case 1: return FE_DEC_TOWARDZERO;
    case 2: return FE_DEC_UPWARD;
    case 3: return FE_DEC_DOWNWARD;
    case 4: return FE_DEC_TONEARESTFROMZERO;
    case 5: return 5;
    case 6: return 6;
    case 7: return 7;
    default: return FE_DEC_TONEAREST; /* Default  */
  }
}
strong_alias(__fe_dec_getround, fe_dec_getround)
hidden_def(__fe_dec_getround)

extern int (*__printf_dfp_getround_callback)(void);

extern void __init_printf_dfp_getround (void);

void __attribute__ ((constructor)) 
__init_printf_dfp_getround (void)
{
  __printf_dfp_getround_callback = &__fe_dec_getround;
}
hidden_def (__init_printf_dfp_getround);
