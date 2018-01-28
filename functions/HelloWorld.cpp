#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>


std::vector<std::string> getCliArgs(char** argv, int argc)
{
	std::vector<std::string> args(argc - 1);

	for (int i{ 1 }; i < argc; ++i)
	{
		args[i - 1] = argv[i];
	}

	return args;
}


void sayHello(std::vector<std::string> names)
{
	for (auto name : names)
	{
		std::cout << "Hello " << name << std::endl;
	}
}


/// This program takes any number of names as a
/// command-line argument and says hello to each
int main(int argc, char** argv)
{
	std::vector<std::string> args{ getCliArgs(argv, argc) };

	sayHello(args);

	return EXIT_SUCCESS;
}
