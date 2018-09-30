#include <iostream>
#include <cstdlib>
#include <cassert>

#include "Handle.h"

using namespace excpp;


int main()
{
	Handle<int> a { new int{ 1 } };
	assert( *a == 1 && "a is not one" );

	Handle<int> b { std::move( a ) };
	assert( *b == 1 && "b is not one" );

	Handle<int> c { new int{ 2 } };
	c = std::move( b );
	assert( *c == 1 && "c is not one" );

	return EXIT_SUCCESS;
}