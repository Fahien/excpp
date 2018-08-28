#include <iostream>
#include <cstdlib>

#include "Common.h"
#include "Histogram.h"


using namespace std;
using namespace excpp;


template <typename K, typename V>
void Histogram<K,V>::Print() const
{
	for ( auto& pair : m_Map )
	{
		auto& key = pair.first;
		auto& val = pair.second;

		cout << key << " ";
		for ( unsigned i { 0 }; i < val; ++i )
		{
			cout << "#";
		}
		cout << endl;
	}
}


int main()
{
	Histogram<i32, u8> his { { { 0, 1 }, { 1, 2 } } };
	his.Print();

	return EXIT_SUCCESS;
}
