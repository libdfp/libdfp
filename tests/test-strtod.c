#ifndef __STDC_WANT_DEC_FP__
# define __STDC_WANT_DEC_FP__ 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define _WANT_VC 1
#define _WANT_DC 1

#include "scaffold.c"

/* Inspired by GLIBC stdio-common/tfformat.c  */
typedef struct{
  int line;
  const char *input;
  _Decimal32 d32;
  _Decimal64 d64;
  _Decimal128 d128;
} d_type;

d_type strtods[] =
{
  {__LINE__, "12.04", 12.04DF,12.04DD, 12.04DL},
  {__LINE__, "1.0", 1.0DF, 1.0DD, 1.0DL },
  {__LINE__, "1", 1.0DF, 1.0DD, 1.0DL },
  {__LINE__, "0", 0.0DF, 0.0DD, 0.0DL },
  {__LINE__, "0.0", 0.0DF, 0.0DD, 0.0DL },
  {__LINE__, "-0.0001", -0.0001DF, -0.0001DD, -0.0001DL },
  {__LINE__, "inf", DEC_INFINITY, DEC_INFINITY, DEC_INFINITY },
  {__LINE__, "INFINITY", DEC_INFINITY, DEC_INFINITY, DEC_INFINITY },
  {__LINE__, "0.0E+100", DEC_INFINITY, 0.0DD, 0.0DL },
  {__LINE__, "0.01", 0.01DF, 0.01DD, 0.01DL },
  {__LINE__, "0.1", 0.1DF, 0.1DD, 0.1DL },
  {__LINE__, "0.11", 0.11DF, 0.11DD, 0.11DL },
  {__LINE__, "0.21", 0.21DF, 0.21DD, 0.21DL },
  {__LINE__, "1.23456789E-7", 0.0DF, 0.0DD, 0.0DL },
  {__LINE__, "19e9", 19000000000.0DF, 19000000000.0DD, 19000000000.0DL },
  {__LINE__, "1234.5678910111213e-5", 0.01234DF ,0.01234DD ,0.01234DL },
  {__LINE__, "3.14", 3.140000DF, 3.140000DD, 3.140000DL },
  {__LINE__, "3.14e-2", 0.031400DF, 0.031400DD, 0.031400DL },
  {0,0,0,0,0 }
};

const char DECLET32_NAN[] = "+0,000,000E-101";
const char DECLET64_NAN[] = "+0,000,000,000,000,000E-398";
const char DECLET128_NAN[] = "+0,000,000,000,000,000,000,000,000,000,000,000E-6176";

/* Inspired by GLIBC stdio-common/tfformat.c  */
typedef struct{
  int line;
  const char *input;
  const char *d32;
  const char *d64;
  const char *d128;
} d_nan_type;

d_nan_type strtods_nan[] =
{
  /* Compare against the decoded declet for each representation of DEC_NAN since
   * since you can't compare DEC_NAN to DEC_NAN.  */
  {__LINE__, "NaN", DECLET32_NAN, DECLET64_NAN, DECLET128_NAN},
  {0,0,0,0,0 }
};

#include "decode.h"

int main(void) {

  d_type *dptr;

  for (dptr = strtods; dptr->line; dptr++)
    {

      fprintf(stderr, "  strtod32(\"%s\",NULL) == %Hf\n  ", dptr->input, strtod32(dptr->input, NULL));
      _VC_P(__FILE__,dptr->line,dptr->d32,strtod32(dptr->input,NULL), "%Hf");

      fprintf(stderr, "  strtod64(\"%s\",NULL) == %Df\n  ", dptr->input, strtod64(dptr->input, NULL));
      _VC_P(__FILE__,dptr->line,dptr->d64, strtod64(dptr->input,NULL), "%Df");

      fprintf(stderr, "  strtod128(\"%s\",NULL) == %DDf\n  ", dptr->input, strtod128(dptr->input, NULL));
      _VC_P(__FILE__,dptr->line,dptr->d128, strtod128(dptr->input,NULL), "%DDf");
    }

  d_nan_type *dnanptr;
  for (dnanptr = strtods_nan; dnanptr->line; dnanptr++)
    {
      _DC_P(__FILE__,dnanptr->line,dnanptr->d32,strtod32(dnanptr->input,NULL));
      _DC_P(__FILE__,dnanptr->line,dnanptr->d64, strtod64(dnanptr->input,NULL));
      _DC_P(__FILE__,dnanptr->line,dnanptr->d128, strtod128(dnanptr->input,NULL));
    }

  _REPORT();

  /* fail comes from scaffold.c  */
  return fail;
}
