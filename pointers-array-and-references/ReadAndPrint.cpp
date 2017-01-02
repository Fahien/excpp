#include <iostream>
#include <string>
#include <set>
#include <cstdlib>

using namespace std;

template<class T>
ostream& operator<<(const ostream &out, const set<T> &a_set) {
	cout << "{";
	for (auto x : a_set) { cout << x << ", "; }
	return cout << "\b\b}";
}

///  Read a sequence of words from input. Use Quit as a word that terminates the input.
/// Print the words in the order they where entered. Don't print a word twice
int main()
{
	set<string> inputs{};
	string word{};
	while (true) {
		cin >> word;
		if (word == "Quit") break;
		inputs.insert(word);
		cout << inputs << endl;
	}
	cout << "Bye!\n";
	return EXIT_SUCCESS;
}
