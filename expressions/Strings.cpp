#include <iostream>
#include <cstdlib>
#include <memory>


namespace my
{


/// Returns the length of a C-style string
size_t strlen(const char* s)
{
	size_t len{ 0 };
	// Count til the end character
	for (; *s != 0; ++s)
	{
		++len;
	}

	return len;
}


/// Copies a C-style string into another
void strcpy(const char* src, char* dst)
{
	for (; *src != 0; ++src)
	{
		*dst++ = *src;
	}
	*dst = 0;
}


/// Compares two C-style strings
int strcmp(const char* a, const char* b)
{
	for(; *a != 0 || *b != 0; ++a, ++b)
	{
		if (*a < *b)
		{
			return -1;
		}
		else if (*a > *b)
		{
			return 1;
		}
	}
	return 0;
}


/// Returns a new string which is the concatenation of a and b
char* cat(const char* a, const char* b)
{
	size_t aLen{ strlen(a) };
	size_t bLen{ strlen(b) };
	size_t cLen{ aLen + bLen + 1 };
	char* c{ new char[cLen] };

	size_t i{ 0 };
	// Copy a into c
	for (; i < aLen; ++i)
	{
		c[i] = a[i];
	}
	// Append b
	for (i = 0; i < bLen; ++i)
	{
		c[i + aLen] = b[i];
	}
	c[cLen - 1] = 0;
	return c;
}


/// Takes a C-style string argument and reverses the characters in it
void rev(char* s)
{
	size_t sLen{ strlen(s) };
	for(size_t i{ 0 }; i < sLen / 2; ++i)
	{
		char t{ s[i] };
		s[i] = s[sLen - 1 - i];
		s[sLen - 1 - i] = t;
	}
}


} // namespace my


constexpr size_t bufferMax{ 3 };

int main()
{
	const char* teststr{ "du" };
	char buffer[bufferMax]{  };

	std::cout << "Len " << teststr << " = " << my::strlen(teststr) << std::endl;
	std::cout << my::strcmp(teststr, buffer) << std::endl;
	my::strcpy(teststr, buffer);
	std::cout << buffer << std::endl;
	std::cout << my::strcmp(teststr, buffer) << std::endl;
	std::cout << my::strcmp(buffer, "a")     << std::endl;

	std::unique_ptr<char[]> pstring{ my::cat("con", "cat") };
	std::cout << pstring.get() << std::endl;

	my::rev(pstring.get());
	std::cout << pstring.get() << std::endl;

	return EXIT_SUCCESS;
}