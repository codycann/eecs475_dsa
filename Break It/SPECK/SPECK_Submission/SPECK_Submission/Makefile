PRODUCTS = proj
CFLAGS = -Wall -O3 -pthread -Wl,--no-as-needed -std=c++11
OBJ = SPECK128_256.o

all: $(OBJ)
	g++ $(CFLAGS) -o $(PRODUCTS) $(OBJ)

# Rules for various parts of the target
%.o: %.c
	g++ $(CFLAGS) -c -o $@ $*.c

%.o: %.cc
	g++ $(CFLAGS) -c -o $@ $*.cc
	
%.o: %.cpp
	g++ $(CFLAGS) -c -o $@ $*.cpp

# Delete all .o files and executable
clean:
	rm -rf *.o $(PRODUCTS)