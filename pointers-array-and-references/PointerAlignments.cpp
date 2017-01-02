#include <iostream>
#include <cstdlib>

using namespace std;

int main()
{
	cout << "Align of char* =\t" << alignof(char*) << endl;
	cout << "Align of int*  =\t" << alignof(int*) << endl;
	cout << "Align of void* =\t" << alignof(void*) << endl;
	return EXIT_SUCCESS;
}
