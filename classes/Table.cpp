#include <cstdlib>
#include <cassert>

#include "Table.h"

using namespace std;
using namespace excpp;


int main()
{
	Table<i32,i32> table { { { 0, 1 }, { 2, 3 } } };

	{
		auto result = table.Get( 0 );
		assert( result.IsOk() && "Result is not Ok" );
		assert( ( result.GetVal() == pair<i32,i32>{ 0, 1 } ) && "Result is not { 0, 1 }" );
	}

	{
		auto result = table.Get( 1 );
		assert( !result.IsOk() && "Result should not be Ok" );
		assert( result.GetErr() == TableError::KeyNotFound && "Error is not KeyNotFound" );
	}

	{
		auto option = table.Put( 1, 2 );
		assert( option.IsNone() && "Option is not None" );
	}

	{
		auto option = table.Put( 0, 2 );
		assert( option.IsSome() && "Option is not Some" );
		assert( option.GetVal() == 1 && "Option is not 1" );

		auto result = table.Get( 0 );
		assert( result.IsOk() && "Result is not Ok" );
		assert( ( result.GetVal() == pair<i32, i32>{ 0, 2 } ) && "Result is not { 0, 2 }" );
	}

	{
		auto option = table.Remove( 0 );
		assert( option.IsSome() && "Option is not Some" );
		assert( ( option.GetVal() == pair<i32, i32>{ 0, 2 } ) && "Option is not { 0, 2 }" );
		assert( table.GetSize() == 2 && "Size is not 2" );
	}

	{
		auto option = table.Remove( 0 );
		assert( option.IsNone() && "Option is not None" );
	}


	return EXIT_SUCCESS;
}
