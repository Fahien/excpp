#include <cstdlib>
#include <cassert>

#include <ExString.h>


int main()
{
	using namespace excpp;

	String hello { "Hello" };
	assert( hello.get_length() == 5 && "Length is not 5" );

	String world { "world" };
	String hello_world { hello + ", " + world };
	assert( hello_world == "Hello, world" && "Content is not \"Hello, world\"" );
	assert( hello_world == String{ "Hello, world" } && "Content is not \"Hello, world\"" );
	assert( hello_world == "Hello, world"_str && "Content is not \"Hello, world\"" );

	hello_world = hello;
	assert( hello_world == "Hello" && "Content is not \"Hello\"" );

	return EXIT_SUCCESS;
}
