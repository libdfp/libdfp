/* Hacks for libdfp  */

#if !defined __ASSEMBLER__ && !defined NOT_IN_libdfp && defined SHARED
/* GCC generates intrisics calls to some functions for _Decimal to/from
   convertions. These bindings avoids intra library  PLT calls generations,
   since libdfp provides such symbols.  */
double  __bid_extendsddf (_Decimal32);
hidden_proto (__bid_extendsddf)
asm ("__bid_extendsddf = __GI___bid_extendsddf");

double __bid_truncdddf (_Decimal64);
hidden_proto (__bid_truncdddf)
asm ("__bid_truncdddf  = __GI___bid_truncdddf");

double __bid_trunctddf (_Decimal128);
hidden_proto (__bid_trunctddf)
asm ("__bid_trunctddf  = __GI___bid_trunctddf");

_Decimal32 __bid_truncdfsd (double);
hidden_proto (__bid_truncdfsd)
asm ("__bid_truncdfsd  = __GI___bid_truncdfsd");

_Decimal64  __bid_extenddfdd (double);
hidden_proto (__bid_extenddfdd)
asm ("__bid_extenddfdd = __GI___bid_extenddfdd");

_Decimal128  __bid_extenddftd (double);
hidden_proto (__bid_extenddftd)
asm ("__bid_extenddftd = __GI___bid_extenddftd");

_Decimal32 __bid_trunctdsd2 (_Decimal128);
hidden_proto (__bid_trunctdsd2)
asm ("__bid_trunctdsd2 = __GI___bid_trunctdsd2");
#endif
