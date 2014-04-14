#ifndef FUNCTIONLIB_H
#define FUNCTIONLIB_H
#include <iostream>

using namespace std;
template <typename integerType>
inline integerType rotate(integerType number,signed int distance)
{
	if(distance>0)
	{
		return number<<distance | number>>(sizeof(number)*8-distance);
	}
	else return number>>(0-distance) | number<<(sizeof(number)*8+distance);
}
template <typename integerType>
inline integerType msh(integerType number)
{
	return number>>(8*sizeof(integerType)/2);
}
template <typename integerType>
inline integerType lsh(integerType number)
{
	return number<<(8*sizeof(integerType)/2)>>(8*sizeof(integerType)/2);
}
template <typename iterableContainer>
void printContainer(iterableContainer container,char delimiter=',')
{
	for(auto it=container.begin();it!=container.end();++it)
	{
		cout<<*it<<delimiter;
	}
    cout<<"\b \b";
}
#endif // FUNCTIONLIB_H
