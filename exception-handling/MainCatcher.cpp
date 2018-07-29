#include <cstdlib>
#include <iostream>

using namespace std;


/// Catch all exceptions thrown from the ...,
/// turn them into error messages, and abort.
int main()
{
	exception_ptr e{ nullptr };

	try
	{
		// Do something
		string{}.at(1);
	}
	catch (...)
	{
		// Capture the current exception
		e = current_exception();
	}

	try
	{
		if (e)
		{
			// Rethrow as exception object
			rethrow_exception(e);
		}
	}
	catch (const exception& e)
	{
		// Catch and log to standard error
		cerr << e.what() << endl;
	}

	return EXIT_SUCCESS;
}
