/* Hacks for symbol bindings.

   Copyright (C) 2014 Free Software Foundation, Inc.

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
double  __dpd_extendsddf (_Decimal32);
hidden_proto (__dpd_extendsddf)
asm ("__dpd_extendsddf = __GI___dpd_extendsddf");

double __dpd_truncdddf (_Decimal64);
hidden_proto (__dpd_truncdddf)
asm ("__dpd_truncdddf  = __GI___dpd_truncdddf");

double __dpd_trunctddf (_Decimal128);
hidden_proto (__dpd_trunctddf)
asm ("__dpd_trunctddf  = __GI___dpd_trunctddf");

_Decimal32 __dpd_truncdfsd (double);
hidden_proto (__dpd_truncdfsd)
asm ("__dpd_truncdfsd  = __GI___dpd_truncdfsd");

_Decimal64  __dpd_extenddfdd (double);
hidden_proto (__dpd_extenddfdd)
asm ("__dpd_extenddfdd = __GI___dpd_extenddfdd");

_Decimal128  __dpd_extenddftd (double);
hidden_proto (__dpd_extenddftd)
asm ("__dpd_extenddftd = __GI___dpd_extenddftd");

_Decimal32 __dpd_trunctdsd2 (_Decimal128);
hidden_proto (__dpd_trunctdsd2)
asm ("__dpd_trunctdsd2 = __GI___dpd_trunctdsd2");
#endif
