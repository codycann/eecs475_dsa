CC = g++
LD = g++

CFLAGS = -c
CRYPTO_FLAGS = -lssl -lcrypto

PROG = gcm
OBJS = uberzahl.o increment.o multiplication.o ghash.o gctr.o encrypt.o genkey.o

default: $(PROG)

$(PROG): $(OBJS)
	$(LD) $(OBJS) $(CRYPTO_FLAGS) -o $(PROG)

increment.o: increment.cpp uberzahl.h
	$(CC) $(CFLAGS) increment.cpp

multiplication.o: multiplication.cpp uberzahl.h
	$(CC) $(CFLAGS) multiplication.cpp

ghash.o: ghash.cpp uberzahl.h
	$(CC) $(CFLAGS) ghash.cpp

gctr.o: gctr.cpp uberzahl.h
	$(CC) $(CFLAGS) gctr.cpp

encrypt.o: encrypt.cpp uberzahl.h
	$(CC) $(CFLAGS) encrypt.cpp

genkey.o: genkey.cpp uberzahl.h
	$(CC) $(CFLAGS) genkey.cpp

uberzahl.o: uberzahl.cpp uberzahl.h
	$(CC) $(CFLAGS) uberzahl.cpp

clean:
	rm -f encrypt

real_clean:
	rm -f *.o
	rm -f encrypt