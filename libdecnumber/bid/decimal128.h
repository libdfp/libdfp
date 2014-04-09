#ifndef _DECIMAL128_H
# define _DECIMAL128_H

#include <stdint.h>
#include "decContext.h"
#include "decNumber.h"

#define DECIMAL128_Bytes  16		/* length		      */

#define DECIMAL128_Emax   6144		/* maximum adjusted exponent  */
#define DECIMAL128_Emin  -6143		/* minimum adjusted exponent  */

/* Decimal 128-bit type, accessible by bytes			      */
typedef struct {
  uint8_t bytes[DECIMAL128_Bytes]; /* decimal128: 1, 5, 12, 110 bits*/
} decimal128;

#define DECIMAL_NaN     0x7c		/* 0 11111 00 NaN	      */
#define DECIMAL_sNaN    0x7e		/* 0 11111 10 sNaN	      */
#define DECIMAL_Inf     0x78		/* 0 11110 00 Infinity	      */


#include "decimal128Symbols.h"

/* String conversions 					      */
decimal128 * decimal128FromString(decimal128 *, const char *, decContext *);
char * decimal128ToString(const decimal128 *, char *);
char * decimal128ToEngString(const decimal128 *, char *);

/* decNumber conversions					      */
decimal128 * decimal128FromNumber(decimal128 *, const decNumber *,
				  decContext *);
decNumber * decimal128ToNumber(const decimal128 *, decNumber *);

/* Format-dependent utilities 				      */
uint32_t    decimal128IsCanonical(const decimal128 *);
decimal128 * decimal128Canonical(decimal128 *, const decimal128 *);

#endif
