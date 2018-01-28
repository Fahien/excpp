#include <fstream>
#include <iostream>
#include <cstdlib>


/// Reads an arbitrary number of files whose names are given as
/// command-line arguments and writes them one after another on cout.
int main(int argc, char** argv)
{
	for (int i{ 1 }; i < argc; ++i)
	{
		std::ifstream is{ argv[i] };
		if (is.is_open())
		{
			std::cout << is.rdbuf();
		}
	}

	return EXIT_SUCCESS;
}
