/// Write a function that takes a C-style string containing digits and
/// returns the corresponding int. Handle C++ octal and hexadecimal
/// notation. Handle the C++ character constant notation

#include <iostream>
#include <cstdlib>
#include <locale>


namespace my
{

int atoi(const char* str);

enum class IntType
{
	DECIMAL,
	OCTAL,
	HEXADECIMAL
};

}

// Test atoi function
int main()
{
	std::cout << my::atoi("0")  << std::endl;
	std::cout << my::atoi("1")  << std::endl;
	std::cout << my::atoi("19") << std::endl;

	std::cout << my::atoi("00")  << std::endl;
	std::cout << my::atoi("01")  << std::endl;
	std::cout << my::atoi("017") << std::endl;
	std::cout << my::atoi("09")  << std::endl;

	std::cout << my::atoi("0x0")  << std::endl;
	std::cout << my::atoi("0X1")  << std::endl;
	std::cout << my::atoi("0xf")  << std::endl;
	std::cout << my::atoi("0xa9") << std::endl;
	std::cout << my::atoi("0xaf") << std::endl;
	std::cout << my::atoi("0xag") << std::endl;
	std::cout << my::atoi("0xff") << std::endl;

	return EXIT_SUCCESS;
}


int my::atoi(const char* str)
{
	// Classify a single char using default locale
	auto& f = std::use_facet<std::ctype<char>>(std::locale());

	my::IntType type{ my::IntType::DECIMAL };
	int multiplier{ 10 };
	int result{ 0 };

	// Check whether it is an octal/hexadecimal
	if (str[0] == '0')
	{
		// Check for hexadecimal
		if (str[1] == 'x' || str[1] == 'X')
		{
			type = IntType::HEXADECIMAL;
			multiplier = 16;
			result = 0x0;
			str += 2;
		}
		// Check for octal
		else if (f.is(std::ctype_base::digit, str[1]))
		{
			type = IntType::OCTAL;
			multiplier = 8;
			result = 00;
			str += 1;
		}
		else if (str[1] == 0)
		{
			return 0; // Just 0
		}
		else
		{
			std::cerr << "Invalid character " << str[1] << std::endl;
			return 0; // Default value for error
		}
	}

	// Scan the string till the \0
	for (; *str != 0; ++str)
	{
		result *= multiplier;
		switch (type)
		{
			case my::IntType::DECIMAL:
			{
				if (f.is(std::ctype_base::digit, *str))
				{
					result += *str - '0';
				}
				break;
			}
			case my::IntType::OCTAL:
			{
				if (f.is(std::ctype_base::digit, *str))
				{
					if (*str == '8' || *str == '9')
					{
						std::cerr << "Invalid character in octal " << *str << std::endl;
						return 0; // Default value error
					}
					result += *str - '0';
				}
				break;
			}
			case my::IntType::HEXADECIMAL:
			{
				if (f.is(std::ctype_base::xdigit, *str))
				{
					if (*str >= '0' && *str <= '9')
					{
						result += *str - '0';
					}
					else if (*str >= 'a' && *str <= 'f')
					{
						result += 10 + (*str - 'a');
					}
					else if (*str >= 'A' && *str <= 'F')
					{
						result += 10 + (*str - 'A');
					}
					else
					{
						std::cerr << "Invalid character in hexadecimal " << *str << std::endl;
						return 0; // Default value error
					}
				}
				break;
			}
		}
	}

	return result;
}
