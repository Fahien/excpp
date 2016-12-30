#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>

using namespace std;

/// Time a loop. Write out the time in milliseconds
int main()
{
	auto t1 = chrono::high_resolution_clock::now();
	for (int i = 0; i < 10; i++) {
		cout << "Iteration " << i + 1 << endl;
		this_thread::sleep_for(chrono::milliseconds{ 1000 });
	}
	auto t2 = chrono::high_resolution_clock::now();
	cout << (t2 - t1).count() << " nanoseconds passed\n";
	return EXIT_SUCCESS;
}
