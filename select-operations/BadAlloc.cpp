#include <iostream>
#include <cstdlib>
#include <chrono>

using namespace std;


/// Allocate so much memory using new that bad_alloc is thrown.
/// Report how much memory was allocated and how much time it took.
int main()
{
	/// Start chrono
	auto t1 = chrono::high_resolution_clock::now();

	/// Allocate memory exponentially
	int bytes{ 1 };
	int total{ bytes };
	try
	{
		while (true)
		{
			auto memory = new char[bytes];
			total += bytes;
			bytes *= 2;
		}
	}
	catch (const bad_alloc& e)
	{
		cout << "Bad alloc exception captured\n";
	}

	/// Stop chrono and print output
	auto t2 = chrono::high_resolution_clock::now();
	cout << (t2 - t1).count() / 1000000.0 << " milliseconds passed\n";
	cout << total / 1000 << "MB allocated\n";

	return EXIT_SUCCESS;
}
