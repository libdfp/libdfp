/* Test facility scaffolding.

   Copyright (C) 2009 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Ryan S. Arnold <rsa@us.ibm.com>

   The Decimal Floating Point C Library is free software; you can
   redistribute it and/or modify it under the terms of the GNU Lesser
   General Public License version 2.1.

   The Decimal Floating Point C Library is distributed in the hope that
   it will be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
   the GNU Lesser General Public License version 2.1 for more details.

   You should have received a copy of the GNU Lesser General Public
   License version 2.1 along with the Decimal Floating Point C Library;
   if not, write to the Free Software Foundation, Inc., 59 Temple Place,
   Suite 330, Boston, MA 02111-1307 USA.

   Please see libdfp/COPYING.txt for more information.  */


#include <stdio.h>

#ifndef _C
#include <string.h>
#include <stdarg.h>
#include <limits.h> /* For CHAR_MAX.  */

static int fail = 0;

/* Generally don't invoke this directly unless you already have __LINE__ set in
 * a variable and want _C_P to output it.  The name stands for Compare with
 * Position.  */
#define _C_P(f,l,x,y) do { \
  if(strcmp(x,y)) { \
    fprintf(stderr, "Error: Expected: \"%s\" - Result: \"%s\" in: %s:%d.\n", x,y,f,l); \
    ++fail; \
  } else { \
    fprintf(stdout, "Success: Expected: \"%s\" - Result: \"%s\" in: %s:%d.\n", x,y,f,l); \
  } \
} while (0)

#define _C(x,y) _C_P (__FILE__,__LINE__,x,y)

#endif /* _C  */

#ifndef _PC

static char buf[CHAR_MAX];

/* _PC == Printf_dfp Compare with Position  */
#define _PC_P(f,l,x,y,args...) do { \
  memset(buf,'\0',CHAR_MAX); \
  /* Invokes printf dfp.  */  \
  sprintf(buf, y, ##args); \
  _C_P(f,l,x,buf); \
} while (0)

/* _PC == Printf_dfp Compare
 *
 * Variadic macro used to compare a printf invocation with an expected result.
 *
 * X: Expected String
 * Y: Format String
 * ...: Argument list
 *
 * Use it like you'd use printf, except you include an 'expected result' string
 * to precede everything.
 *
 * e.g.
 *   _PC("0.000033333","%DDf.\n", (_Decimal128) 0.00033333DL);
 *
 */
#define _PC(x,y,...) _PC_P (__FILE__,__LINE__,x,y,__VA_ARGS__)
#endif /* _PC  */

#ifndef _REPORT
/* Don't print anything if there are no failures.  */
#define _REPORT() do { \
    if(fail) { \
      fprintf(stderr, "Found %d failures.\n", fail); \
    } \
} while (0)

#endif /* _REPORT  */


