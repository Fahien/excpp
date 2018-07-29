#include <cstdlib>
#include <iostream>

#include "Integer.h"


int main()
{
	Integer i{ 0 };

	// Simple test
	for (; i < 9; ++i)
	{
		std::cout << i + 1 << ", "
		          << i - 1 << ", "
		          << i * 2 << ", "
		          << i / 2 << std::endl;
	}

	// Test overflow
	try
	{
		while (true)
		{
			i *= 2;
		}
	}
	catch (const OverflowException& e)
	{
		std::cout << e.what() << std::endl;
	}

	// Test underflow
	i = -1;
	try
	{
		while (true)
		{
			i *= 2;
		}
	}
	catch (const UnderflowException& e)
	{
		std::cout << e.what() << std::endl;
	}

	return EXIT_SUCCESS;
}
