#include <iostream>
#include <cstdlib>

using namespace std;

/// Prints out the letters 'a'..'z' and the digits '0'..'9' and their integer values
int main()
{
	int p{ 'a' };
	while (p <= 'z') {
		cout << p << " ";
		p++;
	}
	cout << endl;

	p = '0';
	while (p <= '9') {
		cout << p << " ";
		p++;
	}
	cout << endl;

	p = '(';
	cout << "(:" << p << endl;

	p = 0x6a;
	cout << static_cast<char>(0x6a) << ": " << p << endl;

	return EXIT_SUCCESS;
}
