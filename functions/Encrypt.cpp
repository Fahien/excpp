/// Write an encryption program that reads from cin and writes the encoded
/// characters to cout. You might use this simple encryption scheme: the
/// encrypted form of a character c is c^key[i], where key is a string
/// passed as a command-line argument. The program uses the characters in
/// key in a cyclic manner until all the input has been read.
/// Re-encrypting encoded text with the same key produces the original test.
/// If no key (or a null string) is passed, then no encryption is done.

#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>


char encrypt(char c, char k)
{
	return c ^ k;
}


int main(int argc, char** argv)
{
	std::string key{ "" };

	if (argc >= 2)
	{
		key = argv[1];
	}

	char c{ 0 };
	for (size_t i{ 0 }; true; ++i)
	{
		c = std::cin.get();

		if (std::cin.eof())
		{
			break;
		}

		size_t index{ 0 };
		char k;

		if (key.length() == 0)
		{
			k = 0;
		}
		else
		{
			index = i % key.length();
			k     = key[index];
		}

		char encrypted{ encrypt(c, k) };
		std::cout << encrypted;
	}

	return EXIT_SUCCESS;
}
