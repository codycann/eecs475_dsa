PATH := /usr/um/gcc-4.7.0/bin:$(PATH)
LD_LIBRARY_PATH := /usr/um/gcc-4.7.0/lib64
LD_RUN_PATH := /usr/um/gcc-4.7.0/lib64

CC = g++
CFLAGS = -std=c++11 -O3 -Wno-deprecated

all:	test speck

speck:
	$(CC) speck.cpp $(CFLAGS) -o speck_cipher
test:
	$(CC) tests.cpp $(CFLAGS) -o speck_tests
	./speck_tests
clean:
	-rm -rf *.o speck_cipher speck_tests
