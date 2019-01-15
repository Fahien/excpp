#include "Complex.h"

#include <cassert>
#include <cstdlib>

void test_imaginary()
{
	using namespace excpp;

	Imaginary one { 1 };

	// Addition
	assert( ( one + one ) == 2_i && "One_i plus one_i is not 2_i" );
	auto two = one;
	two += one;
	assert( two == 2_i && "Two_i is not 2" );

	assert( ( +one == 1_i ) && "+One_i is not 1_i" );

	// Subtraction
	assert( ( one - one ) == 0 && "One_i minus one_i is not zero" );
	assert( ( one - 1_i ) == 0 && "One_i minus 1_i is not zero" );
	assert( ( 1_i - one ) == 0 && "1_i minus one_i is not zero" );

	auto zero = one;
	zero -= one;
	assert( zero == 0 && "Zero is not 0" );

	assert( ( -one == -1_i ) && "-One is not -1_i" );

	// Multiplication
	assert( ( two * two == -4 ) && "Two_i * two_i is not -4" );
	assert( ( two * 2_i == -4 ) && "Two_i * 2_i is not -4" );
	assert( ( 2_i * two == -4 ) && "2_i * two_i is not -4" );

	// Division
	auto four = 4_i;
	assert( ( four / two == 2 ) && "Four_i / two_i is not 2" );
	assert( ( four / -2_i == -2 ) && "Four_i / -2_i is not -2" );
	assert( ( 4_i / -two == -2 ) && "4_i / -two_i is not -2" );
}

void test_complex()
{
	using namespace excpp;

	Complex a {};
	assert( a == 0 && "A is not 0" );

	// Addition
	Complex one{ 1 };
	a += one;
	assert( ( a == one ) && "A + one is not one" );

	// Subtraction
	a -= one;
	Complex zero {};
	assert( a == zero && "A is not zero" );

	// Multiplication
	a = Complex{ 3, 1_i };
	a *= Complex{ 2, 1_i };
	assert( ( a == Complex{ 5, 5_i } && "A * b is not 5+5i" ) );
}

int main()
{
	test_imaginary();
	test_complex();
	return EXIT_SUCCESS;
}
