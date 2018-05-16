#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>

#define NUL '\0'

#define POSITIVE 1
#define NEGATIVE -1

long strtol (const char *nPtr, char **endPtr, int base) {
    //checking if the base value is correct
    if ((base < 2 || base > 36) && base != 0) {
        errno = EINVAL;
        return 0;
    }

    long number = 0;
    const char * divider;
    int currentdigit,
        sign,
        maximum_digittoadd;
    unsigned long maximum_number;
    bool conversion_possible = true;

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
    if (sign == POSITIVE)
        maximum_number = (unsigned long) LONG_MAX * sign / (unsigned long) base;
    else
        maximum_number = (unsigned long) LONG_MIN * sign / (unsigned long) base;
    maximum_digittoadd = maximum_number % (unsigned long) base;

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
    	if (!conversion_possible ||
            (unsigned long) number * sign > maximum_number ||
            ((unsigned long) number * sign == maximum_number && (int) currentdigit > (unsigned int) maximum_digittoadd)) {
    		  conversion_possible = false;
    		  divider++;
    	} else { //the actual conversion to decimal
    		conversion_possible = true;
    		number = (number * base) + ((sign == POSITIVE) ? 1 : -1) * currentdigit;
            divider++;
    	}
    }
    if (!conversion_possible) {
    	if (sign == POSITIVE)
    		number = LONG_MAX;
    	else
    		number = LONG_MIN;
    	errno = ERANGE;
    }
    //if (sign == NEGATIVE)
    //	number = -number;
    if (endPtr != NUL) {
        if (isspace(* divider)) //checking if the number is separated
            divider++;          //from the rest of the string
    	* endPtr = (char *) divider;
    }
    return number;
}
