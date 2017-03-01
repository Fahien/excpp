#include <iostream>
#include <cstdlib>


/// See how your compiler reacts to these errors
int main()
{
	int a, b;

	if (a = 3)
	{
		std::cout << "= instead of  ==\n";
	}

	if (a&077 == 0)
	{
		std::cout << "& check operator precedence for possible error\n"
		          << "use parentheses to clarify precedence\n";
	}

	// Syntax error =
	a := b + 1;

	return EXIT_SUCCESS;
}