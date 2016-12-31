#include <iostream>
#include <cstdlib>

using namespace std;

/// Prints the sizes of the fundamental types, a few pointer types, and a few enumerations
int main()
{
	cout << "Size of bool:\t" << sizeof(bool) << endl;
	cout << "Size of wchar_t:\t" << sizeof(wchar_t) << endl;
	cout << "Size of char:\t" << sizeof(char) << endl;
	cout << "Size of short:\t" << sizeof(short) << endl;
	cout << "Size of int:\t" << sizeof(int) << endl;
	cout << "Size of float:\t" << sizeof(float) << endl;
	cout << "Size of double:\t" << sizeof(double) << endl;
	cout << endl;
	cout << "Size of pointer to void:\t" << sizeof(void*) << endl;
	cout << "Size of pointer to int:\t" << sizeof(int*) << endl;
	cout << "Size of pointer to float:\t" << sizeof(float*) << endl;
	cout << endl;
	enum Color { red, green, blue };
	cout << "Size of Color enum:\t" << sizeof(Color) << endl;
	enum access_t { read = 1, write = 2, exec = 4 };
	cout << "Size of access_t enum:\t" << sizeof(access_t) << endl;
	return EXIT_SUCCESS;
}
