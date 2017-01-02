#include <iostream>
#include <cstdlib>

using namespace std;

/// Use an alias (using) to define the types unsigned char, const unsigned char, pointer to
/// integer, pointer to pointer to char, pointer to array of char, array of 7 pointers to int,
/// pointer to an array of 7 pointers to int, and array of 8 arrays of 7 pointers to int.
int main()
{
	using uchar = unsigned char;
	uchar a_char{ 'a' };
	cout << "a_char = " << a_char << endl;

	using cuchar = const uchar;
	cuchar c_char = 'c';
	cout << "c_char = " << c_char << endl;

	int one{ 1 };
	using pint = int*;
	pint pint_to_one{ &one };
	cout << "*pint_to_one = " << *pint_to_one << endl;

	char b{ 'b' };
	char *p_to_b{ &b };
	using ppchar = char**;
	ppchar ppchar_to_b{ &p_to_b };
	cout << "**ppchar_to_b = " << **ppchar_to_b << endl;

	char abc[]{ "abc" };
	using pachar = char*;
	pachar pachar_to_abc = (abc);
	cout << "*pachar_to_abc = " << *pachar_to_abc << endl;

	int two{ 2 };
	int three{ 3 };
	int four{ 4 };
	int five{ 5 };
	int six{ 6 };
	int seven{ 7 };
	using asevint = int*[7];
	asevint seven_p_to_i = { &one, &two, &three, &four, &five, &six, &seven };
	cout << "seven_p_to_i = {";
	for (auto p : seven_p_to_i) {
		cout << *p << ", ";
	}
	cout << "\b\b}\n";

	using pasevint = asevint*;
	pasevint pto_seven_p_to_i{ &seven_p_to_i };
	cout << "***pto_seven_p_to_i = " << ***pto_seven_p_to_i << endl;

	using eightpasevint = pasevint[8];
	eightpasevint eight_psi{ pto_seven_p_to_i };
	cout << "***eight_psi[0] = " << ***eight_psi[0] << endl;

	return EXIT_SUCCESS;
}
