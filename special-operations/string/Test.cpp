#include <cstdlib>
#include <cassert>
#include <cstring>
#include <iostream>

#include "Iterator.h"
#include "ExString.h"

int main()
{
	using namespace excpp;

	String hello { "Hello" };

	StringIterator it { hello };

	try
	{
		while ( auto& c = it.next() )
		{
			std::cout << c << std::endl;
		}
	}
	catch( const std::runtime_error& e )
	{
	}

	// Test substring
	auto ell = hello( 1, 3 );
	std::cout << ell << std::endl;
	assert( ell == "ell" );

	ell = "eelll";
	std::cout << hello << std::endl;
	assert( hello == "Heelllo" );

	auto c_hello = static_cast<char*>( hello );
	std::cout << c_hello << std::endl;
	assert( std::strlen( c_hello ) == hello.get_length() );
	delete[] c_hello;

	return EXIT_SUCCESS;
}