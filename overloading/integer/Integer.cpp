#include "Integer.h"

#include <cassert>
#include <cstdlib>

int main()
{
	using namespace excpp;

	Integer one { 1 };

	// Addition
	assert( ( one + one ) == 2 && "One plus one is not two" );
	assert( ( one + 1 ) == 2 && "One plus 1 is not two" );
	assert( ( 1 + one ) == 2 && "1 plus one is not two" );
	
	auto two = one;
	two += one;
	assert( two == 2 && "Two is not 2" );

	assert( ( +one == 1 ) && "+One is not 1" );

	// Subtraction
	assert( ( one - one ) == 0 && "One minus one is not zero" );
	assert( ( one - 1 ) == 0 && "One minus 1 is not zero" );
	assert( ( 1 - one ) == 0 && "1 minus one is not zero" );

	auto zero = one;
	zero -= one;
	assert( zero == 0 && "Zero is not 0" );

	assert( ( -one == -1 ) && "-One is not -1" );

	// Multiplication
	assert( ( two * two == 4 ) && "Two * two is not 4" );
	assert( ( two * 2 == 4 ) && "Two * 2 is not 4" );
	assert( ( 2 * two == 4 ) && "2 * two is not 4" );

	auto four = two;
	four *= two;
	assert( ( four == 4 ) && "Four is not 4" );

	// Division
	assert( ( four / two  == two ) && "Four / two is not two" );
	assert( ( four / 2  == 2 ) && "Four / 2 is not 2" );
	assert( ( 4 / two  == 2 ) && "4 / two is not 2" );

	auto eight = 16;
	eight /= two;
	assert( ( eight == 8 ) && "Eight is not 8" );

	// Modulo
	assert( ( four % two == 0 ) && "Four %% two is not 0" );
	assert( ( four % 2 == 0 ) && "Four %% 2 is not 0" );
	assert( ( 4 % two == 0 ) && "4 %% two is not 0" );

	zero = four;
	zero %= two;
	assert( ( zero == 0 ) && "Zero is not 0" );

	return EXIT_SUCCESS;
}
