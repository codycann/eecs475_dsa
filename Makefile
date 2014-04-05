CC = g++
LD = g++

CFLAGS = -c -Wall -std=c++11
LFLAGS = -pedantic -Wall

OBJS = DSA.o test.o uberzahl.o hash.o
RUNTESTS = runtests

default: CFALGS +=  -O3 -NDEBUG
default: $(RUNTESTS)

debug: CFLAGS +=  -g
debug: $(RUNTESTS)


$(RUNTESTS): $(OBJS)
	$(LD) $(LFLAGS) $(OBJS) -o $@ -lgmp -lgmpxx

test.o: test.cpp DSA.h uberzahl.h
	$(CC) $(CFLAGS) $<

DSA.o: DSA.cpp DSA.h uberzahl.h
	$(CC) $(CFLAGS) $<

uberzahl.o: uberzahl.cpp uberzahl.h
	$(CC) $(CFLAGS) $<

hash.o: hash.cpp hash.h uberzahl.h
	$(CC) $(CFLAGS) $<

.PHONY: clean
clean:
	rm *.o $(RUNTESTS)
