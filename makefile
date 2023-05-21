CC = gcc
FLAGS = -O3 -Wall -Wextra -g3
CCF = $(CC) $(FLAGS)

bin:
	mkdir -p bin

src/rln.c: bin
	$(CCF) -c -obin/rln.o src/rln.c -lcstr

bin/rln.o: src/rln.c
	ar rcs bin/librln.a bin/rln.o

install: bin/rln.o
	cp src/rln.h /usr/local/include
	cp bin/librln.a /usr/local/lib

examples/test.c: src/rln.c
	$(CCF) -obin/test examples/test.c bin/rln.o -lcstr

examples/test2.c: src/rln.c
	$(CCF) -obin/test2 examples/test2.c bin/rln.o -lcstr

examples/: examples/test.c examples/test2.c

