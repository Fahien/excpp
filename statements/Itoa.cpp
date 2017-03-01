#include <iostream>
#include <cstdlib>


namespace my
{

/// Creates a string representation of i in b and returns b
char* itoa(int i, char* b)
{
	// Base case
	if (i == 0)
	{
		b = new char[2];
		b[0] = '0';
		b[1] = '\0';
		return b;
	}

	// Calculate the number of characters
	int tmp{ i };
	int len{ 0 };
	while (tmp != 0)
	{
		tmp /= 10;
		++len;
	}

	// Check the sign
	if (i < 0)
	{
		++len;
	}

	b = new char[len + 1];
	b[len] = '\0';


	// Put the minus
	if (i < 0)
	{
		i = -i;
		b[0] = '-';
	}

	// Use modulus to extract numbers
	int mod = i % 10;
	while (i != 0)
	{
		b[len - 1] = '0' + mod;
		i /= 10;
		mod = i % 10;
		--len;
	}

	return b;
}

}


int main(int argc, char** argv)
{
	char* strs[8]{ 0 };

	int i{ 0 };

	std::cout << my::itoa(0,      strs[i++]) << std::endl;
	std::cout << my::itoa(1,      strs[i++]) << std::endl;
	std::cout << my::itoa(-1,     strs[i++]) << std::endl;
	std::cout << my::itoa(12376,  strs[i++]) << std::endl;
	std::cout << my::itoa(-987,   strs[i++]) << std::endl;
	std::cout << my::itoa(9090,   strs[i++]) << std::endl;
	std::cout << my::itoa(0x19,   strs[i++]) << std::endl;
	std::cout << my::itoa(000042, strs[i++]) << std::endl;

	for (auto s : strs)
	{
		delete s;
	}

	return EXIT_SUCCESS;
}
