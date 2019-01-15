#include <cstdlib>
#include <cassert>

#include "ExString.h"

using namespace excpp;


int main()
{
	String hello { "Hello" };
	assert( hello.GetLength() == 5 && "Length is not 5" );

	String world { "world" };
	String helloWorld { hello + ", " + world };
	assert( helloWorld == "Hello, world" && "Content is not \"Hello, world\"" );
	assert( helloWorld == String{ "Hello, world" } && "Content is not \"Hello, world\"" );
	assert( helloWorld == "Hello, world"_str && "Content is not \"Hello, world\"" );

	return EXIT_SUCCESS;
}
