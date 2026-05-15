YACC = yacc
CC = gcc
CFLAGS = -Wall -O2

all: expanse calculator

expanse: expanse.c
	$(CC) $(CFLAGS) -o $@ $^

expanse.c: expanse.y
	$(YACC) -o $@ $^

calculator: calculator.c
	$(CC) $(CFLAGS) -o $@ $^

calculator.c: calculator.y
	$(YACC) -o $@ $^

clean:
	rm -f expanse.c expanse calculator.c calculator

.PHONY: all clean
