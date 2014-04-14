#Enables C++11
PATH := /usr/um/gcc-4.7.0/bin:$(PATH)
LD_LIBRARY_PATH := /usr/um/gcc-4.7.0/lib64
LD_RUN_PATH := /usr/um/gcc-4.7.0/lib64

all:
	g++ --std=c++11 cli.cpp /usr/lib64/libgmp.so /usr/lib64/libgmpxx.so -o simon
	g++ --std=c++11 test.cpp /usr/lib64/libgmp.so /usr/lib64/libgmpxx.so -o test

nickt:
	g++ --std=c++11 test.cpp /usr/local/lib/libgmpxx.a /usr/local/lib/libgmp.a -o test
	g++ --std=c++11 cli.cpp /usr/local/lib/libgmpxx.a /usr/local/lib/libgmp.a -o simon

chart:
	g++ --std=c++11 cli.cpp /usr/lib64/libgmp.so /usr/lib64/libgmpxx.so -o simon
	g++ --std=c++11 chart.cpp /usr/lib64/libgmp.so /usr/lib64/libgmpxx.so -o chart
