#include "Month.h"

using namespace std;

/// Define a table of the names of months of the year and the number of days in each month.
/// Write out that table. Use an array of structures, with each structure holding the name of
/// a month and the number of days in it.
int main()
{
	for (unsigned i{ 0 }; i < 12; ++i) {
		cout << Month::months[i].name << " [" << Month::months[i].days << "]\n";
	}

	Month m;
	if (cin >> m) {
		cout << m << std::endl;
	}
	else {
		cerr << "Invalid number for month\n";
	}

	return EXIT_SUCCESS;
}
