#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <cstdlib>

using namespace std;


/// Opens the file "integers" as an ifstream and read it
int main()
{
	ifstream is{ "integers" }; // Input stream for file "integers"
	istream_iterator<int> ii{ is }; // Input iterator for stream
	istream_iterator<int> eos{}; // Input sentinel

	vector<int> integers{ ii, eos }; // Initialize vector from input [ii, eos)

	for (auto x : integers) { cout << x << " "; } // Print vector
	cout << "\b\n";
	return EXIT_SUCCESS;
}
