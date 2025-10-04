all: main

main: calculator.c
	gcc -o app calculator.c bcal.c

unit_test: unit_test.c
	gcc -std=c11 -o unit_test unit_test.c bcal.c munit/munit.c

int_test: int_test.c
	gcc -std=c11 -o int_test int_test.c bcal.c

clean:
	rm -f app unit_test int_test