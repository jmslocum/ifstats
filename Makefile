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
	install -g 0 -o 0 -m 644 man/ifstats.3 /usr/share/man/man3/
	gzip -f /usr/share/man/man3/ifstats.3
	ln -f -s /usr/share/man/man3/ifstats.3.gz /usr/share/man/man3/getIfStats.3.gz
	ln -f -s /usr/share/man/man3/ifstats.3.gz /usr/share/man/man3/releaseStats.3.gz

clean:
	-rm *.o
	-rm *.a
	-rm ifstatstest

.PHONY : test clean
