CC = gcc
CFLAGS = -Wall -g
TARGET = memreader memwriter

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

memreader: memreader.o storage.o
	$(CC) $(CFLAGS) -o $@ $^

memwriter: memwriter.o storage.o
	$(CC) $(CFLAGS) -o $@ $^

tar:
	tar -cf task.tar *.c *.h Makefile

clean:
	rm -f *.o $(TARGET)
