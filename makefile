all: test_std_strtol test_my_strtol tests

.c.o:
	gcc -c -Wall -pedantic -g $<

test_std_strtol: test_strtol.o
	gcc -g test_strtol.o -o test_std_strtol

test_my_strtol: test_strtol.o strtol.o
	gcc -g test_strtol.o strtol.o -o test_my_strtol


.PHONY: tests clean

tests: test_std_strtol test_my_strtol
	./test_my_strtol > rep.my
	./test_std_strtol > rep.std
	cmp rep.my rep.std

clean:
	-rm test_strtol.o strtol.o test_std_strtol test_my_strtol rep.my rep.std

wip:
	gcc strtolwip.c -o strtolwip -lm
