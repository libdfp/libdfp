/* Decimal Number Library - Math Functions

   Copyright (C) 2006 IBM Corporation.
   Copyright (C) 2007-2015 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Contributed by IBM Corporation.

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

#define  DECNUMDIGITS 60
#include "decNumber.h"             // base number library
#include "decNumberMath.h"
#include "decNumberLocal.h"
#include "decContext.h"
#include <stdio.h>                 // for printf
#include <stdlib.h>                // for malloc/free
#include <ctype.h>                 // for tolower
#include <assert.h>

#define E  "2.7182818284590452353602874713526624977572470936999595749669676277240766303535"
#define PI "3.1415926535897932384626433832795028841971693993751058209749445923078164062862"

//Flags
#define	DIVIDE		0x80
#define	REMAINDER	0x40
#define	DIVIDEINT	0x20
#define	REMNEAR		0x10
#define	COMPARE		0x01
#define	COMPMAX		0x02
#define	COMPMIN		0x03
#define	COMPNAN		0x04

#define BADINT	(Int)0x80000000

#define DIVIDE_BY_10(x) x.exponent--

// ----------------------------------------------------------------------
// Basic Functions
// ----------------------------------------------------------------------

// Return the largest integer less than or equal to x
static decNumber* decNumberFloor (decNumber *result, decNumber *x, 
			   decContext *set) 
{
  int round = set->round;

  set->round = DEC_ROUND_DOWN;
  decNumberToIntegralValue (result, x, set);
  set->round = round;
  return result;
} /* decNumberFloor  */

static int decNumberIsEqual (decNumber *x, decNumber *y, decContext *set)
{
  decNumber diff;
  decNumberSubtract (&diff, x, y, set);
  return decNumberIsZero (&diff);
} /* decNumberIsEqual  */

static int decNumberIsInteger (decNumber *x, decContext *set)
{
  decNumber y;

  decNumberToIntegralValue (&y, x, set);
  return decNumberIsEqual (x, &y, set);
} /* decNumberIsInteger  */

// Modulo function
static decNumber* decNumberMod (decNumber *result, decNumber *x, decNumber *y,
			 decContext *set)
{
  // x mod y = x - k*y where k = floor(x/y)
  decNumber k;

  decNumberDivide (&k, x, y, set);
  decNumberFloor (&k, &k, set);
  decNumberMultiply (&k, &k, y, set);
  decNumberSubtract (result, x, &k, set);
  return result;
} /* decNumberMod  */

// ----------------------------------------------------------------------
// Exponential Function
// ----------------------------------------------------------------------

decNumber* decNumberPow (decNumber *result, decNumber *x, decNumber *y, 
			 decContext *set)
{
  decNumber tmp;

  if (decNumberIsInteger (y, set)) {
    return decNumberPower (result, x, y, set);
  }
  // y is not an integer.
  if (decNumberIsNegative (x)) {
    // result would be a complex number.
    decNumberFromString (result, "NaN", set);
    return result;
  }
  // x^y = exp (y*ln(x))
  decNumberLn(&tmp, x, set);
  decNumberMultiply (&tmp, &tmp, y, set);
  decNumberExp (result, &tmp, set);
  return result;
} /* decNumberPow  */

/* ------------------------------------------------------------------ */
/* decNumberSquareRoot -- square root operator                        */
/*                                                                    */
/*   This computes C = squareroot(A)                                  */
/*                                                                    */
/*   res is C, the result.  C may be A                                */
/*   rhs is A                                                         */
/*   set is the context; note that rounding mode has no effect        */
/*                                                                    */
/* C must have space for set->digits digits.                          */
/* ------------------------------------------------------------------ */
/* This uses the following varying-precision algorithm in:            */
/*                                                                    */
/*   Properly Rounded Variable Precision Square Root, T. E. Hull and  */
/*   A. Abrham, ACM Transactions on Mathematical Software, Vol 11 #3, */
/*   pp229-237, ACM, September 1985.                                  */
/*                                                                    */
/* % [Reformatted original Numerical Turing source code follows.]     */
/* function sqrt(x : real) : real                                     */
/* % sqrt(x) returns the properly rounded approximation to the square */
/* % root of x, in the precision of the calling environment, or it    */
/* % fails if x < 0.                                                  */
/* % t e hull and a abrham, august, 1984                              */
/* if x <= 0 then                                                     */
/*   if x < 0 then                                                    */
/*     assert false                                                   */
/*   else                                                             */
/*     result 0                                                       */
/*   end if                                                           */
/* end if                                                             */
/* var f := setexp(x, 0)  % fraction part of x   [0.1 <= x < 1]       */
/* var e := getexp(x)     % exponent part of x                        */
/* var approx : real                                                  */
/* if e mod 2 = 0  then                                               */
/*   approx := .259 + .819 * f   % approx to root of f                */
/* else                                                               */
/*   f := f/l0                   % adjustments                        */
/*   e := e + 1                  %   for odd                          */
/*   approx := .0819 + 2.59 * f  %   exponent                         */
/* end if                                                             */
/*                                                                    */
/* var p:= 3                                                          */
/* const maxp := currentprecision + 2                                 */
/* loop                                                               */
/*   p := min(2*p - 2, maxp)     % p = 4,6,10, . . . , maxp           */
/*   precision p                                                      */
/*   approx := .5 * (approx + f/approx)                               */
/*   exit when p = maxp                                               */
/* end loop                                                           */
/*                                                                    */
/* % approx is now within 1 ulp of the properly rounded square root   */
/* % of f; to ensure proper rounding, compare squares of (approx -    */
/* % l/2 ulp) and (approx + l/2 ulp) with f.                          */
/* p := currentprecision                                              */
/* begin                                                              */
/*   precision p + 2                                                  */
/*   const approxsubhalf := approx - setexp(.5, -p)                   */
/*   if mulru(approxsubhalf, approxsubhalf) > f then                  */
/*     approx := approx - setexp(.l, -p + 1)                          */
/*   else                                                             */
/*     const approxaddhalf := approx + setexp(.5, -p)                 */
/*     if mulrd(approxaddhalf, approxaddhalf) < f then                */
/*       approx := approx + setexp(.l, -p + 1)                        */
/*     end if                                                         */
/*   end if                                                           */
/* end                                                                */
/* result setexp(approx, e div 2)  % fix exponent                     */
/* end sqrt                                                           */
/* ------------------------------------------------------------------ */
/*decNumber * decNumberSquareRoot(decNumber *res, decNumber *rhs,
                                decContext *set) {
  decContext workset, approxset;   // work contexts
  decNumber dzero;                 // used for constant zero
  Int  maxp=set->digits+2;         // largest working precision
  Int  residue=0;                  // rounding residue
  uInt status=0, ignore=0;         // status accumulators
  Int  exp;                        // working exponent
  Int  ideal;                      // ideal (preferred) exponent
  Int  needbytes;                  // work
  Int  dropped;                    // ..

  decNumber *allocrhs=NULL;        // non-NULL if rounded rhs allocated
  // buffer for f [needs +1 in case DECBUFFER 0]
  uByte buff[sizeof(decNumber)+(D2U(DECBUFFER+1)-1)*sizeof(Unit)];
  // buffer for a [needs +2 to match maxp]
  uByte bufa[sizeof(decNumber)+(D2U(DECBUFFER+2)-1)*sizeof(Unit)];
  // buffer for temporary, b [must be same size as a]
  uByte bufb[sizeof(decNumber)+(D2U(DECBUFFER+2)-1)*sizeof(Unit)];
  decNumber *allocbuff=NULL;       // -> allocated buff, iff allocated
  decNumber *allocbufa=NULL;       // -> allocated bufa, iff allocated
  decNumber *allocbufb=NULL;       // -> allocated bufb, iff allocated
  decNumber *f=(decNumber *)buff;  // reduced fraction
  decNumber *a=(decNumber *)bufa;  // approximation to result
  decNumber *b=(decNumber *)bufb;  // intermediate result
  // buffer for temporary variable, up to 3 digits
  uByte buft[sizeof(decNumber)+(D2U(3)-1)*sizeof(Unit)];
  decNumber *t=(decNumber *)buft;  // up-to-3-digit constant or work

  #if DECCHECK
  if (decCheckOperands(res, DECUNUSED, rhs, set)) return res;
  #endif

  do {                             // protect allocated storage
    #if DECSUBSET
    if (!set->extended) {
      // reduce operand and set lostDigits status, as needed
      if (rhs->digits>set->digits) {
        allocrhs=decRoundOperand(rhs, set, &status);
        if (allocrhs==NULL) break;
        // [Note: 'f' allocation below could reuse this buffer if
        // used, but as this is rare we keep them separate for clarity.]
        rhs=allocrhs;
        }
      }
    #endif
    // [following code does not require input rounding]

    // handle infinities and NaNs
    if (rhs->bits & DECSPECIAL) {
      if (decNumberIsInfinite(rhs)) {         // an infinity
        if (decNumberIsNegative(rhs)) status|=DEC_Invalid_operation;
         else decNumberCopy(res, rhs);        // +Infinity
        }
       else decNaNs(res, rhs, NULL, &status); // a NaN
      break;
      }

    // calculate the ideal (preferred) exponent [floor(exp/2)]
    // [We would like to write: ideal=rhs->exponent>>1, but this
    // generates a compiler warning.  Generated code is the same.]
    ideal=(rhs->exponent&~1)/2;         // target

    // handle zeros
    if (ISZERO(rhs)) {
      decNumberCopy(res, rhs);          // could be 0 or -0
      res->exponent=ideal;              // use the ideal [safe]
      break;
      }

    // any other -x is an oops
    if (decNumberIsNegative(rhs)) {
      status|=DEC_Invalid_operation;
      break;
      }

    // we need space for three working variables
    //   f -- the same precision as the RHS, reduced to 0.01->0.99...
    //   a -- Hull's approx -- precision, when assigned, is
    //        currentprecision (we allow +2 for use as temporary)
    //   b -- intermediate temporary result
    // if any is too long for local storage, then allocate
    needbytes=sizeof(decNumber)+(D2U(rhs->digits)-1)*sizeof(Unit);
    if (needbytes>sizeof(buff)) {
      allocbuff=(decNumber *)malloc(needbytes);
      if (allocbuff==NULL) {  // hopeless -- abandon
        status|=DEC_Insufficient_storage;
        break;}
      f=allocbuff;            // use the allocated space
      }
    // a and b both need to be able to hold a maxp-length number
    needbytes=sizeof(decNumber)+(D2U(maxp)-1)*sizeof(Unit);
    if (needbytes>sizeof(bufa)) {                // [same applies to b]
      allocbufa=(decNumber *)malloc(needbytes);
      allocbufb=(decNumber *)malloc(needbytes);
      if (allocbufa==NULL || allocbufb==NULL) {  // hopeless
        status|=DEC_Insufficient_storage;
        break;}
      a=allocbufa;            // use the allocated space
      b=allocbufb;            // ..
      }

    // copy rhs -> f, save exponent, and reduce so 0.1 <= f < 1
    decNumberCopy(f, rhs);
    exp=f->exponent+f->digits;               // adjusted to Hull rules
    f->exponent=-(f->digits);                // to range

    // set up working contexts (the second is used for Numerical
    // Turing assignment)
    decContextDefault(&workset, DEC_INIT_DECIMAL64);
    decContextDefault(&approxset, DEC_INIT_DECIMAL64);
    approxset.digits=set->digits;            // approx's length

    // [Until further notice, no error is possible and status bits
    // (Rounded, etc.) should be ignored, not accumulated.]

    // Calculate initial approximation, and allow for odd exponent
    workset.digits=set->digits;              // p for initial calculation
    t->bits=0; t->digits=3;
    a->bits=0; a->digits=3;
    if ((exp & 1)==0) {                      // even exponent
      // Set t=0.259, a=0.819
      t->exponent=-3;
      a->exponent=-3;
      #if DECDPUN>=3
        t->lsu[0]=259;
        a->lsu[0]=819;
      #elif DECDPUN==2
        t->lsu[0]=59; t->lsu[1]=2;
        a->lsu[0]=19; a->lsu[1]=8;
      #else
        t->lsu[0]=9; t->lsu[1]=5; t->lsu[2]=2;
        a->lsu[0]=9; a->lsu[1]=1; a->lsu[2]=8;
      #endif
      }
     else {                                  // odd exponent
      // Set t=0.0819, a=2.59
      f->exponent--;                         // f=f/10
      exp++;                                 // e=e+1
      t->exponent=-4;
      a->exponent=-2;
      #if DECDPUN>=3
        t->lsu[0]=819;
        a->lsu[0]=259;
      #elif DECDPUN==2
        t->lsu[0]=19; t->lsu[1]=8;
        a->lsu[0]=59; a->lsu[1]=2;
      #else
        t->lsu[0]=9; t->lsu[1]=1; t->lsu[2]=8;
        a->lsu[0]=9; a->lsu[1]=5; a->lsu[2]=2;
      #endif
      }
    decMultiplyOp(a, a, f, &workset, &ignore);  // a=a*f
    decAddOp(a, a, t, &workset, 0, &ignore);    // ..+t
    // [a is now the initial approximation for sqrt(f), calculated with
    // currentprecision, which is also a's precision.]

    // the main calculation loop
    decNumberZero(&dzero);                      // make 0
    decNumberZero(t);                           // set t = 0.5
    t->lsu[0]=5;                                  // ..
    t->exponent=-1;                               // ..
    workset.digits=3;                             // initial p
    for (;;) {
      // set p to min(2*p - 2, maxp)  [hence 3; or: 4, 6, 10, ... , maxp]
      workset.digits=workset.digits*2-2;
      if (workset.digits>maxp) workset.digits=maxp;
      // a = 0.5 * (a + f/a)
      // [calculated at p then rounded to currentprecision]
      decDivideOp(b, f, a, &workset, DIVIDE, &ignore); // b=f/a
      decAddOp(b, b, a, &workset, 0, &ignore);  // b=b+a
      decMultiplyOp(a, b, t, &workset, &ignore);// a=b*0.5
      // assign to approx [round to length]
      decAddOp(a, &dzero, a, &approxset, 0, &ignore);
      if (workset.digits==maxp) break;            // just did final
      } // loop

    // a is now at currentprecision and within 1 ulp of the properly
    // rounded square root of f; to ensure proper rounding, compare
    // squares of (a - l/2 ulp) and (a + l/2 ulp) with f.
    // Here workset.digits=maxp and t=0.5
    workset.digits--;                             // maxp-1 is OK now
    t->exponent=-set->digits-1;                   // make 0.5 ulp
    decNumberCopy(b, a);
    decAddOp(b, b, t, &workset, DECNEG, &ignore); // b = a - 0.5 ulp
    workset.round=DEC_ROUND_UP;
    decMultiplyOp(b, b, b, &workset, &ignore);  // b = mulru(b, b)
    decNumberCompare(b, f, b, &workset); // b ? f, reversed
    if (decNumberIsNegative(b)) {               // f < b [i.e., b > f]
      // this is the more common adjustment, though both are rare
      t->exponent++;                              // make 1.0 ulp
      t->lsu[0]=1;                                // ..
      decAddOp(a, a, t, &workset, DECNEG, &ignore); // a = a - 1 ulp
      // assign to approx [round to length]
      decAddOp(a, &dzero, a, &approxset, 0, &ignore);
      }
    else {
      decNumberCopy(b, a);
      decAddOp(b, b, t, &workset, 0, &ignore);  // b = a + 0.5 ulp
      workset.round=DEC_ROUND_DOWN;
      decMultiplyOp(b, b, b, &workset, &ignore);// b = mulrd(b, b)
      decNumberCompare(b, b, f, &workset); // b ? f
      if (decNumberIsNegative(b)) {             // b < f
        t->exponent++;                            // make 1.0 ulp
        t->lsu[0]=1;                              // ..
        decAddOp(a, a, t, &workset, 0, &ignore);// a = a + 1 ulp
        // assign to approx [round to length]
        decAddOp(a, &dzero, a, &approxset, 0, &ignore);
        }
      }
    // [no errors are possible in the above, and rounding/inexact during
    // estimation are irrelevant, so status was not accumulated]

    // Here, 0.1 <= a < 1  [Hull]
    a->exponent+=exp/2;                      // set correct exponent

    // Process Subnormals
    decFinalize(a, set, &residue, &status);

    // count dropable zeros [after any subnormal rounding]
    decNumberCopy(b, a);
    decTrim(b, 1, &dropped);               // [drops trailing zeros]

    // Finally set Inexact and Rounded.  The answer can only be exact if
    // it is short enough so that squaring it could fit in set->digits,
    // so this is the only (relatively rare) time we have to check
    // carefully
    if (b->digits*2-1 > set->digits) {       // cannot fit
      status|=DEC_Inexact|DEC_Rounded;
      }
     else {                                  // could be exact/unrounded
      uInt mstatus=0;                        // local status
      decMultiplyOp(b, b, b, &workset, &mstatus); // try the multiply
      if (mstatus!=0) {                      // result won't fit
        status|=DEC_Inexact|DEC_Rounded;
        }
       else {                                // plausible
        decNumberCompare(t, b, rhs, &workset); // b ? rhs
        if (!ISZERO(t)) {
          status|=DEC_Inexact|DEC_Rounded;
          }
         else {                              // is Exact
          // here, dropped is the count of trailing zeros in 'a'
          // use closest exponent to ideal...
          Int todrop=ideal-a->exponent;      // most we can drop

          if (todrop<0) {                    // ideally would add 0s
            status|=DEC_Rounded;
            }
           else {                            // unrounded
            if (dropped<todrop) todrop=dropped; // clamp to those available
            if (todrop>0) {                  // OK, some to drop
              decShiftToLeast(a->lsu, D2U(a->digits), todrop);
              a->exponent+=todrop;           // maintain numerical value
              a->digits-=todrop;             // new length
              }
            }
          }
        }
      }
    decNumberCopy(res, a);                 // assume this is the result
    } while(0);                              // end protected

  if (allocbuff!=NULL) free(allocbuff);      // drop any storage we used
  if (allocbufa!=NULL) free(allocbufa);      // ..
  if (allocbufb!=NULL) free(allocbufb);      // ..
  if (allocrhs !=NULL) free(allocrhs);       // ..
  if (status!=0) decStatus(res, status, set);// then report status
  return res;
}*/ /* decNumberSquareRoot  */

// ----------------------------------------------------------------------
// Hyperbolic Functions
// ----------------------------------------------------------------------

decNumber* decNumberSinh (decNumber *result, decNumber *x, decContext *set)
{
  // sinh x = (e^x - e^-x)/2
  decNumber ex, emx, mx, two;

  decNumberExp (&ex, x, set);
  decNumberMinus (&mx, x, set);
  decNumberExp (&emx, &mx, set);
  decNumberSubtract (result, &ex, &emx, set);
  decNumberFromString (&two, "2", set);
  decNumberDivide (result, result, &two, set);
  return result;
}

decNumber* decNumberCosh (decNumber *result, decNumber *x, decContext *set)
{
  // cosh x = (e^x + e^-x)/2
  decNumber ex, emx, mx, two;

  decNumberExp (&ex, x, set);
  decNumberMinus (&mx, x, set);
  decNumberExp (&emx, &mx, set);
  decNumberAdd (result, &ex, &emx, set);
  decNumberFromString (&two, "2", set);
  decNumberDivide (result, result, &two, set);
  return result;
} /* decNumberCosh  */

decNumber* decNumberTanh (decNumber *result, decNumber *x, decContext *set)
{
  // tanh x = sinh x / cosh x = (e^x - e^-x) / (e^x + e^-x)
  decNumber ex, emx, mx, denominator;
  decNumberExp (&ex, x, set);
  decNumberMinus (&mx, x, set);
  decNumberExp (&emx, &mx, set);
  decNumberSubtract (result, &ex, &emx, set);
  decNumberAdd (&denominator, &ex, &emx, set);
  decNumberDivide (result, result, &denominator, set);
  return result;
} /* decNumberTanh  */

// ----------------------------------------------------------------------
// Trigonometric Functions
// ----------------------------------------------------------------------

decNumber* decNumberSin (decNumber *result, decNumber *y, decContext *set)
{
  decNumber pi, pi2, zero, one, two, x, cnt, term, cmp;
  int i;
  int negate = 0;

  decNumberFromString (&zero,"0", set);
  decNumberFromString (&one, "1", set);
  decNumberFromString (&two, "2", set);
  decNumberFromString (&pi,  PI , set);

  // Copy the argument y, so we can modify it.
  decNumberCopy (&x, y);
  // sin -x = - sin x
  /* if (decCompare (&x, &zero) < 0) { */
  if (decNumberIsNegative (&x)) { // x < 0
    decNumberMinus (&x, &x, set);
    negate = 1;
  }
  // We now have x >= 0
  decNumberMultiply (&pi2, &pi, &two, set); // pi2 = 2*pi
  decNumberMod (&x, &x, &pi2, set);
  // We now have 0 <= x < 2*pi
  /*if (decCompare (&x, &pi) >= 0) {*/
  decNumberCompare (&cmp, &x, &pi, set);
  if (!decNumberIsNegative (&cmp)) {
    // x >= pi
    decNumberSubtract (&x, &x, &pi, set);
    negate = 1-negate;
  }
  // We now have 0 <= x < pi
  decNumberDivide (&pi2, &pi, &two, set); // pi2 = pi/2
  /*if (decCompare (&x, &pi2) >= 0) {*/
  decNumberCompare (&cmp, &x, &pi2, set);
  if (!decNumberIsNegative (&cmp)) {
    // x >= pi/2, so let x = pi-x 
    decNumberSubtract (&x, &pi, &x, set);
  }
  // We now have 0 <= x <= pi/2.

  //             x^3   x^5    x^7
  // sin x = x - --- + --- - ---- + ...
  //              6    120   5040
  //
  // term(0) = x
  // term(i) = - term(i-1) * x^2 / ((2*i)*(2*i+1))

  decNumberCopy (&cnt, &two);
  decNumberCopy (&term, &x);
  decNumberCopy (result, &x);
  // DECNUMDIGITS+3 terms are enough to achieve the required precision.
  for (i=0; i<DECNUMDIGITS+3; i++) {
    // term = -term * x^2 / (cnt*(cnt+1))
    // cnt = cnt+2
    decNumberMinus (&term, &term, set);
    decNumberMultiply (&term, &term, &x, set);
    decNumberMultiply (&term, &term, &x, set);
    decNumberDivide   (&term, &term, &cnt, set);
    decNumberAdd (&cnt, &cnt, &one, set); 
    decNumberDivide   (&term, &term, &cnt, set);
    decNumberAdd (&cnt, &cnt, &one, set);
    // sum = sum + term
    decNumberAdd (result, result, &term, set);
  }
  if (negate) {
    decNumberMinus (result, result, set);
  }
  return result;
} /* decNumberSin  */

decNumber* decNumberCos (decNumber *result, decNumber *y, decContext *set)
{
  decNumber pi, pi2, zero, one, two, x, cnt, term, cmp;
  int i;
  int negate = 0;

  decNumberFromString (&zero,"0", set);
  decNumberFromString (&one, "1", set);
  decNumberFromString (&two, "2", set);
  decNumberFromString (&pi,  PI , set);

  // Copy the argument y, so we can modify it.
  decNumberCopy (&x, y);
  // cos -y = cos y
  /*if (decCompare (&x, &zero) < 0) {*/
  if (decNumberIsNegative (&x)) {
    decNumberMinus (&x, &x, set);
  }
  // We now have x >= 0
  decNumberMultiply (&pi2, &pi, &two, set); // pi2 = 2*pi
  decNumberMod (&x, &x, &pi2, set);
  // We now have 0 <= x < 2*pi
  /*if (decCompare (&x, &pi) >= 0) {*/
  decNumberCompare (&cmp, &x, &pi, set);
  if (!decNumberIsNegative (&cmp)) {
    // x >= pi
    decNumberSubtract (&x, &pi2, &x, set);
  }
  // We now have 0 <= x < pi
  decNumberDivide (&pi2, &pi, &two, set); // pi2 = pi/2
  /*if (decCompare (&x, &pi2) >= 0) {*/
  decNumberCompare (&cmp, &x, &pi2, set);
  if (!decNumberIsNegative (&cmp)) {
    // x >= pi/2, so let x = pi-x 
    decNumberSubtract (&x, &pi, &x, set);
    negate = 1;
  }
  // We now have 0 <= x <= pi/2.

  //             x^2   x^4   x^6
  // cos x = 1 - --- + --- - --- + ...
  //              2     24   720
  //
  // term(0) = 1
  // term(i) = - term(i-1) * x^2 / ((2*i-1)*(2*i))
  decNumberCopy (&cnt, &one);
  decNumberCopy (&term, &one);
  decNumberCopy (result, &one);
  // DECNUMDIGITS+3 terms are enough to achieve the required precision.
  for (i=0; i<DECNUMDIGITS+3; i++) {
    // term = -term * x^2 / (cnt*(cnt+1))
    // cnt = cnt+2
    decNumberMinus (&term, &term, set);
    decNumberMultiply (&term, &term, &x, set);
    decNumberMultiply (&term, &term, &x, set);
    decNumberDivide   (&term, &term, &cnt, set);
    decNumberAdd (&cnt, &cnt, &one, set); 
    decNumberDivide   (&term, &term, &cnt, set);
    decNumberAdd (&cnt, &cnt, &one, set);
    // sum = sum + term
    decNumberAdd (result, result, &term, set);
  }
  if (negate) {
    decNumberMinus (result, result, set);
  }
  return result;
} /* decNumberCos  */

decNumber* decNumberTan (decNumber *result, decNumber *y, decContext *set)
{
  // tan x = sin x / cos x
  decNumber denominator;

  decNumberSin (result, y, set);
  decNumberCos (&denominator, y, set);
  if (decNumberIsZero (&denominator))
    decNumberFromString (result, "NaN", set);
  else
    decNumberDivide (result, result, &denominator, set);
  return result;
} /* decNumberTan  */

decNumber* decNumberAtan (decNumber *result, decNumber *x, decContext *set) 
{
  //                 x^3   x^5   x^7
  // arctan(x) = x - --- + --- - --- + ...
  //                  3     5     7
  //
  // This power series works well, if x is close to zero (|x|<0.5).
  // If x is larger, the series converges too slowly,
  // so in order to get a smaller x, we apply the identity 
  //
  //                      sqrt(1+x^2) - 1
  // arctan(x) = 2*arctan ---------------
  //                             x
  //
  // twice. The first application gives us a new x with x < 1.
  // The second application gives us a new x with x < 0.4142136.
  // For that x, we use the power series and multiply the result by four.
  
  decNumber f, g, mx2, one, two, term;
  int i;

  decNumberFromString (&one, "1", set);
  decNumberFromString (&two, "2", set);

  if (decNumberIsZero (x)) {
    decNumberCopy (result, x);
    return result;
  }

  for (i=0; i<2; i++) {
    decNumber y;
    decNumberMultiply (&y, x, x, set);     // y = x^2
    decNumberAdd (&y, &y, &one, set);      // y = y+1
    decNumberSquareRoot (&y, &y, set);     // y = sqrt(y)
    decNumberSubtract (&y, &y, &one, set); // y = y-1
    decNumberDivide (x, &y, x, set);       // x = y/x
  }
  // f(0) = x
  // f(i) = f(i-1) * (-x^2)
  //
  // g(0) = 1
  // g(i) = g(i-1) + 2
  //
  // term(i) = f(i) / g(i)
  decNumberCopy (&f, x);     // f(0) = x
  decNumberCopy (&g, &one);  // g(0) = 1
  decNumberCopy (&term, x);  // term = x
  decNumberCopy (result, x); // sum  = x 
  decNumberMultiply (&mx2, x, x, set); // mx2 = x^2
  decNumberMinus (&mx2, &mx2, set);    // mx2 = -x^2  
  // Since x is less than sqrt(2)-1 = 0.4142...,
  // each term is smaller than the previous term by a factor of about 6,
  // so two iterations are more than enough to increase the precision 
  // by one digit.
  // 2*DECNUMDIGITS terms are enough to achieve the required precision.
  for (i=0; i<2*DECNUMDIGITS; i++) {
    // f = f * (-x^2)
    decNumberMultiply (&f, &f, &mx2, set);
    // g = g+2
    decNumberAdd (&g, &g, &two, set); 
    // term = f/g
    decNumberDivide (&term, &f, &g, set);
    // sum = sum + term
    decNumberAdd (result, result, &term, set);
  }
  // Multiply result by four.
  decNumberAdd (result, result, result, set);
  decNumberAdd (result, result, result, set);
  return result;
} /* decNumberAtan  */
