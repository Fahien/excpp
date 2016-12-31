#include <iostream>
#include <list>
#include <cstdlib>

using namespace std;

/// Loop that prints out the values 4, 5, 9, 17, 12 without using an array or a vector
int main()
{
	list<int> integers{ 4, 5, 9, 17, 12 };
	cout << "{";
	for (auto x : integers) { cout << x << ", "; }
	cout << "\b\b}\n";
	return EXIT_SUCCESS;
}
