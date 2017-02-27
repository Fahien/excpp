#include <iostream>
#include <algorithm>
#include <string>

#define SEASON_COUNT 4

/// Implement an enum called Season with enumerators spring, summer, autumn, and winter.
/// Define operators ++ and −− for Season. Define input (>>) and output (<<) operations
/// for Season, providing string values. Provide a way to control the mapping between
/// Season values and their string representations.

enum Season {
	Spring,
	Summer,
	Autumn,
	Winter
};

const std::string seasonNames[SEASON_COUNT] = {
	"spring",
	"summer",
	"autumn",
	"winter"
};

Season &operator++(Season& s)
{
	return s = static_cast<Season>((static_cast<int>(s) + 1) % 4);
}

Season &operator--(Season& s)
{
	return s = static_cast<Season>((static_cast<int>(s) - 1) % 4);
}

Season operator++(Season& s, int n)
{
	return Season{ static_cast<Season>((static_cast<int>(s) + n) % 4) };
}

Season operator--(Season& s, int n)
{
	return Season{ static_cast<Season>((static_cast<int>(s) - n) % 4) };
}


std::istream &operator>>(std::istream &is, Season &s)
{
	std::string name;
	is >> name;
	if (!is.fail()) {
		for (unsigned i{ 0 }; i < SEASON_COUNT; ++i) {
			std::transform(name.begin(), name.end(), name.begin(), ::tolower);
			if (seasonNames[i] == name) {
				s = static_cast<Season>(i);
				return is;
			}
		}
		is.setstate(std::ios::failbit);
	}
	return is;
}

std::ostream &operator<<(std::ostream &os, const Season &s)
{
	return os << seasonNames[static_cast<int>(s)];
}
