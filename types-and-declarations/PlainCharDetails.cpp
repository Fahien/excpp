#include <iostream>
#include <cstdlib>

using namespace std;

/// Prints signed if plain chars are signed and unsigned otherwise
int main()
{
	char ch = 255;
	int i = ch;
	if (i < 0) { cout << "Plain chars are signed\n"; }
	else { cout << "Plain chars are unsigned\n"; }
	return EXIT_SUCCESS;
}
