all: main

main: main.c
	gcc -o app main.c calculator.c utils.c history.c

test: test.c
# 	gcc -lrt -lm -o test test.c calculator.c utils.c history.c
	gcc test.c calculator.c utils.c history.c -lm -o test

memtest: main.c
	gcc -fsanitize=address -g -o app main.c calculator.c utils.c history.c

clean:
	rm -f app test