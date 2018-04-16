#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

long strtolx (const char *nPtr, char **endPtr, int base);

int main(int argc, char const *argv[]) {
    const char text[] = "0xfftu jest dalej tekst";
    char * rest;
    strtolx(text, &rest, 0);

    return 0;
}

long strtolx (const char *nPtr, char **endPtr, int base) {
    long number = 0;
    char * charposition;
    int sign;
    const int POSITIVE = 1,
              NEGATIVE = 0;
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
    if ((base == 16) && (*charposition == '0') {
        charposition++;
        if (*charposition == 'x' || *charposition == 'X') {
            charposition++;
            if (*charposition > 'f') {
                charposition--;
                *endptr = charposition;
                return 0;
            }
        }
        else
            charposition--;
    }



    return number;
}
