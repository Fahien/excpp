#include <cstdlib>
#include <iostream>

#include "Vector.h"

using namespace std;
using namespace excpp;

int main()
{
	Vector<int> v1;
	if (v1.IsEmpty())
	{
		cout << "v1 is empty now" << endl;
	}

	for (int i{ 0 }; i < 24; ++i)
	{
		v1.Push(i);
	}
	cout << v1.Size() << endl;

	Vector<int> v2{ v1 };
	cout << v2.Size() << endl;

	Vector<int> v3{};
	cout << v3.Size() << endl;
	v3 = v2;
	cout << v3.Size() << endl;

	for (int i{ 0 }; i < 24; ++i)
	{
		cout << v3.At(i) << " ";
	}
	cout << endl;

	cout << v3.Front() << " - " << v3.Back() << endl;

	return EXIT_SUCCESS;
}