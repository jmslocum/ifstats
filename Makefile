CC=gcc
CFLAGS= --std=gnu99 -ggdb
OBJS=ifstats.o

all : $(OBJS)

test : ifstats.c ifstats.h
	$(CC) -c -DIFSTATS_TEST $(CFLAGS) -o ifstats.o ifstats.c
	$(CC) $(CFLAGS) $(OBJS) -o ifstatstest

clean :
	rm *.o
	rm ifstatstest
