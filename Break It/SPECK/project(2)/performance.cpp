#include "speck.h"
#include "fixedInt.h"
#include <vector>
#include <ctime>

int main()
{
	//the number of keys to generate for each word size of each type
	int numberKeysToGenerate=800;
	int numberBlocksToGenerate=800;

	///generate keys

	//first dimension is the number of key words
	//second dimension if the number of rounds
	//third dimension is a number of repeatedly generated random keys
	//fourth dimension is the key itself
	vector<vector<vector<vector<fixedInt<2>>>>> keys16; vector<int> keySizes16; vector<int> numberRounds16;
	vector<vector<vector<vector<fixedInt<3>>>>> keys24; vector<int> keySizes24; vector<int> numberRounds24;
	vector<vector<vector<vector<fixedInt<4>>>>> keys32; vector<int> keySizes32; vector<int> numberRounds32;
	vector<vector<vector<vector<fixedInt<6>>>>> keys48; vector<int> keySizes48; vector<int> numberRounds48;
	vector<vector<vector<vector<fixedInt<8>>>>> keys64; vector<int> keySizes64; vector<int> numberRounds64;

	//4 words for 16-bit
	keys16.resize(1); keySizes16={4}; numberRounds16={22};
	//3 or 4 for 24 bit, 32 bit
	//2 or 3 for 48 bit
	keys24.resize(2); keySizes24={3,4}; numberRounds24={22,23};
	keys32.resize(2); keySizes32={3,4}; numberRounds32={26,27};
	keys48.resize(2); keySizes48={2,3}; numberRounds48={28,29};
	//2 or 3 or 4 for 64 bit
	keys64.resize(3); keySizes64={2,3,4}; numberRounds64={32,33,34};

	///generate and expand keys
	//16 bit
	for(int i=0;i<keySizes16.size();++i)
	{
		keys16[i].resize(numberRounds16.size());
		for(int j=0;j<numberRounds16.size();++j)
		{
			keys16[i][j].resize(numberKeysToGenerate);
			for(int k=0;k<numberKeysToGenerate;++k)
			{
				auto x=generate_key<fixedInt<2>>(keySizes16[i]);
				keys16[i][j][k].push_back(expandKey<fixedInt<2>,fixedInt<4>>(numberRounds16[j],x));
			}
		}
	}

	//24 bit
	for(int i=0;i<keySizes24.size();++i)
	{
		keys24[i].resize(numberRounds24.size());
		for(int j=0;j<numberRounds24.size();++j)
		{
			keys24[i][j].resize(numberKeysToGenerate);
			for(int k=0;k<numberKeysToGenerate;++k)
			{
				auto x=generate_key<fixedInt<3>>(keySizes24[i]);
				keys24[i][j][k].push_back(expandKey<fixedInt<3>,fixedInt<6>>(numberRounds24[j],x));
			}
		}
	}

	//32 bit
	for(int i=0;i<keySizes32.size();++i)
	{
		keys32[i].resize(numberRounds32.size());
		for(int j=0;j<numberRounds32.size();++j)
		{
			keys32[i][j].resize(numberKeysToGenerate);
			for(int k=0;k<numberKeysToGenerate;++k)
			{
				auto x=generate_key<fixedInt<4>>(keySizes32[i]);
				keys32[i][j][k].push_back(expandKey<fixedInt<4>,fixedInt<8>>(numberRounds32[j],x));
			}
		}
	}

	//48 bit
	for(int i=0;i<keySizes48.size();++i)
	{
		keys48[i].resize(numberRounds48.size());
		for(int j=0;j<numberRounds48.size();++j)
		{
			keys48[i][j].resize(numberKeysToGenerate);
			for(int k=0;k<numberKeysToGenerate;++k)
			{
				auto x=generate_key<fixedInt<6>>(keySizes48[i]);
				keys48[i][j][k].push_back(expandKey<fixedInt<6>,fixedInt<12>>(numberRounds48[j],x));
			}
		}
	}

	//64 bit
	for(int i=0;i<keySizes64.size();++i)
	{
		keys64[i].resize(numberRounds64.size());
		for(int j=0;j<numberRounds64.size();++j)
		{
			keys64[i][j].resize(numberKeysToGenerate);
			for(int k=0;k<numberKeysToGenerate;++k)
			{
				auto x=generate_key<fixedInt<8>>(keySizes64[i]);
				keys64[i][j][k].push_back(expandKey<fixedInt<8>,fixedInt<16>>(numberRounds64[j],x));
			}
		}
	}

	///generate blocks
	vector<fixedInt<4>> blocks16;
	vector<fixedInt<6>> blocks24;
	vector<fixedInt<8>> blocks32;
	vector<fixedInt<12>> blocks48;
	vector<fixedInt<16>> blocks64;

	//we just use generate_key to generate blocks, because it's really just a
	//vector-based random number generator for fixedInts
	blocks16=generate_key<fixedInt<4>>(numberBlocksToGenerate);
	blocks24=generate_key<fixedInt<6>>(numberBlocksToGenerate);
	blocks32=generate_key<fixedInt<8>>(numberBlocksToGenerate);
	blocks48=generate_key<fixedInt<12>>(numberBlocksToGenerate);
	blocks64=generate_key<fixedInt<16>>(numberBlocksToGenerate);

	///test performance
	clock_t startTime;
	clock_t stopTime;
	cout<<"ENCRYPTION\n=========\n";
	cout<<"16 bit word size"<<'\n';
	for(int i=0;i<keySizes16.size();++i)
	{
		cout<<"\t"<<keySizes16[i]*16<<"-bit key"<<endl;
		for(int j=0;j<numberRounds16.size();++j)
		{
			cout<<"\t\t"<<numberRounds16[j]<<" rounds: ";
			startTime=clock();
			for(int k=0;k<numberKeysToGenerate;++k)
			{
				encrypt(blocks16,keys16[i][j][k],numberRounds16[j]);
			}
			stopTime=clock();
			cout<<(1000*(double)stopTime-(double)startTime)/(CLOCKS_PER_SEC*numberKeysToGenerate*numberBlocksToGenerate)<<" ms"<<endl;
		}
	}

	cout<<"24 bit word size"<<'\n';
	for(int i=0;i<keySizes24.size();++i)
	{
		cout<<"\t"<<keySizes24[i]*24<<"-bit key"<<endl;
		for(int j=0;j<numberRounds24.size();++j)
		{
			cout<<"\t\t"<<numberRounds24[j]<<" rounds: ";
			startTime=clock();
			for(int k=0;k<numberKeysToGenerate;++k)
			{
				encrypt(blocks24,keys24[i][j][k],numberRounds24[j]);
			}
			stopTime=clock();
			cout<<1000*((double)stopTime-(double)startTime)/(CLOCKS_PER_SEC*numberKeysToGenerate*numberBlocksToGenerate)<<" ms"<<endl;
		}
	}

	cout<<"32 bit word size"<<'\n';
	for(int i=0;i<keySizes32.size();++i)
	{
		cout<<"\t"<<keySizes32[i]*32<<"-bit key"<<endl;
		for(int j=0;j<numberRounds32.size();++j)
		{
			cout<<"\t\t"<<numberRounds32[j]<<" rounds: ";
			startTime=clock();
			for(int k=0;k<numberKeysToGenerate;++k)
			{
				encrypt(blocks32,keys32[i][j][k],numberRounds32[j]);
			}
			stopTime=clock();
			cout<<1000*((double)stopTime-(double)startTime)/(CLOCKS_PER_SEC*numberKeysToGenerate*numberBlocksToGenerate)<<" ms"<<endl;
		}
	}

	cout<<"48 bit word size"<<'\n';
	for(int i=0;i<keySizes48.size();++i)
	{
		cout<<"\t"<<keySizes48[i]*48<<"-bit key"<<endl;
		for(int j=0;j<numberRounds48.size();++j)
		{
			cout<<"\t\t"<<numberRounds48[j]<<" rounds: ";
			startTime=clock();
			for(int k=0;k<numberKeysToGenerate;++k)
			{
				encrypt(blocks48,keys48[i][j][k],numberRounds48[j]);
			}
			stopTime=clock();
			cout<<1000*((double)stopTime-(double)startTime)/(CLOCKS_PER_SEC*numberKeysToGenerate*numberBlocksToGenerate)<<" ms"<<endl;
		}
	}

	cout<<"64 bit word size"<<'\n';
	for(int i=0;i<keySizes64.size();++i)
	{
		cout<<"\t"<<keySizes64[i]*64<<"-bit key"<<endl;
		for(int j=0;j<numberRounds64.size();++j)
		{
			cout<<"\t\t"<<numberRounds64[j]<<" rounds: ";
			startTime=clock();
			for(int k=0;k<numberKeysToGenerate;++k)
			{
				encrypt(blocks64,keys64[i][j][k],numberRounds64[j]);
			}
			stopTime=clock();
			cout<<1000*((double)stopTime-(double)startTime)/(CLOCKS_PER_SEC*numberKeysToGenerate*numberBlocksToGenerate)<<" ms"<<endl;
		}
	}

	cout<<"DECRYPTION\n==========\n";

		cout<<"16 bit word size"<<'\n';
	for(int i=0;i<keySizes16.size();++i)
	{
		cout<<"\t"<<keySizes16[i]*16<<"-bit key"<<endl;
		for(int j=0;j<numberRounds16.size();++j)
		{
			cout<<"\t\t"<<numberRounds16[j]<<" rounds: ";
			startTime=clock();
			for(int k=0;k<numberKeysToGenerate;++k)
			{
				decrypt(blocks16,keys16[i][j][k],numberRounds16[j]);
			}
			stopTime=clock();
			cout<<(1000*(double)stopTime-(double)startTime)/(CLOCKS_PER_SEC*numberKeysToGenerate*numberBlocksToGenerate)<<" ms"<<endl;
		}
	}

	cout<<"24 bit word size"<<'\n';
	for(int i=0;i<keySizes24.size();++i)
	{
		cout<<"\t"<<keySizes24[i]*24<<"-bit key"<<endl;
		for(int j=0;j<numberRounds24.size();++j)
		{
			cout<<"\t\t"<<numberRounds24[j]<<" rounds: ";
			startTime=clock();
			for(int k=0;k<numberKeysToGenerate;++k)
			{
				decrypt(blocks24,keys24[i][j][k],numberRounds24[j]);
			}
			stopTime=clock();
			cout<<1000*((double)stopTime-(double)startTime)/(CLOCKS_PER_SEC*numberKeysToGenerate*numberBlocksToGenerate)<<" ms"<<endl;
		}
	}

	cout<<"32 bit word size"<<'\n';
	for(int i=0;i<keySizes32.size();++i)
	{
		cout<<"\t"<<keySizes32[i]*32<<"-bit key"<<endl;
		for(int j=0;j<numberRounds32.size();++j)
		{
			cout<<"\t\t"<<numberRounds32[j]<<" rounds: ";
			startTime=clock();
			for(int k=0;k<numberKeysToGenerate;++k)
			{
				decrypt(blocks32,keys32[i][j][k],numberRounds32[j]);
			}
			stopTime=clock();
			cout<<1000*((double)stopTime-(double)startTime)/(CLOCKS_PER_SEC*numberKeysToGenerate*numberBlocksToGenerate)<<" ms"<<endl;
		}
	}

	cout<<"48 bit word size"<<'\n';
	for(int i=0;i<keySizes48.size();++i)
	{
		cout<<"\t"<<keySizes48[i]*48<<"-bit key"<<endl;
		for(int j=0;j<numberRounds48.size();++j)
		{
			cout<<"\t\t"<<numberRounds48[j]<<" rounds: ";
			startTime=clock();
			for(int k=0;k<numberKeysToGenerate;++k)
			{
				decrypt(blocks48,keys48[i][j][k],numberRounds48[j]);
			}
			stopTime=clock();
			cout<<1000*((double)stopTime-(double)startTime)/(CLOCKS_PER_SEC*numberKeysToGenerate*numberBlocksToGenerate)<<" ms"<<endl;
		}
	}

	cout<<"64 bit word size"<<'\n';
	for(int i=0;i<keySizes64.size();++i)
	{
		cout<<"\t"<<keySizes64[i]*64<<"-bit key"<<endl;
		for(int j=0;j<numberRounds64.size();++j)
		{
			cout<<"\t\t"<<numberRounds64[j]<<" rounds: ";
			startTime=clock();
			for(int k=0;k<numberKeysToGenerate;++k)
			{
				decrypt(blocks64,keys64[i][j][k],numberRounds64[j]);
			}
			stopTime=clock();
			cout<<1000*((double)stopTime-(double)startTime)/(CLOCKS_PER_SEC*numberKeysToGenerate*numberBlocksToGenerate)<<" ms"<<endl;
		}
	}


}
