#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cstdlib>

using namespace std;


/// Prints a generic vector
template<typename T>
ostream &operator<<(ostream &os, vector<T> &v)
{
	if (!v.empty()) {
		cout << "[";
		copy(v.begin(), v.end(), ostream_iterator<T>{ os, ", " });
		cout << "\b\b]";
	}
	return os;
}


int main()
{
	// Initialize a vector of strings
	vector<string> names{ "Kant", "Plato", "Aristotle", "Kierkegard", "Hume" };
	cout << names << endl; // Print the vector
	sort(names.begin(), names.end()); // Sort the vector
	cout << names << endl; // Print the vector again
	return EXIT_SUCCESS;
}
