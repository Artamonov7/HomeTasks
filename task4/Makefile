CC = gcc
CFLAGS = -Wall
SOURCES = ls.c pwd.c cp.c cat.c cmp.c echo.c
PROGS = $(SOURCES:.c=)

all: $(PROGS)

$(PROGS): %: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(PROGS)

