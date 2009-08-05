#if !defined(DECDPDSYMBOLS)
#define DECDPDSYMBOLS

#ifdef IN_LIBGCC2
#define BCD2DPD __decBCD2DPD
#define BIN2BCD8 __decBIN2BCD8
#define BIN2CHAR __decBIN2CHAR
#define BIN2DPD __decBIN2DPD
#define DPD2BCD8 __decDPD2BCD8
#define DPD2BIN __decDPD2BIN
#define DPD2BINK __decDPD2BINK
#define DPD2BINM __decDPD2BINM
#endif

#ifdef __STDC_DEC_FP__
#define BCD2DPD ___decBCD2DPD
#define BIN2BCD8 ___decBIN2BCD8
#define BIN2CHAR ___decBIN2CHAR
#define BIN2DPD ___decBIN2DPD
#define DPD2BCD8 ___decDPD2BCD8
#define DPD2BIN ___decDPD2BIN
#define DPD2BINK ___decDPD2BINK
#define DPD2BINM ___decDPD2BINM
#endif

#endif
