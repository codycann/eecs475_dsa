CC = g++
LD = g++

CFLAGS = -c -Wall -g -std=c++98
LFLAGS = -pedantic -Wall

OBJS = DSA.o test.o uberzahl.o
RUNTESTS = runtests

default: $(RUNTESTS)


$(RUNTESTS): $(OBJS)
	$(LD) $(LFLAGS) $(OBJS) -o $@

test.o: test.cpp DSA.h uberzahl/uberzahl.h
	$(CC) $(CFLAGS) $<

DSA.o: DSA.cpp DSA.h uberzahl/uberzahl.h
	$(CC) $(CFLAGS) $<

uberzahl.o: uberzahl/uberzahl.cpp uberzahl/uberzahl.h
	$(CC) $(CFLAGS) -w $<

.PHONY: clean
clean:
	rm *.o $(RUNTESTS)
