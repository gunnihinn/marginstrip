# Change installdir to where you want the program to be installed
installdir = $(HOME)/bin

DATE=$(shell date +'%Y-%m-%d')
HASH=$(shell git rev-parse --short HEAD)

bin=marginstrip
src=src/main.c

# C libraries used
LIBS=-lc

# Compiler flags: Common, Debug and Release
CFLAGS+=-std=c99 -DDATE=\"$(DATE)\" -DHASH=\"$(HASH)\"
DFLAGS=-O0 -g -Wall -Wextra
RFLAGS=-O2

# Default target
$(bin): $(src)
	$(CC) $(CFLAGS) $(RFLAGS) $(LIBS) -o $(bin) $(src)

debug: $(src)
	$(CC) $(CFLAGS) $(DFLAGS) $(LIBS) -o $(bin) $(src)
	
install: $(bin)
	install -C $(bin) $(installdir)

test: $(bin)
	./runtests

clean:
	rm -f $(bin)

.PHONY: debug, clean, test, install
