#include <iostream>
#include <cstdlib>

using namespace std;

struct Large {
	bool b;
	char c;
	int i;
	long l;
	double d;
	long double ld;
};

struct Small {
	long double ld;
	double d;
	long l;
	int i;
	char c;
	bool b;
};

/// Define a struct with a member of each of the types bool, char, int, long, double and long double.
/// Order the members so as to get the largest size of the struct and the smallest size of the struct.
int main()
{
	Large large{};
	Small small{};

	cout << "Size of large: " << sizeof(large) << endl;
	cout << "Size of small: " << sizeof(small) << endl;

	return EXIT_SUCCESS;
}
