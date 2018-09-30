#include "Editor.h"
#include "File.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>

using namespace excpp;


int main( void )
{
	File file { "construction-cleanup-copy-and-move/test.txt" };

	assert( file.ReadLine() == "line1" && "Line one is not line 1" );

	file.Rewind();
	assert( file.ReadLine( 1 ) == "line2" && "Line two is not line 2" );
	assert( file.ReadLine( 0 ) == "line1" && "Line one is not line 1" );

	file.DeleteLine( 2 );
	file.Save();

	return EXIT_SUCCESS;
}
