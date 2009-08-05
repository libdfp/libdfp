#if !defined(DECIMAL64SYMBOLS)
#define DECIMAL64SYMBOLS

#ifdef IN_LIBGCC2
#define decDigitsFromDPD __decDigitsFromDPD
#define decDigitsToDPD __decDigitsToDPD
#define decimal64Canonical __decimal64Canonical
#define decimal64FromNumber __decimal64FromNumber
#define decimal64FromString __decimal64FromString
#define decimal64IsCanonical __decimal64IsCanonical
#define decimal64ToEngString __decimal64ToEngString
#define decimal64ToNumber __decimal64ToNumber
#define decimal64ToString __decimal64ToString
#define COMBEXP __decnnCOMBEXP
#define COMBMSD __decnnCOMBMSD

/* DPD2BIN and BIN2DPD are used in support for decimal32/decimal64/decimal128
   and also in support for decSingle/decDouble/decQuad.  Rename them in case
   both types of support are used in the same executable.  */
#undef DPD2BIN
#define DPD2BIN __decnnDPD2BIN
#undef BIN2DPD
#define BIN2DPD __decnnBIN2DPD
#endif

#ifdef __STDC_DEC_FP__
#define decDigitsFromDPD ___decDigitsFromDPD
#define decDigitsToDPD ___decDigitsToDPD
#define decimal64Canonical ___decimal64Canonical
#define decimal64FromNumber ___decimal64FromNumber
#define decimal64FromString ___decimal64FromString
#define decimal64IsCanonical ___decimal64IsCanonical
#define decimal64ToEngString ___decimal64ToEngString
#define decimal64ToNumber ___decimal64ToNumber
#define decimal64ToString ___decimal64ToString
#define COMBEXP ___decnnCOMBEXP
#define COMBMSD ___decnnCOMBMSD

/* DPD2BIN and BIN2DPD are used in support for decimal32/decimal64/decimal128
   and also in support for decSingle/decDouble/decQuad.  Rename them in case
   both types of support are used in the same executable.  */
#undef DPD2BIN
#define DPD2BIN ___decnnDPD2BIN
#undef BIN2DPD
#define BIN2DPD ___decnnBIN2DPD
#endif

#endif
