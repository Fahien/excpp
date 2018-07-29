#include "Result.hpp"
#include <string>
#include <iostream>

using namespace std;


Result<int, string> calculate(bool ok)
{
	if (ok)
	{
		return Ok<int>(42);
	}
	else
	{
		return Err<string>( "Error" );
	}
}


int main()
{
	auto result = calculate(true);

	if (result.IsOk())
	{
		cout << result.Get() << endl;
	}
	else
	{
		cout << result.Get() << endl;
	}

	{
		auto result = calculate(false);

		if (result.IsOk())
		{
			cout << result.GetVal() << endl;
		}
		else
		{
			cout << result.GetErr() << endl;
		}
	}

	return 0;
}