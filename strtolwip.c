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
    char * string;
    int sign;
    const int POSITIVE = 1,
              NEGATIVE = 0;
    assert(nPtr);
    string = nPtr;

    //looking for a space if the beggining of the string is moved further
    while (isspace(* string))
        string++;

    if (* string == '+') {
        sign = POSITIVE;
        string++;
    } else if (* string == '-') {
        sign = NEGATIVE;
        string++;
    } else
        sign = POSITIVE;

    //printf("%d\n", sign);
    //printf("%c\n", *string);



    return number;
}
