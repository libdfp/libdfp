#if !defined(DECPACKEDSYMBOLS)
#define DECPACKEDSYMBOLS

#ifdef IN_LIBGCC2
#define decPackedFromNumber __decPackedFromNumber
#define decPackedToNumber __decPackedToNumber
#endif

#ifdef __STDC_DEC_FP__
#define decPackedFromNumber ___decPackedFromNumber
#define decPackedToNumber ___decPackedToNumber
#endif

#endif
