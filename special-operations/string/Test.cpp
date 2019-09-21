#include <cstdlib>
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

	return EXIT_SUCCESS;
}