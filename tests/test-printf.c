#ifndef __STDC_WANT_DEC_FP__
#define __STDC_WANT_DEC_FP__
#endif

#include <float.h>
#include <dfp.h>
#include <stdio.h>
#include <wchar.h> /* This should pick up the libdfp wchar in dfp/wchar.h.  */

int main ()
{
	//_Decimal32 d32 = 1.11111111111DF;
	//_Decimal128 d128 = 0.000033333DL;
	_Decimal64 d64 = 2.22222222222DD;

	double d = 1.234567;
	printf("%e.\n",d);

	register_printf_dfp();
	printf("%Df.\n",d64);

	d = 3.1415962;
	printf("%e.\n",d);
	return 0;
}
