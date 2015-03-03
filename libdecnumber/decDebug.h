/* Debug directives for libdecnumber.

   Copyright (C) 2014-2015 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

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

#if DECTRACE || DECCHECK

/* ------------------------------------------------------------------ */
/* decNumberShow -- display a number [debug aid]		      */
/*   dn is the number to show					      */
/*								      */
/* Shows: sign, exponent, coefficient (msu first), digits	      */
/*    or: sign, special-value					      */
/* ------------------------------------------------------------------ */
/* this is public so other modules can use it */
static inline void
decNumberShow (const decNumber * dn)
{
  const Unit *up;		/* work */
  uInt u, d;			/* .. */
  Int cut;			/* .. */
  char isign = '+';		/* main sign */
  if (dn == NULL)
    {
      printf ("NULL\n");
      return;
    }
  if (decNumberIsNegative (dn))
    isign = '-';
  printf (" >> %c ", isign);
  if (dn->bits & DECSPECIAL)
    {				/* Is a special value */
      if (decNumberIsInfinite (dn))
	printf ("Infinity");
      else
	{			/* a NaN */
	  if (dn->bits & DECSNAN)
	    printf ("sNaN");	/* signalling NaN */
	  else
	    printf ("NaN");
	}
      /* if coefficient and exponent are 0, no more to do */
      if (dn->exponent == 0 && dn->digits == 1 && *dn->lsu == 0)
	{
	  printf ("\n");
	  return;
	}
      /* drop through to report other information */
      printf (" ");
    }

  /* now carefully display the coefficient */
  up = dn->lsu + D2U (dn->digits) - 1;	/* msu */
  printf ("%ld", (LI) * up);
  for (up = up - 1; up >= dn->lsu; up--)
    {
      u = *up;
      printf (":");
      for (cut = DECDPUN - 1; cut >= 0; cut--)
	{
	  d = u / DECPOWERS[cut];
	  u -= d * DECPOWERS[cut];
	  printf ("%ld", (LI) d);
	}			/* cut */
    }				/* up */
  if (dn->exponent != 0)
    {
      char esign = '+';
      if (dn->exponent < 0)
	esign = '-';
      printf (" E%c%ld", esign, (LI) abs (dn->exponent));
    }
  printf (" [%ld]\n", (LI) dn->digits);
}				/* decNumberShow */
#else
# define decimal64Show(__x)
# define decNumberShow(__x)
#endif
