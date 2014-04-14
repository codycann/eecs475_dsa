FLAGS = -Wall -Wvla -Wextra -pedantic -std=c++11 -lssl
OFILES = speck.o

all: speck.o specktest speckutil
	
release: FLAGS += -O3
release: all

speck.o: speck.h speck.cc
	g++ speck.cc $(FLAGS) -c

specktest: speck.o specktest.cc
	g++ specktest.cc $(FLAGS) $(OFILES) -o specktest.out 

speckutil: speck.o speckutil.cc
	g++ speckutil.cc $(FLAGS) $(OFILES) -o speckutil.out
	
clean:
	rm *.o *.out -f
