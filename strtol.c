#include <string.h>
#include <ctype.h>
#include <errno.h> //for returning error codes to compare with test_strtol
#include <limits.h> //for LONG_MAX & LONG_MIN
#include <stdbool.h>

#define NUL '\0'

long strtol (const char *nPtr, char **endPtr, int base) {
    //checking if the base value is correct
    if((base < 2 || base > 36) && base != 0) {
        errno = EINVAL;
        return 0;
    }

    long number = 0;
    const char * divider;
    int currentdigit,
        sign,
        cutlim;
    enum sign {NEGATIVE, POSITIVE};
    unsigned long cutoff;
    bool correctconversion = true;

    divider = nPtr;

    //looking for a space if the beggining of the string is moved further
    while (isspace(* divider))
        divider++;

    //detecting the sign, positive by default
    if (* divider == '+') {
        sign = POSITIVE;
        divider++;
    } else if (* divider == '-') {
        sign = NEGATIVE;
        divider++;
    } else
        sign = POSITIVE;

    if (* divider == NUL) {
        * endPtr = (char *) divider;
        return 0;
    }

    if (* divider < '0' || (* divider > '9' && * divider < 'A') || (* divider > 'z'))
        return 0;

    if ((base == 8) && (* divider == '0')) {
        divider++;
        if (* divider == 'o' || * divider == 'O') //if the input includes 'o', it's skipped
            divider++;
    }
    else if ((base == 16)) {
        if (* divider == '0') {
            divider++;
            if (* divider == 'x' || * divider == 'X') {
                divider++;
                if (* divider > 'f' || * divider > 'F') {
                    divider--;
                    *endPtr = (char *) divider;
                    return 0;
                }
            }
            else
                divider--;
        }
    //basically the system-detecting algorithm
    } else if (base == 0) {
        if (* divider == '0') {
            divider++;
            if (* divider == 'o' || * divider == 'O') {
                base = 8;
                divider++;
                if (* divider > '7') {
                    divider--;
                    * endPtr = (char *) divider;
                    return 0;
                }
            } else if (* divider == 'x' || * divider == 'X') {
                base = 16;
                divider++;
                if (* divider > 'f' || * divider > 'F') {
                    divider--;
                    * endPtr = (char *) divider;
                    return 0;
                }
            } else if (* divider <= '7') {
                base = 8;
            } else {
                * endPtr = (char *) divider;
                return 0;
            }
        } else if (* divider >= '1' && * divider <= '9') {
                base = 10;
        }
    }

    //two conditions just for clarity --> |LONG_MIN| = LONG_MAX + 1
    if (sign)
        cutoff = LONG_MAX / (unsigned long) base;
    else
        cutoff = (unsigned long) LONG_MIN / (unsigned long) base;
    cutlim = cutoff % (unsigned long) base;

    //looping until the end of the input string
    //searching for convertable characters
    while (* divider != NUL) {
    	if (isdigit(* divider))
    		currentdigit = * divider - '0'; //converting to the actual integer
    	else {
    		if (isalpha(* divider)) {
    			if (islower(* divider) && (* divider - 'a') + 10 < base)
    				currentdigit = (* divider - 'a') + 10;
    			else if (!islower(* divider) && (* divider - 'A') + 10 < base)
                    currentdigit = (* divider - 'A') + 10;
                else
                    break;
    		} else
    			break;
    	}
    	if (!correctconversion ||
            number > cutoff ||
            (number == cutoff && (int) currentdigit > cutlim)) {
    		  correctconversion = false;
    		  divider++;
    	} else { //the actual conversion to decimal
    		correctconversion = true;
    		number = (number * base) + currentdigit;
    		divider++;
    	}
    }
    if (!correctconversion) {
    	if (sign)
    		number = LONG_MAX;
    	else
    		number = LONG_MIN;
    	errno = ERANGE;
    }
    if (sign == NEGATIVE)
    	number *= -1;
    if (endPtr != NUL) {
        if (isspace(* divider)) //checking if the number is separated
            divider++;          //from the rest of the string
    	* endPtr = (char *) divider;
    }
    return number;
}
