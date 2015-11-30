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

asm ("__dpd_floatsidd  = __GI___dpd_floatsidd");
asm ("__dpd_floatditd  = __GI___dpd_floatditd");
asm ("__dpd_floatsitd  = __GI___dpd_floatsitd");
asm ("__dpd_floatdidd  = __GI___dpd_floatdidd");
asm ("__dpd_floatsisd  = __GI___dpd_floatsisd");

asm ("__dpd_floatunsditd  = __GI___dpd_floatditd");

#ifdef HAVE_UINT128_T
asm ("__dpd_floattitd  = __GI___dpd_floattitd");

asm ("__dpd_floatunstitd  = __GI___dpd_floatunstitd");
#endif

asm ("__dpd_trunctddd2 = __GI___dpd_trunctddd2");
asm ("__dpd_truncddsd2 = __GI___dpd_truncddsd2");

asm ("__dpd_extendsddf = __GI___dpd_extendsddf");
asm ("__dpd_extenddfdd = __GI___dpd_extenddfdd");
asm ("__dpd_extenddftd = __GI___dpd_extenddftd");
asm ("__dpd_extendsdtd2 = __GI___dpd_extendsdtd2");
asm ("__dpd_extendsddd2 = __GI___dpd_extendsddd2");
asm ("__dpd_extendddtd2 = __GI___dpd_extendddtd2");

asm ("__dpd_truncdddf  = __GI___dpd_truncdddf");
asm ("__dpd_trunctddf  = __GI___dpd_trunctddf");
asm ("__dpd_truncdfsd  = __GI___dpd_truncdfsd");
asm ("__dpd_trunctdsd2 = __GI___dpd_trunctdsd2");

asm ("__dpd_fixddsi = __GI___dpd_fixddsi");
asm ("__dpd_fixsdsi = __GI___dpd_fixsdsi");
asm ("__dpd_fixdddi = __GI___dpd_fixdddi");
asm ("__dpd_fixtdsi = __GI___dpd_fixtdsi");
asm ("__dpd_fixsddi = __GI___dpd_fixsddi");
asm ("__dpd_fixtddi = __GI___dpd_fixtddi");

asm ("__dpd_fixunssddi = __GI___dpd_fixunssddi");
asm ("__dpd_fixunsdddi = __GI___dpd_fixunsdddi");
asm ("__dpd_fixunstddi = __GI___dpd_fixunstddi");

#ifdef HAVE_UINT128_T
asm ("__dpd_fixtdti = __GI___dpd_fixtdti");

asm ("__dpd_fixunstdti = __GI___dpd_fixunstdti");
#endif


asm ("__dpd_addsd3 = __GI___dpd_addsd3");
asm ("__dpd_addtd3 = __GI___dpd_addtd3");
asm ("__dpd_adddd3 = __GI___dpd_adddd3");

asm ("__dpd_subdd3 = __GI___dpd_subdd3");
asm ("__dpd_subsd3 = __GI___dpd_subsd3");
asm ("__dpd_subtd3 = __GI___dpd_subtd3");

asm ("__dpd_divsd3 = __GI___dpd_divsd3");
asm ("__dpd_divtd3 = __GI___dpd_divtd3");
asm ("__dpd_divdd3 = __GI___dpd_divdd3");

asm ("__dpd_mulsd3 = __GI___dpd_mulsd3");
asm ("__dpd_multd3 = __GI___dpd_multd3");
asm ("__dpd_muldd3 = __GI___dpd_muldd3");

asm ("__dpd_gtdd2 = __GI___dpd_gtdd2");
asm ("__dpd_gtds2 = __GI___dpd_gtsd2");
asm ("__dpd_gtdt2 = __GI___dpd_gttd2");

asm ("__dpd_eqdd2 = __GI___dpd_eqdd2");
asm ("__dpd_eqsd2 = __GI___dpd_eqsd2");
asm ("__dpd_eqtd2 = __GI___dpd_eqtd2");

asm ("__dpd_ltdd2 = __GI___dpd_ltdd2");
asm ("__dpd_ltsd2 = __GI___dpd_ltsd2");
asm ("__dpd_lttd2 = __GI___dpd_lttd2");

asm ("__dpd_gtsd2 = __GI___dpd_gtsd2");
asm ("__dpd_gttd2 = __GI___dpd_gttd2");

asm ("__dpd_letd2 = __GI___dpd_letd2");
asm ("__dpd_getd2 = __GI___dpd_getd2");

asm ("__dpd_netd2 = __GI___dpd_netd2");
asm ("__dpd_nesd2 = __GI___dpd_nesd2");
asm ("__dpd_nedd2 = __GI___dpd_nedd2");

#endif
