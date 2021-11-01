CC = gcc
CFLAGS = -W -Wall -IIncludes

BINARY = word-search-solver
SOURCES = $(wildcard Sources/*.c)

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(BINARY)

clean:
	rm -rf $(BINARY)
