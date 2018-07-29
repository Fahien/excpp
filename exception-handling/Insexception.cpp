#include <cstdlib>
#include <stdexcept>
#include <chrono>
#include <iostream>

using namespace std;


constexpr unsigned kMaxDepth = 10;

void foo(const unsigned max);

void bar(const unsigned max);

/// Program consisting of functions calling each other to a calling depth of 10.
/// Each function has an argument that determines at which level an exception is thrown.
/// Main catches these exceptions a prints out which exception is caucht.
int main()
{
	for (unsigned i { 0 }; i <= kMaxDepth; ++i)
	{
		auto t1 = chrono::high_resolution_clock::now();
		try
		{
			foo(i);
		}
		catch (runtime_error& e)
		{
			auto t2 = chrono::high_resolution_clock::now();
			cout << (t2 - t1).count() << " nanoseconds passed" << endl;
			cout << "Exception captured from level " << i + 1 << endl;
		}
	}

	return EXIT_SUCCESS;
}


void foo(const unsigned max)
{
	static unsigned level { 0 };

	if (level == kMaxDepth)
	{
		return;
	}

	if (level >= max)
	{
		throw runtime_error { "Depth max reached" };
	}

	++level;

	bar(max);
}


void bar(const unsigned max)
{
	static unsigned level { 0 };

	if (level == kMaxDepth)
	{
		return;
	}

	if (level >= max)
	{
		throw runtime_error { "Depth max reached" };
	}

	++level;

	foo(max);
}
