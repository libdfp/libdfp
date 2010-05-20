/*
set -ex

gcc -m64 -O2 -g -D__STDC_WANT_DEC_FP__=1 -std=gnu99 test_amort.c -ldfp -ldecnumber -lm -o test_amort64

gcc -m64 -O2 -g -D__STDC_WANT_DEC_FP__=1 -std=gnu99 test_amort.c -ldfp -ldecnumber -lm -o test_amort32

gcc -m64 -O2 -mcpu=power6 -g -D__STDC_WANT_DEC_FP__=1 -std=gnu99 test_amort.c -ldfp -ldecnumber -lm -o test_amort64P6

echo "generate input data"
rm "amort.input"
rm "test_amort.data"
./test_amort64 10000.0 0.075 5.0
./test_amort64 15000.0 0.050 5.0
./test_amort64 20000.0 0.065 5.0
./test_amort64 25000.0 0.080 5.0
./test_amort64 30000.0 0.090 5.0

./test_amort64 80000.0 0.055 20.0
./test_amort64 120000.0 0.055 20.0
./test_amort64 150000.0 0.055 20.0
./test_amort64 185000.0 0.055 20.0
./test_amort64 199500.0 0.055 20.0

./test_amort64 180000.0 0.055 30.0
./test_amort64 199500.0 0.055 30.0
./test_amort64 210000.0 0.055 30.0
./test_amort64 350000.0 0.055 30.0
./test_amort64 550000.0 0.055 30.0

echo "verify amortization tables"

./test_amort64 -v > test_amort.data

diff test_amort.verify.data test_amort.data

echo "$?"

echo "performance testing"

./test_amort64 10000

exit
*/



/* File test_amort.c created by sjmunroe on Mon Oct 30 2006. */
/* -std=gnu99 */
/* #define __STDC_WANT_DEC_FP__ 1 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <float.h>
#include <math.h>
#include <sys/time.h>
#include <dfp/dfp.h>

//extern _Decimal64 powd64 (_Decimal64, _Decimal64);
//extern _Decimal64 quantized64 (_Decimal64, _Decimal64);
//extern _Decimal64 strtod64 (char *, char *);

typedef struct
{
  _Decimal64
    month,
    payment,
    interest_payed,
    principal_payed, principal, accum_principal, accum_interest;
} tabletype;

tabletype *table;

typedef struct
{
  _Decimal64 loan, rate, duration;
} inputtype;

static _Decimal64
armort_monthly_payment (_Decimal64 loan, _Decimal64 yearly_rate,
			_Decimal64 years)
{
  _Decimal64 payment = -1.0DD;
  _Decimal64 months = years * 12.0DD;
  _Decimal64 monthly_rate = yearly_rate / 12.0DD;
  _Decimal64 temp0, temp1, temp2, temp3;

  temp0 = 1.0DD / (1.0DD + monthly_rate);
#ifdef __DEBUG_PRINT__
  printf ("1/(1+r) = %Df\n", temp0);
#endif
  temp1 = 1.0DD - powd64 (temp0, months);
#ifdef __DEBUG_PRINT__
  printf ("1 - (1/(1+r))**n = %Df\n", temp1);
#endif
  temp2 = temp1 / monthly_rate;
#ifdef __DEBUG_PRINT__
  printf ("(1 - (1/(1+r))**n)/r = %Df\n", temp2);
#endif
  temp3 = loan / temp2;
  payment = quantized64 (temp3, 1.00DD);
#ifdef __DEBUG_PRINT__
  printf ("quantized64(%Df, 1.00DD) = %Df\n", temp3, payment);
#endif
  return payment;
}



static void
armort_monthly_table (_Decimal64 loan, _Decimal64 yearly_rate,
		      _Decimal64 years, int format_enable,
		      int print_enable)
{
  _Decimal64 months = years * 12.0DD;
  _Decimal64 monthly_rate = yearly_rate / 12.0DD;
  //_Decimal64 temp0, temp1, temp2, temp3;
  _Decimal64 temp0;
  _Decimal64 principal, principal_payed, interest_payed;
  _Decimal64 accum_principal, accum_interest;
  _Decimal64 payment;
  char buf[80];

  principal = loan;
  accum_principal = 0.0DD;
  accum_interest = 0.0DD;
  
  payment = armort_monthly_payment (loan,
				    yearly_rate, years);
  
  if (format_enable)
  {
	  snprintf(buf, 80, "Loan %Df at %Df for %Df years payments %Df",
		   loan, yearly_rate, years, payment);
	  if (print_enable)
	  {
		  printf ("\n%s\n", buf);
	  }
  }

  for (temp0 = 0.0DD; temp0 < (months); temp0 += 1.0DD)
    {
      interest_payed = quantized64 ((principal * monthly_rate), 1.00DD);
      principal_payed = payment - interest_payed;
      principal -= principal_payed;
      accum_principal += principal_payed;
      accum_interest += interest_payed;

      if (temp0 >= (months - 1.0DD) && principal != 0.00DD)
	{
	  payment += principal;
	  accum_principal += principal;
	  principal = 0.00DD;
	}

	if (format_enable)
      {
        snprintf(buf, 80, "%Df: %Df-%Df=%Df, total(%Df,%Df) balance=%Df",
         temp0, payment, interest_payed, principal_payed,
         accum_interest, accum_principal, principal);
	
	if (print_enable)
		printf ("%s\n", buf);
      }
      else 
      {
	tabletype *t;
	t = table + (int) temp0;
	t->month = temp0;
	t->payment = payment;
	t->interest_payed = interest_payed;
	t->principal_payed = principal_payed;
	t->accum_interest = accum_interest;
	t->accum_principal = accum_principal;
	t->principal = principal;
      }
    }

  if ( principal > 0.0DD)
  {
	interest_payed = quantized64 ((principal * monthly_rate), 1.00DD);
	principal_payed = payment - interest_payed;
	principal -= principal_payed;
	accum_principal += principal_payed;
	accum_interest += interest_payed;
	
	if (principal != 0.00DD)
	{
	payment += principal;
	accum_principal += principal;
	principal = 0.00DD;
	}
	
		if (format_enable)
		{
			snprintf(buf, 80, "%Df: %Df-%Df=%Df, total(%Df,%Df) balance=%Df\n",
				temp0, payment, interest_payed, principal_payed,
				accum_interest, accum_principal, principal);
			if (print_enable)
			{
				printf ("%s\n", buf);
			}
		}
  }
}

static void
armort_verifier (inputtype *inputs, int numinputs)
{
//	_Decimal64 payment;
	int i;
	
	for (i = 0; i < numinputs; i++)
	{
		inputtype *input = &inputs[i];

		armort_monthly_table (input->loan,
				      input->rate, 
				      input->duration, 
				      1,
				      1);
	}
}

typedef long long msec_t;

static msec_t
		getTimeOfDayMsec (void)
{
	msec_t result, msecs;
	struct timeval time;
	int rc;

	rc = gettimeofday (&time, NULL);
	if (rc == 0)
	{
		msecs = (msec_t) ((time.tv_usec + 500) / 1000);
		result = (msec_t) (time.tv_sec);
		result = (result * 1000) + msecs;
	}
	else
	{
		result = 0;
	}

	return result;
}

int
main (int argc, char *argv[])
{
  int i, iterations;
  msec_t startt, endt, deltat;
  double mill_sec;
  inputtype *inputs;
  int numinputs;
  FILE *inputfile;
  char inputfilename[] = "amort.input";
  char *argv_option;

  table = (tabletype *) malloc (30 * 12 * sizeof (tabletype));
  //printf ("table@%p for %d bytes\n", table, (30 * 12 * sizeof (tabletype)));
  iterations = 50000;

  if (argc < 2)
	 {
		 printf ("This application requires parameters.\n");
		 return 0;
	 }

  argv_option = argv[1];
  if (*argv_option == '-')
	{
		argv_option++;
		if (*argv_option == 'v')
			;
		else {
			printf ("only v is valid %s is not\n", argv[1]);
			return 2;
		}
	} else {
		argv_option = " ";
		if (argc >= 4)
		{
			inputtype input;
			input.loan = strtod64 (argv[1], NULL);
			input.rate = strtod64 (argv[2], NULL);
			input.duration = strtod64 (argv[3], NULL);
			if ( input.duration > 30.0DD)
				{
				printf ("Maximum duration is 30 years, exiting\n");
				return 1;
				}
			
			inputfile = fopen (inputfilename, "a");
			printf ("loan=%Df rate=%Df duration=%Df added to %s\n",
				input.loan, input.rate, input.duration, inputfilename);
			fwrite (&input, sizeof (inputtype), 1, inputfile);
			fclose (inputfile);
			return 0;
		}
		else
		{
		iterations = atoi (argv[1]);
		}
	}

  inputfile = fopen (inputfilename, "r");
  fseek (inputfile, 0, SEEK_END);
  numinputs = ftell (inputfile) / sizeof (inputtype);
  rewind (inputfile);
  inputs = malloc (numinputs * sizeof (inputtype));
  fread (inputs, sizeof (inputtype), numinputs, inputfile);
  fclose (inputfile);

  if (*argv_option == 'v')
  {
	  armort_verifier(inputs, numinputs);
	  return 0;
  }
  
  startt = getTimeOfDayMsec ();

  for (i = 0; i < iterations; i++)
    {
      inputtype *input = &inputs[i % numinputs];

      /*printf ("armort_monthly_payment(%Df,%Df,%Df) = %Df\n",
         169900.0DD, 0.0725DD, 30.0DD, 0, 0); */
      armort_monthly_table (input->loan,
			    input->rate, 
			    input->duration, 
			    0, 0);
    }

  endt = getTimeOfDayMsec ();
  deltat = endt - startt;
  mill_sec = (double) deltat / 1000.0;

  printf (" %d iterations of amort w/o formating requires %12.3f seconds\n", iterations,
	  mill_sec);
  
  startt = getTimeOfDayMsec ();

  for (i = 0; i < iterations; i++)
  {
	  inputtype *input = &inputs[i % numinputs];

      /*printf ("armort_monthly_payment(%Df,%Df,%Df) = %Df\n",
	  169900.0DD, 0.0725DD, 30.0DD, 0, 0); */
	  armort_monthly_table (input->loan,
				input->rate, 
				input->duration, 
				1, 0);
  }

  endt = getTimeOfDayMsec ();
  deltat = endt - startt;
  mill_sec = (double) deltat / 1000.0;

  printf (" %d iterations of amort with formating requires %12.3f seconds\n", iterations,
	  mill_sec);

  return 0;
}
