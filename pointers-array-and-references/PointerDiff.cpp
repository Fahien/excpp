#include <iostream>
#include <cstdlib>

using namespace std;

/// Given two char*s pointing into an array, find and output the
/// number of characters between the two pointed-to characters.
/// Given two int*s pointing into an array, find and output the
/// number of ints between the two pointed-to-ints.
int main()
{
	char hello[]{ "hello" };

	char *p_to_h{ hello };
	char *p_to_o{ &hello[4] };

	cout << "Distance between p_to_h and p_to_o = " << p_to_o - p_to_h << endl;

	int numbers[]{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	int *p_one{ numbers };
	int *p_nine{ &numbers[8] };

	cout << "Distance between p_one and p_nine = " << p_nine - p_one << endl;
	return EXIT_SUCCESS;
}
