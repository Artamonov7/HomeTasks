CC = gcc
CFLAGS = -Wall
SOURCES = main.c token_list.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = shell

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

%*.o: %*.c
	$(CC) $(CFLAGS) -Wextra -c $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)

