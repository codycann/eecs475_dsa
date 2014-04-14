
all: SignAndVerify.cpp
	g++ SignAndVerify.cpp uberzahl.cpp SHA1.cpp main.cpp keygen.cpp -lgmpxx -lgmp  -o proj
	
blah: all
	./proj
	
	
time: all
	time ./proj
