#include <iostream>
#include <fstream>
#include <regex>
#include <cstdlib>

using namespace std;

/// Finds all decimal numbers in a file using a regex
int main() {
	ifstream is{ "regex" }; // Open a file input stream
	regex pattern(R"(\d+)"); // Create a regex pattern for decimals

	smatch matches;
	for (string line; getline(is, line);) {
		// Search for pattern and print out matches
		if (regex_search(line, matches, pattern)) {
			for (auto x : matches) {
				cout << x << " ";
			}
			cout << endl;
		}
	}
	return EXIT_SUCCESS;
}
