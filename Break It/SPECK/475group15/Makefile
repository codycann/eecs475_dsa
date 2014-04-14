PATH := /usr/um/gcc-4.7.0/bin:$(PATH)
LD_LIBRARY_PATH := /usr/um/gcc-4.7.0/lib64
LD_RUN_PATH := /usr/um/gcc-4.7.0/lib64

DEBUG = -g

FLAGS = -Wall -Wvla -Wextra -pedantic -std=c++11

all: speck_openssl

debug: FLAGS += -g
debug: all



speck_openssl: speck_openssl.cpp speck_openssl.h
	g++ $(FLAGS) speck_openssl.cpp -o speck_openssl.out /usr/lib64/libcrypto.so

# perfTest: perfTest.cpp 
# 	g++ $(FLAGS) perfTest.cpp -o perfTest.out


clean:
	rm -f *.o *.out core.*
