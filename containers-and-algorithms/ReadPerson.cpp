#include <iostream>
#include <cstdlib>

using namespace std;


struct Person {
	string name;
	int age;
};


/// Writes a person to an output stream
ostream &operator<<(ostream &os, const Person &p)
{
	return os << p.name << "{" << p.age << "}";
}


/// Reads name{age} pair
istream &operator >>(istream &is, Person &p)
{
	char c;
	string name;

	// Anything before { is part of the name 
	while (is.get(c) && c != '{') { name += c; }

	int age;
	// Read the age and a }
	if (is >> age >> c && c == '}') {
		// Assign to the members
		p = { name, age };
	}
	else { // Register the failure in the stream
		is.setstate(ios_base::failbit);
	}

	return is;
}


/// Reads several (name, age) pairs in a class using >> and << operators
int main()
{
	cout << "Write name{age} pairs\n";
	Person person;
	while (cin >> person) { cout << person << endl; }
	return EXIT_SUCCESS;
}
