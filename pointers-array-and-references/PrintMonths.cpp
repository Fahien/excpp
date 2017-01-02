#include <iostream>
#include <cstdlib>

using namespace std;

void print_months(char months[12][12])
{
	cout << "Months = {";
	for (int i = 0; i < 12; i++) {
		for (int j = 0; months[i][j] != '\0'; j++) {
			cout << months[i][j];
		}
		cout << ", ";
	}
	cout << "\b\b}\n";
}

/// Define an array of strings in which the strings contain the names of the months.
/// Print those strings. Pass the array to a function that prints those strings.
int main()
{
	char months[12][12]{
			"Genuary",
			"February",
			"March",
			"April",
			"May",
			"June",
			"Jule",
			"August",
			"September",
			"October",
			"November",
			"December"
	};

	print_months(months);
	return EXIT_SUCCESS;
}
