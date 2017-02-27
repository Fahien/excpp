#include "Season.h"
#include <cstdlib>

int main()
{
	Season s;
	if (std::cin >> s) {
		std::cout << s << std::endl;
	}
	else {
		std::cerr << "Invalid name for season\n";
	}

	return EXIT_SUCCESS;
}
