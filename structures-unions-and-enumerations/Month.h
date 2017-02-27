#include <iostream>
#include <cstdlib>
#include <string>


struct Month {
	static const Month months[12];

	std::string name;
	unsigned days;

	friend std::istream &operator>>(std::istream &is, Month &m);
	friend std::ostream &operator<<(std::ostream &os, const Month &m);
};


const Month Month::months[12] = {
	{"January", 31},
	{"February", 28},
	{"March", 31},
	{"April", 30},
	{"May", 31},
	{"June", 30},
	{"Jule", 31},
	{"August", 31},
	{"Semptember", 30},
	{"October", 31},
	{"November", 30},
	{"December", 31}
};


std::istream &operator>>(std::istream &is, Month &m)
{
	int n;
	is >> n;
	if (n < 1 || n > 12) {
		is.setstate(std::ios::failbit);
	}
	else {
		m = Month::months[--n];
	}
	return is;
}


std::ostream &operator<<(std::ostream &os, const Month &m)
{
	return os << m.name;
}
