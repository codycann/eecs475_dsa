/**
--------------------------- USAGE ---------------------------
to declare a 4-byte number named alpha:
fixedInt<4> alpha;

the sizeof operator works correctly for fixedInts, returning
its actual size. This is because a fixedInt's memory is
on the stack and the class has no overhead whatsoever.

A fixedInt's size must be known at compile time and cannot
be changed during program execution.

Operators +, -, ^ always return a fixedInt of the same size
as the larger input.

A fixedInt can be implicitly cast to or from any built-in
integral data type, or to any other fixedInt size.

**/


#ifndef FIXED_INT_H
#define FIXED_INT_H
#include <iostream>
#include <string>
using namespace std;
template <unsigned int width>
class fixedInt
{
public:
	unsigned char data[width];
	fixedInt(){}
	// constructs fixedInt from a hexadecimal string
	fixedInt(const string &input) {
		int b = 0, n = 0, i = 0;
		for (; i < width * 2; ++i) {
			char c = i < input.size() ? input[input.size() - i - 1] : '0';
			if (c >= 'a' && c < 'g') c = c - 'a' + 10; 
			else if (c >= 'A' && c < 'G') c = c - 'A' + 10;
			else c = c - '0';

			if (b == 1) {
				data[i / 2] = n + c * 16;
				n = 0;
			} else n += c;
			b = 1 - b;
		}
		i /= 2;
		for (; i < width; i++) {
			data[i] = 0;
		}
	}
	fixedInt(const char *input) { fixedInt(string(input)); }
	template<typename Type>
	fixedInt(const Type& input)
	{
		*this=input;
	}

	template <unsigned int otherWidth>
	fixedInt<(otherWidth>width?otherWidth:width)> operator|(const fixedInt<otherWidth> rhs)
	{
		fixedInt<(otherWidth>width?otherWidth:width)> ret;
		int i=0;
		for(;i<min(width,otherWidth);++i)
		{
			ret.data[i]=data[i] | rhs.data[i];
		}
		if(width>otherWidth)
			for(;i<width;++i)
			{
				ret.data[i]=data[i];
			}
		else if(width<otherWidth)
			for(;i<otherWidth;++i)
			{
				ret.data[i]=rhs.data[i];
			}
		return ret;
	}

	template <unsigned int otherWidth>
	fixedInt<(otherWidth>width?otherWidth:width)> operator^(const fixedInt<otherWidth> rhs)
	{
		fixedInt<(otherWidth>width?otherWidth:width)> ret;
		int i=0;
		for(;i<min(width,otherWidth);++i)
		{
			ret.data[i]=data[i]^rhs.data[i];
		}
		if(width>otherWidth)
			for(;i<width;++i)
			{
				ret.data[i]=data[i];
			}
		else if(width<otherWidth)
			for(;i<otherWidth;++i)
			{
				ret.data[i]=rhs.data[i];
			}
		return ret;
	}



	template<unsigned int otherWidth>
	fixedInt<(otherWidth>width?otherWidth:width)> operator+(const fixedInt<otherWidth> rhs)
	{
		unsigned short buffer=0;
		char& carry=*(  ((char*)(&buffer))  +1);
		fixedInt<(otherWidth>width?otherWidth:width)> ret;
		int i=0;
		for(;i<min(width,otherWidth);++i)
		{
			buffer=carry;
			buffer+=data[i];
			buffer+=rhs.data[i];
			ret.data[i]=(char)buffer;
		}
		if(width>otherWidth)
		{
			for(;i<width;++i)
			{
				buffer=carry;
				buffer+=data[i];
				ret.data[i]=(char)buffer;
			}
		} else if(width<otherWidth)
		{
			for(;i<otherWidth;++i)
			{
				buffer=carry;
				buffer+=rhs.data[i];
				ret.data[i]=(char)buffer;
			}
		}
		return ret;

	}

	template<unsigned int otherWidth>
	fixedInt<(otherWidth>width?otherWidth:width)> operator-(fixedInt<otherWidth> rhs)
	{
		//extend the smaller number
		fixedInt<(otherWidth>width?otherWidth:width)> extendedNumber;
		fixedInt<(otherWidth>width?otherWidth:width)>* biggerNumber;
		if(width<otherWidth)
		{
			extendedNumber=*this;
			biggerNumber=(fixedInt<(otherWidth>width?otherWidth:width)>*)&rhs;//this cast is neccessary only for the code to compile. If it ever actually gets here, rhs will always be an appropriately-typed pointer.
		} else
		{
			extendedNumber=rhs;
			biggerNumber=(fixedInt<(otherWidth>width?otherWidth:width)>*)this;
		}

		//two's compliment the right-hand number
		fixedInt<1> one;
		one=1;
		if(width<otherWidth)//*this was extended
		{
			*biggerNumber=~*biggerNumber;
			*biggerNumber=*biggerNumber+one;
		}
		else //rhs was extended
		{
			extendedNumber=~extendedNumber;
			extendedNumber=extendedNumber+one;
		}
		//now, just add them
		return extendedNumber+*biggerNumber;
	}

	template<typename Type>
	fixedInt& operator=(Type input)
	{
		char* inputPtr=(char*)&input;
		int i=0;
		for(;i<min((unsigned int)sizeof(Type),width);++i)
		{
			data[i]=inputPtr[i];
		}
		if(width>sizeof(Type))
		{
			for(;i<width;++i)
			{
				data[i]=0;
			}
		}
		return *this;
	}
	template<typename Type>
	operator Type()
	{
		char ret[sizeof(Type)];
		int i=0;
		for(;i<min((unsigned int)sizeof(Type),width);++i)
		{
			ret[i]=data[i];
		}
		for(;i<max((unsigned int)sizeof(Type),width);++i)
		{
			ret[i]=0;
		}
		return *(Type*)ret;
	}
	fixedInt operator~()
	{
		fixedInt ret;
		for(int i=0;i<width;++i)
		{
			ret.data[i]=~data[i];
		}
		return ret;
	}
	template<typename Type>
	fixedInt operator<<(Type bitshift)
	{
		fixedInt ret;
		Type byteshift=bitshift/8;
		int i=width-1;
		//first, do the easy part. Shift everything over by the appropriate number of whole bytes
		for(;i>=byteshift;--i)
		{
			ret.data[i]=data[i-byteshift];
		}
		for(;i>=0;--i)
		{
			ret.data[i]=0;
		}
		//now, do the hard part. The individual bitwise shifts.
		bitshift=bitshift%8;	
		unsigned short buffer=0;
		char& carry=*(  ((char*)(&buffer))  +1);
		for(i=0;i<width;++i)
		{
			buffer=buffer+(ret.data[i]<<bitshift);
			ret.data[i]=buffer;
			buffer=carry;
		}
		return ret;
	}
	template<typename Type>
	fixedInt operator>>(Type bitshift)
	{
		fixedInt ret;
		Type byteshift=bitshift/8;
		int i=0;
		//first, do the easy part. Shift everything over by the appropriate number of whole bytes
		for(;i<width - byteshift;++i)
		{
			ret.data[i]=data[i+byteshift];
		}
		for(;i<width;++i)
		{
			ret.data[i]=0;
		}
		//now, do the hard part. The individual bitwise shifts.
		bitshift=bitshift%8;
		unsigned short buffer=0;
		char& carry=*(  ((char*)(&buffer))  +1);
		for(i=width-1;i>=0;--i)
		{
			char temp=buffer;
			buffer=0;
			carry=ret.data[i];
			buffer=buffer>>bitshift;
			ret.data[i]=temp+(ret.data[i]>>bitshift);
		}
		return ret;
	}
	template<unsigned int w>
	friend ostream& operator<<(ostream& os, const fixedInt<w>& input);
};
template<unsigned int w>
ostream& operator<<(ostream& os, const fixedInt<w>& input)
{
	bool wasntHex=false;
	if(!(os.flags()&ios::hex))
	{
		wasntHex=true;
		os<<hex;//temporarily enter hex-printing mode
	}
	for(int i=w-1;i>=0;--i)
	{
		if (input.data[i] < 16) os << 0;
		os<<(unsigned short)input.data[i];
	}
	if(wasntHex)
	{
		os<<dec;
	}
	return os;

}
#endif
