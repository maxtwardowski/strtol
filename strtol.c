#include <assert.h> //po co ta biblioteka
#include <ctype.h> //jw
#include <errno.h>  //jw
#include <limits.h> //jw
#include <string.h> //jw

long strtol(char* nptr, char **endptr /*podwójny wskaŸnik?*/, int base){
char *string;
long number = 0;
int sign, digit;
unsigned long cutoff;
int cutlim, any = 0;
/********************* quits if string is empty *******************/
string = nptr;
/**************************** looks for the start of the string, if there are white spaces beforehand *******************************************/
while(isspace(*string))
{
string++;
}
/********************* checks the sign of the number *******************/
if(*string=='-'){
	sign = 1;
	string++;
}
else if (*string=='+'){
		sign = 0;
		string++;
} else
    sign = 0;
/************************ Checks if base is 0 and quits if number is 0x+(>f)*******************************/
if((base==8)&&(*string=='0')) string++;
if((base==16)&&(*string=='0')){
	 string++;
	 if((*string=='x')||(*string=='X')){
		 string++;
		 if(*string > 'f') {string--; *endptr = string; return 0;}
	  }
	 else string--;
}
if((*string=='0')&&(base==0)){
base = 8;
string++;
	if((*string=='x')||(*string=='X')){
		 base = 16;
		 string++;
		 if(*string > 'f') {string--; *endptr = string; return 0;}
	}
	if(*string > '7') {*endptr = string; return 0;}
}
if(base==0) base = 10;
if(base <2 || base >36){
	errno=ERANGE;// o co chodzi
	return 0;
}
cutoff = sign ? -(unsigned long) LONG_MIN : LONG_MAX; // jesli sign=1 to cutoff=-LONG_MIN
cutlim = cutoff % (unsigned long) base; //cutlim to reszta z dzielenia cutoff przez baze
cutoff /= (unsigned long) base; //cutoff=cutoff/baze
while(*string!='\0'){ //kiedy string wskazuje na wartosc ktora nie jest koncem pliku
	if (isdigit(*string)){ //jesli string wskazuje na liczbe, funkcja isdigit zwraca 0 jesli jej argument nie jest liczba, a jesli jest to zwraca ta liczbe
		digit = *string - '0'; //digit=ta liczba
	}
	else {
		if(isalpha(*string)){// isalpha zwraca 0 jesli argument nie jest litera, jesli jest to zwraca ta litere
			if(islower(*string)){ //sprawdza czy arg jest ma³¹ liter¹
				digit = (*string - 'a') + 10; // zamienia litery uzywane w systemie 16 na liczby np B=11 itp
			}
			else digit = (*string - 'A') + 10;
		}
		else{
			break;
		}
	}
	if (any < 0 || number > cutoff || (number == cutoff && (int) digit > cutlim)){//co robi ten warunek?
		  any =-1;
		  string++;
	}
	else {//?
		any = 1;
		number = (number*base)+digit;
		string++;
	}
}
if (any<0){
	number = sign ? LONG_MIN : LONG_MAX;
	errno = ERANGE;
}
if (sign==1){
	number*=-1;
}
if (endptr != '\0') *endptr = any ? string : (char *) nptr;
return number;
}
