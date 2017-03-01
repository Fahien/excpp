/// Write a program that strips comments out of a C++ program.
/// That is, read from cin, remove both // comments and /*∗*/ comments, and
/// write the result to cout. Do not worry about making the layout of the
/// output look nice (that would be another, and much harder exercise).
/// Do not worry about incorrect programs. Beware of //, /*∗, and∗*/
/// in comments, strings, and character constants.

#include <iostream>
#include <cstdlib>

int main(void)
{
	bool inMultilineComment{ false };
	bool inOnelineComment{ false };
	bool inString{ false };
	bool inChar{ false };

	/*
	 * Test
	 * Multiline
	 * Comment
	 */

	char c;

	while(!std::cin.eof())
	{
		// Get the next char
		c = std::cin.get();

		// Test in multiline comment
		if (inMultilineComment)
		{
			if (c == '*')
			{
				char cNext = std::cin.peek();
				if (cNext == '/')
				{
					std::cin.get();
					inMultilineComment = false;
				}
			}
			continue;
		}
		// Test in oneline comment
		else if (inOnelineComment)
		{
			if (c == '\n')
			{
				inOnelineComment = false;
			}
			else continue;
		}
		// Test in string
		else if (inString)
		{
			if (c == '\\')
			{
				char cNext = std::cin.peek();
				if (cNext == '"')
				{
					std::cout << c;
					c = std::cin.get();
				}
			}
			else if (c == '"')
			{
				inString = false;
			}
		}
		// Test in character constant
		else if (inChar)
		{
			if (c == '\\')
			{
				char cNext = std::cin.peek();
				if (cNext == '\'')
				{
					std::cout << c;
					c = std::cin.get();
				}
			}
			else if (c == '\'')
			{
				inChar = false;
			}
		}
		// Test in normal flow
		else if (!inString && !inChar)
		{
			switch (c)
			{
				char cNext;
				case '/':
					cNext = std::cin.peek();
					if (cNext == '*')
					{
						inMultilineComment = true;
						std::cin.get();
						continue;
					}
					else if (cNext == '/')
					{
						inOnelineComment = true;
						std::cin.get();
						continue;
					}
					break;
				case '"':
					inString = true;
					break;
				case '\'':
					inChar = true;
					break;
				default:
					break;
			}
		}
		std::cout << c;
	}
	std::cout << std::endl;

	return EXIT_SUCCESS;
}
