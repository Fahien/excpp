#include <iostream>
#include <cstdlib>
#include "Month.h"

using namespace std;

struct Date {
	Date() {}
	Date(Month m, unsigned d, int y)
		: month{ m }
		, day{ d }
		, year{ y }
	{}

	Month month;
	unsigned day;
	int year;

	friend ostream &operator<<(ostream &os, const Date &d)
	{
		return os << d.month.name << " " << d.day << ", " << d.year;
	}

	friend istream &operator>>(istream &is, Date &d)
	{
		is >> d.month >> d.day >> d.year;
		if (!is.fail() && (d.day < 1 || d.day > d.month.days)) {
			is.setstate(ios::failbit);
		}
		return is;
	}
};

int main()
{
	Date d;
	if (cin >> d) {
		cout << d << std::endl;
	}
	else {
		cerr << "Invalid format for date\n";
	}

	return EXIT_SUCCESS;
}
