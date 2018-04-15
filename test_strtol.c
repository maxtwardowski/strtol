#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>

/* print number using specified base, 2 < base < 36 */
void
print_num (long number, int base)
{
  long power_of_base;

  if (number >= 0)
    {
      power_of_base = 1;
      /* Find the greatest power of 'base' which isn't more 
         than the number */
      while (power_of_base <= number / base)
	power_of_base = power_of_base * base;
      /* Now print out the digits */
      while (power_of_base > 0)
	{
	  int dig = number / power_of_base;
	  if (dig <= 9)
	    dig += '0';
	  else
	    dig += 'A' - 10;
	  putchar (dig);
	  number = number % power_of_base;
	  power_of_base = power_of_base / base;
	}
    }
  else				/* number <0 */
    {
      putchar('-');
      power_of_base = -1;
      /* cannot just negate the number - need a symmetrical case */
      while (power_of_base >= number / base)
	power_of_base = power_of_base * base;

      while (power_of_base < 0)
	{
	  int dig = number / power_of_base;
	  if (dig <= 9)
	    dig += '0';
	  else
	    dig += 'A' - 10;
	  putchar (dig);
	  /* % for negative numbers is not well defined in C */
	  number = number - (number/power_of_base) * power_of_base;
	  power_of_base = power_of_base / base;
	}
    }
}

void
test_strtol (char *s, int base, int printbase)
{
  char * cp = strdup(s);
  char *endPtr = cp;
  errno = 0;

  printf ("%s in base %d is ", s, base);
  print_num (strtol (cp, &endPtr, base), printbase);
  putchar ('\n');

  printf("strtol: %s\n",strerror(errno));
  if(endPtr==cp)
    printf("String is not a number\n");
  else
    printf("End pointer points to \"%s\"\n",endPtr);
  free(cp);
}


long long_min = LONG_MIN;
long long_max = LONG_MAX;

int
main ()
{
  strtol("1234",NULL,10);

  test_strtol ("", 10 ,10);
  test_strtol ("0", 16 ,16);

  test_strtol ("1234", 10 ,10);
  test_strtol ("1234", 10 ,10);
  test_strtol ("-1234", 10, 10);
  test_strtol ("F", 16, 16);
  test_strtol ("7FFFFFFE", 16, 16);
  test_strtol ("7FFFFFFF", 16, 16);
  test_strtol ("80000000", 16, 16);
  test_strtol ("-80000001", 16, 16);
  test_strtol ("-80000000", 16, 16);
  test_strtol ("-7FFFFFFF", 16, 16);
  test_strtol ("-7FFFFFFE", 16, 16);
  test_strtol ("ZIK0ZJ", 36, 36);
  test_strtol ("ZIK0ZK", 36, 36);
  test_strtol ("-ZIK0ZJ", 36, 36);
  test_strtol ("-ZIK0ZK", 36, 36);
  test_strtol ("-ZIK0ZL", 36, 36);

  test_strtol (" \t\n -0x234", 16, 16);
  test_strtol (" \t\n +128", 0, 10);
  test_strtol (" \t\n + 128", 0, 10);

  test_strtol ("-0x7FFFFFFE", 0, 16);
  test_strtol ("-0x7FFFFFFEEEEEEEEEEE???", 0, 16);
  test_strtol ("-0xZZZZZZZZZZZZZZZZ???", 36, 36);
  test_strtol ("-#$#$", 36, 36);
  
  test_strtol ("-#$#$", 40, 10);  
  test_strtol ("10", 40, 10);
  test_strtol ("10", 1, 10);
  test_strtol ("0", 1, 10);
  test_strtol ("0", -1, 10);

  test_strtol ("-071", 0, 8);

  test_strtol ("-0xr", 0, 16);
  test_strtol ("-0xr", 16, 16);
  test_strtol ("-0r", 0, 8);
  test_strtol ("-08", 0, 16);

  test_strtol ("21474836481", 0, 10);

  return 0;
}
