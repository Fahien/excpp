#include <iostream>
#include <cstdlib>


void print(const bool a, const bool b)
{
	std::cout << a << "\t"
	          << b << "\t"
	          << (a & b) << "\t"
	          << (a | b) << "\t"
	          << (a ^ b) << std::endl;
}


/// Write a table of values for the bitwise logical operations
/// for all possible combinations of 0 and 1 operands.
int main()
{
	std::cout << "a\tb\ta&b\ta|b\ta^b\n\n";
	print(false, false);
	print(true, false);
	print(false, true);
	print(true, true);

	return EXIT_SUCCESS;
}
