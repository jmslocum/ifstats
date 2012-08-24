CC=gcc
CFLAGS= --std=gnu99 -Wall -O3
OBJS=ifstats.o

all: staticlib

staticlib: $(OBJS)
	ar -cr libifstats.a $(OBJS)

test: ifstats.c ifstats.h
	$(CC) -c -DIFSTATS_TEST $(CFLAGS) -o ifstats.o ifstats.c
	$(CC) $(CFLAGS) $(OBJS) -o ifstatstest

install: 
	cp libifstats.a /usr/local/lib
	cp ifstats.h /usr/local/include

clean:
	rm *.o
	rm ifstatstest

.PHONY : test clean
