CC = g++
CFLAGS = -I src/include
LDFLAGS = -L src/lib
LDLIBS = -lmingw32 -lSDL3

all: main

main: main.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)