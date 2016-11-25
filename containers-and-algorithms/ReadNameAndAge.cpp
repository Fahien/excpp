#include <iostream>
#include <cstdlib>

using namespace std;


/// Reads a name and an age from the standard input stream cin.
/// Then output a message including the name and age to the standard output stream cout.
int main()
{
	string name;
	int age;

	cout << "What's your name? ";
	cin >> name;
	cout << "How old are you? ";
	cin >> age;

	cout << "Welcome " << name << " - " << age << endl;

	return EXIT_SUCCESS;
}
