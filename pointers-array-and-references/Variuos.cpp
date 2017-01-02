#include <iostream>

using namespace std;

void print_int_array(const int(&array)[10]) {
	cout << "{";
	for (auto x : array) { cout << x << ", "; }
	cout << "\b\b}\n";
}

void print_string(char *a_string) {
	char *p{ a_string };
	while (*p != '\0') {
		cout << *p;
		p++;
	}
}

/// Write declarations for the following:
/// a pointer to a character, an array of 10 integers, a reference to an array of 10 integers,
/// a pointer to an array of character strings, a pointer to a pointer to a character,
/// a constant integer, a pointer to a constant integer, and a constant pointer to an integer.
/// Initialize each one
int main()
{
	char a_char{ 'a' };
	cout << "a_char = " << a_char << endl;

	char *pointer_to_char{ &a_char };
	cout << "*pointer_to_char = " << *pointer_to_char << endl;

	int ten_integers[]{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	cout << "ten_integers = ";
	print_int_array(ten_integers);

	int(&ten_integers_ref)[10]{ ten_integers };
	cout << "ten_integers_ref = ";
	print_int_array(ten_integers_ref);

	char hello_string[]{ "Hello" };
	cout << "hello_string[] = ";
	print_string(hello_string);
	cout << endl;

	char *p_to_hello{ hello_string };
	cout << "*p_to_hello = ";
	print_string(p_to_hello);
	cout << endl;

	char **p_to_h{ &p_to_hello };
	cout << "*p_to_h = " << **p_to_h << endl;

	const int ten{ 10 };
	cout << "ten = " << ten << endl;
	const int *p_to_ten{ &ten };
	cout << "*p_to_ten = " << *p_to_ten << endl;

	int five{ 5 };
	cout << "five = " << five << endl;
	int const* p_to_five{ &five };
	cout << "*p_to_five = " << *p_to_five << endl;

	return EXIT_SUCCESS;
}
