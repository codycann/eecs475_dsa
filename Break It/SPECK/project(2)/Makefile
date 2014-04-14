#CAEN supports C++11, but it must be enabled. The following enables C++11:
PATH := /usr/um/gcc-4.7.0/bin:$(PATH)
LD_LIBRARY_PATH := /usr/um/gcc-4.7.0/lib64
LD_RUN_PATH := /usr/um/gcc-4.7.0/lib64

#Default Flags
FLAGS = -O3 -std=c++11

all:
	g++ $(FLAGS) main.cpp -o speck
performance:
	g++ $(FLAGS) performance.cpp -o speckPerformance
test:
	g++ $(FLAGS) test_vectors.cpp -o speckTest
example:
	g++ $(FLAGS) example.cpp -o speckExample

	
# make clean
clean:
	rm -f *.o
	rm -f speck
	rm -f speckPerformance
	rm -f speckTest
	rm -f speckExample
