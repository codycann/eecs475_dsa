OBJS =  speck.o \
		test.o \
		uberzahl.o \
		main.o

OPT = -O3
CXX = g++
CXXFLAGS = $(OPT) -std=c++11

speck: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o speck

.phony: clean
clean:
	rm -f *.o

.phony: run
run: speck
	time ./speck
