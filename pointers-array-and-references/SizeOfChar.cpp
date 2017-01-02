#include <iostream>
#include <stdlib>

using namespace std;

int main()
{
	char str[]{ "a short string" };

	cout << "Size of str = " << sizeof(str) * sizeof(char) << endl;
	cout << "Lenght of str = " << sizeof(str) - 1 << endl;

	return EXIT_SUCCESS;
}
