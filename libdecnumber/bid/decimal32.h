#ifndef _DECIMAL32_H
# define _DECIMAL32_H

#include <stdint.h>
#include "decContext.h"
#include "decNumber.h"

#define DECIMAL32_Bytes  4		/* length		      */

#define DECIMAL32_Emax   96		/* maximum adjusted exponent  */
#define DECIMAL32_Emin  -95		/* minimum adjusted exponent  */

/* Decimal 32-bit type, accessible by bytes */
typedef struct {
  uint8_t bytes[DECIMAL32_Bytes];	/* decimal32: 1, 5, 6, 20 bits*/
} decimal32;

#define DECIMAL_NaN     0x7c		/* 0 11111 00 NaN	      */
#define DECIMAL_sNaN    0x7e		/* 0 11111 10 sNaN	      */
#define DECIMAL_Inf     0x78		/* 0 11110 00 Infinity	      */

#include "decimal32Symbols.h"

/* String conversions 					      */
decimal32 * decimal32FromString(decimal32 *, const char *, decContext *);
char * decimal32ToString(const decimal32 *, char *);
char * decimal32ToEngString(const decimal32 *, char *);

/* decNumber conversions					      */
decimal32 * decimal32FromNumber(decimal32 *, const decNumber *,
				decContext *);
decNumber * decimal32ToNumber(const decimal32 *, decNumber *);

/* Format-dependent utilities 				      */
uint32_t    decimal32IsCanonical(const decimal32 *);
decimal32 * decimal32Canonical(decimal32 *, const decimal32 *);

#endif
