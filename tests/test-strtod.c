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
  {__LINE__, "19e9", 19000000000.0DF, 19000000000.0DD, 19000000000.0DL },
  {__LINE__, "1234.5678910111213e-5",0.01234DF ,0.01234DD ,0.01234DL },
  {0,0,0,0,0 }
};

const char DECLET_DEC_NAN[] = "-0,000,000,000,000,010E-1";

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
  /* DEC_NAN is +0,000,000,000,000,000E-398 so test against that
   * since you can't compare DEC_NAN to DEC_NAN.  */
  {__LINE__, "NaN", DECLET_DEC_NAN, DECLET_DEC_NAN, DECLET_DEC_NAN},
  {0,0,0,0,0 }
};

int main(void) {
  d_type *dptr;
  for (dptr = strtods; dptr->line; dptr++)
    {
      _VC_P(__FILE__,dptr->line,dptr->d32,strtod32(dptr->input,NULL), "%Hf");
      _VC_P(__FILE__,dptr->line,dptr->d64, strtod64(dptr->input,NULL), "%Df");
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
/*

	char *testcases[] = { "12.04","1234.5678910111213e-5", "1.0", "1", "19","19E2","-0.0001","-5","NaN", "INF", "0", "0.0"};
	int i,numCases=12;

	for(i=0; i< numCases; i++) {
		printf("in: %s\n",testcases[i]);
		printf("  out32: %Hf\n  out64: %Df\n  out128: %DDf\n",
				strtod32(testcases[i],NULL),
				strtod64(testcases[i],NULL),
				strtod128(testcases[i],NULL)
				);
	}
*/
}
