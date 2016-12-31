#include <iostream>
#include <limits>
#include <typeinfo>
#include <cstdlib>

using namespace std;

template<class T>
void details_of(T type)
{
	cout << "Smallest " << typeid(T).name() << ": " << numeric_limits<decltype(type)>::min() << endl;
	cout << "Largest " << typeid(T).name() << ": " << numeric_limits<decltype(type)>::max() << endl;
	cout << "Size of " << typeid(T).name() << ": " << sizeof(decltype(type)) << endl;
	cout << "Align of " << typeid(T).name() << ": " << alignof(decltype(type)) << endl;
}

int main()
{
	details_of(true);
	details_of('a');
	short sh = 1;
	details_of(sh);
	details_of(1);
	details_of(1L);
	details_of(1LL);
	details_of(1.0F);
	details_of(1.0);
	details_of(1.0L);
	unsigned un = 1;
	details_of(un);
	unsigned long unl = 1;
	details_of(unl);

	return EXIT_SUCCESS;
}
