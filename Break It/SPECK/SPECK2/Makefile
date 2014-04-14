PATH := /usr/um/gcc-4.7.0/bin:$(PATH)
LD_LIBRARY_PATH := /usr/um/gcc-4.7.0/lib64
LD_RUN_PATH := /usr/um/gcc-4.7.0/lib64

#Default Flags
FLAGS = -Wall -Wextra -pedantic -Wvla -std=c++11

OBJS = speck2.o uberzahl.o

# make release - will compile "all" with $(FLAGS) and the O3 flag
release: FLAGS += -O3
release: all

# make debug - will compile "all" with $(FLAGS) and the g flag
debug: FLAGS += -g
debug: all

# make profile - will compile "all" with $(FLAGS) and the g and p flags
profile: FLAGS += -pg
profile: all

all: $(OBJS)
	g++ $(FLAGS) $(OBJS) -o run

speck2.o: speck2.cpp speck2.h uberzahl.h
	g++ $(FLAGS) -c speck2.cpp
	
uberzahl.o: uberzahl.cpp uberzahl.h
	g++ $(FLAGS) -c uberzahl.cpp

# Make sure to compile object files with the -c flag
# ***EXAMPLE***
#
# all: example.o
#	 g++ $(FLAGS) example.o -o example
# example.o: example.cpp
#	 g++ $(FLAGS) -c example.cpp
#
# ***END EXAMPLE****

# make clean - remove .o files.  You can modify it to remove whatever
clean:
	rm -f *.o run