#include <iostream>

using namespace std;

bool aBool;
char aChar;
int aInt;
double aDouble;
string aString;

void print();

int main()
{
	print();

	cout << "Now put a boolean" << endl;
	cin >> aBool;
	cout << "Now put a char" << endl;
	cin >> aChar;
	cout << "Now put an int" << endl;
	cin >> aInt;
	cout << "Now put a double" << endl;
	cin >> aDouble;
	cout << "Now put a string" << endl;
	cin >> aString;

	print();
}

void print()
{
	cout
		<< aBool << endl
		<< aChar << endl
		<< aInt << endl
		<< aDouble << endl
		<< aString << endl;
}
