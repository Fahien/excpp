#include <iostream>
#include <algorithm>
#include <iterator>
#include <fstream>

using namespace std;


/// Open a file for writing (as an ofstrem) and write a few hundred integers to it
int main()
{
	ofstream os{ "integers" }; // Output stream for file "integers"
	vector<int> integers; // Initialize a vector of integers
	for (int i = 0; i < 100; i++) { integers.push_back(i); }
	copy(integers.begin(), integers.end(), ostream_iterator<int>{os, " "}); // Copy vector to output
	return !os; // Return error state
}
