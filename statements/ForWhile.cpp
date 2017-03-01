/// Rewrite the following for-statement as an equivalent while-statement
/// Rewrite it to use a pointer as the controlled variable, that is, so
/// that the test is of the form *p == '?'. Rewrite it to use a range-for#include <iostream>

#include <iostream>
#include <string>
#include <cstdlib>


int main()
{
	std::string line{ "? count ? should ? be ? 4" };
	size_t len{ line.length() };
	unsigned count{ 0 };

	// For
	for (size_t i{ 0 }; i != len; ++i)
	{
		if (line[i] == '?')
		{
			++count;
		}
	}

	std::cout << "For: count = " << count << std::endl;

	// While
	count = 0;
	size_t i{ 0 };
	while (i != len)
	{
		if (line[i] == '?')
		{
			++count;
		}
		++i;
	}

	std::cout << "While: count = " << count << std::endl;

	// While *
	count = 0;
	const char *p { line.c_str() };
	while (*p)
	{
		if (*p == '?')
		{
			++count;
		}
		++p;
	}

	std::cout << "While *: count = " << count << std::endl;

	// Range for
	count = 0;
	for(char c : line)
	{
		if (c == '?')
		{
			++count;
		}
	}

	std::cout << "Range for: count = " << count << std::endl;

	return EXIT_SUCCESS;
}
