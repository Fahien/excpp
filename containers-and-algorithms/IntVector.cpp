#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cstdlib>

using namespace std;


/// Prints a vector to an output stream
template<typename T>
ostream &operator<<(ostream& os, vector<T> &path)
{
	if (!path.empty()) {
		os << "[";
		copy(path.begin(), path.end(), ostream_iterator<T>{ os, ", " });
		os << "\b\b]";
	}
	return os;
}


int main()
{
	vector<int> integers{ 5, 9, -1, 200, 0 }; // Initialize a vector of ints
	cout << integers << endl; // Print the vector
	sort(integers.begin(), integers.end()); // Sort the vector
	cout << integers << endl; // Print the vector again
	return EXIT_SUCCESS;
}
