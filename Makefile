# Change installdir to where you want the program to be installed
installdir = $(HOME)/bin

# Project directory structure
builddir=build
bin=$(builddir)/marginstrip
src=src/main.c

# C libraries used
LIBS=-lc

# Compiler flags: Common, Debug and Release
CFLAGS+=-std=c99
DFLAGS=-O0 -g -Wall -Wextra
RFLAGS=-O2

# Default target
$(bin): $(src) $(builddir)
	$(CC) $(CFLAGS) $(RFLAGS) $(LIBS) -o $(bin) $(src)

debug: $(src) $(builddir)
	$(CC) $(CFLAGS) $(DFLAGS) $(LIBS) -o $(bin) $(src)
	
$(builddir):
	mkdir $(builddir)

release:
	mkdir -p marginstrip/src
	cp -r test README.md Makefile runtests marginstrip
	cp $(src) marginstrip/src
	tar -cvf marginstrip.tar marginstrip
	gzip marginstrip.tar
	rm -rf marginstrip

install: $(bin)
	install -C $(bin) $(installdir)

test: $(bin)
	./runtests

clean:
	rm -rf $(builddir) marginstrip.tar

.PHONY: build, debug, clean, test, install, release
