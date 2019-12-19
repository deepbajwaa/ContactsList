CC=gcc
CFLAGS=-Wall -ansi -I$(IDIR) -g

IDIR = ./includes/
SRCDIR = ./src/
BINDIR = ./bin/

SOURCES = $(SRCDIR)*.c

all: $(BINDIR)contactList

$(BINDIR)contactList: $(BINDIR)main.o $(BINDIR)contactList.o
	$(CC) $(BINDIR)main.o $(BINDIR)contactList.o -o $@

$(BINDIR)contactList.o: $(SRCDIR)contactList.c
	$(CC) $(CFLAGS) -c $(SRCDIR)contactList.c -o $@

$(BINDIR)main.o: $(SRCDIR)main.c
	$(CC) $(CFLAGS) -c $(SRCDIR)main.c -o $@
	
clean:
	rm ./bin/*.o
