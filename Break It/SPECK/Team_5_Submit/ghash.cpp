#include <iostream>
#include "uberzahl.h"
#include "multiplication.h"
#include "gctr.h"

using namespace std;

uberzahl ghash(uberzahl x, uberzahl H, int bit_length_diff)
{
	int chunk_size = 128;
	int m = (x.bitLength() + bit_length_diff) / 128;
	unsigned int low_index = x.bitLength() - chunk_size + bit_length_diff;
	unsigned int high_index = x.bitLength() - 1;
	bool bit_length_diff_add = true;
	uberzahl Y;
	for (int i = 1; i < m; ++i)
	{
		Y = multiply(Y ^ betterExtract(x, low_index, high_index), H);
		low_index -= chunk_size;
		high_index -= chunk_size;
		if (bit_length_diff)
		{
			high_index += bit_length_diff;
			bit_length_diff_add = false;
		}
	}

	low_index = 0;
	high_index = 127;
	uberzahl x_or = (Y ^ betterExtract(x, low_index, 127));
	Y = multiply(x_or, H);

	return Y;
}