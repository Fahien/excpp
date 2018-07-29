/// CheckedPtr is a class that uses exceptions to signal runtime errors
/// for a pointer supposed to point to an element out of an array

#include "CheckedPtr.h"

#include <cstdlib>
#include <iostream>


/// Test CheckedPtr
int main()
{
	float n[2]{ 0.0f, 1.0f };

	CheckedPtr<float> numbers{ n };

	std::cout << numbers[0] << std::endl;
	std::cout << numbers[1] << std::endl;

	try
	{
		std::cout << numbers[2] << std::endl;
	}
	catch (const OutOfRange& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	return EXIT_SUCCESS;
}
