#ifndef _DECIMAL64_H
# define _DECIMAL64_H

#include <stdint.h>
#include "decContext.h"
#include "decNumber.h"

#define DECIMAL64_Bytes  8		/* length		      */

#define DECIMAL64_Emax   384		/* maximum adjusted exponent  */
#define DECIMAL64_Emin  -383		/* minimum adjusted exponent  */

/* Decimal 64-bit type, accessible by bytes			      */
typedef struct {
  uint8_t bytes[DECIMAL64_Bytes];	/* decimal64: 1, 5, 8, 50 bits*/
} decimal64;

#define DECIMAL_NaN     0x7c		/* 0 11111 00 NaN	      */
#define DECIMAL_sNaN    0x7e		/* 0 11111 10 sNaN	      */
#define DECIMAL_Inf     0x78		/* 0 11110 00 Infinity	      */

#include "decimal64Symbols.h"

/* String conversions 					      */
decimal64 * decimal64FromString(decimal64 *, const char *, decContext *);
char * decimal64ToString(const decimal64 *, char *);
char * decimal64ToEngString(const decimal64 *, char *);

/* decNumber conversions					      */
decimal64 * decimal64FromNumber(decimal64 *, const decNumber *,
				decContext *);
decNumber * decimal64ToNumber(const decimal64 *, decNumber *);

/* Format-dependent utilities 				      */
uint32_t    decimal64IsCanonical(const decimal64 *);
decimal64 * decimal64Canonical(decimal64 *, const decimal64 *);

#endif
