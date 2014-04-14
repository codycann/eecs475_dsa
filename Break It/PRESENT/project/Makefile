# Environment variables to get newer g++ compiler version
PATH := /usr/um/gcc-4.7.0/bin:$(PATH)
LD_LIBRARY_PATH := /usr/um/gcc-4.7.0/lib64
LD_RUN_PATH := /usr/um/gcc-4.7.0/lib64

BINFILE = present

CFLAGS = -std=c++11

# List of .cpp files
CPPFILES = present.cpp uberzahl.cpp

# List of .h files
HFILES = 

$(BINFILE):	clean
	g++ $(CFLAGS) $(CPPFILES) -o $(BINFILE)

clean:
	rm -rf $(BINFILE)

run:
	./$(BINFILE)
