/* Hacks for symbol bindings.

   Copyright (C) 2014-2015 Free Software Foundation, Inc.

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

#if !defined __ASSEMBLER__ && !defined NOT_IN_libdfp && defined SHARED

/* GCC generates intrisics calls to some functions for _Decimal to/from
   convertions. These bindings avoids intra library  PLT calls generations,
   since libdfp provides such symbols.  */

asm ("__bid_floatsidd  = __GI___bid_floatsidd");
asm ("__bid_floatditd  = __GI___bid_floatditd");
asm ("__bid_floatsitd  = __GI___bid_floatsitd");
asm ("__bid_floatdidd  = __GI___bid_floatdidd");
asm ("__bid_floatdisd  = __GI___bid_floatdisd");
asm ("__bid_floatsisd  = __GI___bid_floatsisd");

asm ("__bid_floatunsdisd  = __GI___bid_floatunsdisd");
asm ("__bid_floatunsdidd  = __GI___bid_floatunsdidd");

#ifdef __SIZEOF_INT128__
asm ("__bid_floattitd  = __GI___bid_floattitd");
asm ("__bid_floatunsditd  = __GI___bid_floatunsditd");
#endif

asm ("__bid_truncdddf  = __GI___bid_truncdddf");
asm ("__bid_trunctddf  = __GI___bid_trunctddf");
asm ("__bid_trunctddd2 = __GI___bid_trunctddd2");
asm ("__bid_truncddsd2 = __GI___bid_truncddsd2");
asm ("__bid_truncdfsd  = __GI___bid_truncdfsd");
asm ("__bid_trunctdsd2 = __GI___bid_trunctdsd2");

asm ("__bid_extendsddf = __GI___bid_extendsddf");
asm ("__bid_extenddftd = __GI___bid_extenddftd");
asm ("__bid_extenddfdd = __GI___bid_extenddfdd");
asm ("__bid_extendsdtd2 = __GI___bid_extendsdtd2");
asm ("__bid_extendddtd2 = __GI___bid_extendddtd2");

asm ("__bid_fixsdsi = __GI___bid_fixsdsi");
asm ("__bid_fixdddi = __GI___bid_fixdddi");
asm ("__bid_fixddsi = __GI___bid_fixddsi");
asm ("__bid_fixsddi = __GI___bid_fixsddi");
asm ("__bid_fixtddi = __GI___bid_fixtddi");
asm ("__bid_fixtdsi = __GI___bid_fixtdsi");

asm ("__bid_fixunsdddi = __GI___bid_fixunsdddi");
asm ("__bid_fixunssddi = __GI___bid_fixunssddi");

asm ("__bid_addsd3 = __GI___bid_addsd3");
asm ("__bid_addtd3 = __GI___bid_addtd3");
asm ("__bid_adddd3 = __GI___bid_adddd3");

asm ("__bid_subdd3 = __GI___bid_subdd3");
asm ("__bid_subsd3 = __GI___bid_subsd3");
asm ("__bid_subtd3 = __GI___bid_subtd3");

asm ("__bid_divsd3 = __GI___bid_divsd3");
asm ("__bid_divtd3 = __GI___bid_divtd3");
asm ("__bid_divdd3 = __GI___bid_divdd3");

asm ("__bid_gtdd2 = __GI___bid_gtdd2");
asm ("__bid_gtds2 = __GI___bid_gtsd2");
asm ("__bid_gtdt2 = __GI___bid_gttd2");

asm ("__bid_mulsd3 = __GI___bid_mulsd3");
asm ("__bid_multd3 = __GI___bid_multd3");
asm ("__bid_muldd3 = __GI___bid_muldd3");

asm ("__bid_gtsd2 = __GI___bid_gtsd2");
asm ("__bid_gttd2 = __GI___bid_gttd2");

asm ("__bid_ltdd2 = __GI___bid_ltdd2");
asm ("__bid_ltsd2 = __GI___bid_ltsd2");
asm ("__bid_lttd2 = __GI___bid_lttd2");

asm ("__bid_eqdd2 = __GI___bid_eqdd2");
asm ("__bid_eqsd2 = __GI___bid_eqsd2");
asm ("__bid_eqtd2 = __GI___bid_eqtd2");

asm ("__bid_nedd2 = __GI___bid_nedd2");
asm ("__bid_nesd2 = __GI___bid_nesd2");
asm ("__bid_netd2 = __GI___bid_netd2");

asm ("__bid_letd2 = __GI___bid_letd2");
asm ("__bid_getd2 = __GI___bid_getd2");

asm ("__bid_unordsd2 = __GI___bid_unordsd2");
asm ("__bid_unorddd2 = __GI___bid_unorddd2");
asm ("__bid_unordtd2 = __GI___bid_unordtd2");

#endif
