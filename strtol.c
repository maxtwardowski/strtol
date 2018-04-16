#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>

long strtolx (const char *nPtr, char **endPtr, int base) {
    long number = 0;
    char * charposition;
    int digit, sign, cutlim, any = 0;
    const int POSITIVE = 1,
              NEGATIVE = 0;
    unsigned long cutoff;

    assert(nPtr);
    charposition = nPtr;

    //looking for a space if the beggining of the string is moved further
    while (isspace(* charposition))
        charposition++;

    if (* charposition == '+') {
        sign = POSITIVE;
        charposition++;
    } else if (* charposition == '-') {
        sign = NEGATIVE;
        charposition++;
    } else
        sign = POSITIVE;

    if ((base == 8) && (*charposition == '0'))
        charposition++;
    if ((base == 16) && (*charposition == '0')) {
        charposition++;
        if (*charposition == 'x' || *charposition == 'X') {
            charposition++;
            if (*charposition > 'f') {
                charposition--;
                *endPtr = charposition;
                return 0;
            }
        }
        else
            charposition--;
    }

    if((* charposition == '0') && (base == 0)) {
        base = 8;
        charposition++;
    	if((* charposition == 'x') || (* charposition == 'X')){
    		 base = 16;
    		 charposition++;
    		 if(* charposition > 'f') {
                 charposition--;
                 * endPtr = charposition;
                 return 0;
             }
    	}
    	if(*charposition > '7') {
            *endPtr = charposition;
            return 0;
        }
    }
    if(base == 0)
        base = 10;
    if(base < 2 || base > 36){
    	errno = EINVAL;
    	return 0;
    }

    if (sign)
    	cutoff = - (unsigned long) LONG_MIN;
    else
    	cutoff = LONG_MAX;
    cutlim = cutoff % (unsigned long) base;
    cutoff /= (unsigned long) base;
    while (* charposition != '\0') {
    	if (isdigit(* charposition))
    		digit = * charposition - '0';
    	else {
    		if(isalpha(* charposition)) {
    			if(islower(* charposition))
    				digit = (* charposition - 'a') + 10;
    			else
                    digit = (* charposition - 'A') + 10;
    		} else
    			break;
    	}
    	if (any < 0 || number > cutoff || (number == cutoff && (int) digit > cutlim)) {
    		  any = -1;
    		  charposition++;
    	}
    	else {
    		any = 1;
    		number = (number * base) + digit;
    		charposition++;
    	}
    }
    if (any < 0) {
    	if (sign)
    		number = LONG_MIN;
    	else
    		number = LONG_MAX;
    	errno = ERANGE;
    }
    if (sign == NEGATIVE)
    	number *= -1;
    if (endPtr != '\0') {
    	if (any)
    		* endPtr = charposition;
    	else
    		* endPtr = (char *) nPtr;
    }
    return number;
}
