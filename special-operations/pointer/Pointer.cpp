/// Complete a class Pointer and test it.
/// To be complete, Pointer must have at least the operators *, ->, =, ++, and -- defined.
/// Do not cause a run-time error until a wild pointer is actually dereferenced.

#include "Pointer.h"

#include <cstdlib>
#include <cassert>
#include <iostream>
#include <string>

struct Spaceship
{
	const char* name;
	size_t power;
};

int main()
{
	using namespace excpp;

	Spaceship ships[2] { { "Heart of Gold", 999 }, { "X-Wing", 300 } };
	auto array = Pointer<Spaceship>{ ships };

	assert( (*array).power == 999 );
	assert( array->power == 999 );

	assert( ( array++ )->power == 999 );
	assert( array->power == 300 );

	try
	{
		assert( ( ++array )->power == 0 && false );
	}
	catch( const std::runtime_error& e )
	{
		assert( std::string{ e.what() } == "Out of bounds" );
	}
	

	return EXIT_SUCCESS;
}
