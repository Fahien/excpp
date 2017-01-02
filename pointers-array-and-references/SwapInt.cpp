#include <iostream>
#include <cstdlib>

using namespace std;

/// Swaps(exchanges the values of) two integers. Use int∗ as the argument type.
void swap_int(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

/// Swaps(exchanges the values of) two integers. Use int& as the argument type
void swap(int& a, int& b) {
	int temp = a;
	a = b;
	b = temp;
}

void print(int& a, int&b)
{
	cout << "a = " << a << " --- b = " << b << endl;
}

int main()
{
	int one = 1;
	int two = 2;

	print(one, two);
	swap_int(&one, &two);
	print(one, two);
	swap(one, two);
	print(one, two);

	return EXIT_SUCCESS;
}
