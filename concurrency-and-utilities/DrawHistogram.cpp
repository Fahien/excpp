#include <iostream>
#include <random>
#include <functional>
#include <cstdlib>

using namespace std;

/// Draws a histogram
int main()
{
	default_random_engine re{}; // Create a random engine
	normal_distribution<> zero_to_twentynine{ 14, 5 }; // Create a normal distribution
	auto die = bind(zero_to_twentynine, re); // Create a generator

	vector<int> histogram(30); // Create a histogram vector
	// Fill histogram with the frequencies of numbers [0:29]
	for (int i = 0; i != 30; ++i) {
		unsigned int x = round(die());
		if (x < histogram.size()) {
			cout << "generating value for " << x << endl;
			++histogram[x];
		}
		else { cout << "invalid value for die(): " << x << endl; }
	}

	cout << "Drawing the Histogram\n";
	// Write out a bar graph
	for (unsigned int i = 0; i < 30; ++i) {
		cout << i << "\t";
		for (int j = 0; j != histogram[i]; ++j) { cout << '*'; }
		cout << endl;
	}

	return EXIT_SUCCESS;
}
