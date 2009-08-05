#if !defined(DECCOMMONSYMBOLS)
#define DECCOMMONSYMBOLS

#ifdef IN_LIBGCC2
#define DECCOMBFROM __decCOMBFROM
#define DECCOMBMSD __decCOMBMSD
#endif

#ifdef __STDC_DEC_FP__
#define DECCOMBFROM ___decCOMBFROM
#define DECCOMBMSD ___decCOMBMSD
#endif

#endif
