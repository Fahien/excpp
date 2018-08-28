#include <cstdlib>
#include <cassert>
#include <string>

#include "Set.h"

using namespace std;
using namespace excpp;


int main()
{
	Set<string> a {};
	string one { "1" };
	a.Insert( one );
	a.Insert( one );
	assert( a.GetSize() == 1 && "Size is not 1" );

	Set<int> b {};
	for ( int i { 0 }; i < 10; ++i )
	{
		b.Insert( i );
	}
	assert( b.GetSize() == 10 && "Size is not 10" );

	Set<int> c { 9, 10, 11 };
	assert( c.GetSize() == 3 && "Size is not 3" );

	Set<int> d { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	assert( d.GetSize() == 10 && "Size is not 10" );

	auto result = c.Union( d );
	assert( result.IsOk() );
	assert( c.GetSize() == 12 && "Size is not 12" );

	return EXIT_SUCCESS;
}
