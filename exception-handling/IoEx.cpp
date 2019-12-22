#include <string>
#include <cstring>

#include "IoEx.h"


FILE* excpp::fopenEx(const std::string& name, const std::string& mode)
{
	return fopenEx(name.c_str(), mode.c_str());
}

FILE* excpp::fopenEx(const char* name, const char* mode)
{
	FILE* file{ fopen(name, mode) };
	if (!file)
	{
		throw excpp::IOException{ std::string{ "Could not open: "} + std::strerror(errno) };
	}
	return file;
}


size_t excpp::freadEx(void* buffer, size_t size, size_t count, FILE* stream)
{
	size_t ret{ fread(buffer, size, count, stream) };
	if (ret != count)
	{
		if (feof(stream))
		{
			throw excpp::IOException{ std::string{ "End Of File while reading" } };
		}
		if (ferror(stream))
		{
			throw excpp::IOException{ std::string{ "Error while reading: " } + std::strerror(errno) };
		}
	}
	return ret;
}


size_t excpp::fwriteEx(const void* buffer, size_t size, size_t count, FILE* stream)
{
	size_t ret{ fwrite(buffer, size, count, stream) };
	if (ret != count)
	{
		throw excpp::IOException{ std::string{ "Error while writing: " } + std::strerror(errno) };
	}
	return ret;
}


int excpp::fcloseEx(FILE* stream)
{
	int ret{ fclose(stream) };
	if (ret == EOF)
	{
		throw excpp::IOException{ std::string{ "Error while closing: " } + std::strerror(errno) };
	}
	return ret;
}


/// Tests the previous functions
int main()
{
	FILE* file{ nullptr };

	try
	{
		file = excpp::fopenEx("test", "r");
		excpp::fcloseEx(file);
	}
	catch(excpp::IOException& e)
	{
		std::cout << "1 Exception: " << e.what() << std::endl;
	}

	try
	{
		file = excpp::fopenEx("test2", "r");
		excpp::fcloseEx(file);
		excpp::fcloseEx(file);
	}
	catch(excpp::IOException& e)
	{
		std::cout << "2 Exception: " << e.what() << std::endl;
	}

	try
	{
		file = excpp::fopenEx("test2", "r");

		constexpr size_t bufferSize{ 6 };
		char buffer[bufferSize]{ 0 };
		size_t count{ 4 };
		excpp::freadEx(buffer, bufferSize, count, file);
		excpp::fcloseEx(file);
	}
	catch(excpp::IOException& e)
	{
		std::cout << "3 Exception: " << e.what() << std::endl;
	}

	try
	{
		file = excpp::fopenEx("test2", "r");

		constexpr size_t bufferSize{ 6 };
		char buffer[bufferSize]{ "Test" };
		size_t count{ 4 };
		excpp::freadEx(buffer, bufferSize, count, file);
		buffer[count] = 0;
		std::cout << buffer << std::endl;
		excpp::fcloseEx(file);
	}
	catch(excpp::IOException& e)
	{
		std::cout << "4 Exception: " << e.what() << std::endl;
	}

	FILE* file3{ nullptr };
	try
	{
		file3 = excpp::fopenEx("test3", "r");

		constexpr size_t bufferSize{ 3 };
		char buffer[bufferSize]{ 0 };
		size_t count{ 2 };
		excpp::freadEx(buffer, bufferSize, count, file);
		buffer[count] = 0;
		std::cout << buffer << std::endl;
		excpp::fcloseEx(file3);
	}
	catch(excpp::IOException& e)
	{
		std::cout << "5 Exception: " << e.what() << std::endl;
	}

	return EXIT_SUCCESS;
}
