// #include <Includes.h> 
// Includes.h is not found with <angled> include
#include "Includes.h"


using namespace std;

// This is where std headers are kept on my system
// It changes according to the MSVC version
const string g_StdIncludePath { "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.14.26428/include" };


/// @return The parent path
string parent( const string& path )
{
	auto slashIndex = path.find_last_of( '/' );
	if ( slashIndex == string::npos ) // not found
	{
		slashIndex = path.find_last_of( '\\' );
	}

	return path.substr( 0, slashIndex );
}


/// Analyses an include file
void analyse( const string& rootPath, const string& filePath, const string& indent = "" )
{
	auto file = ifstream{ rootPath + '/' + filePath };

	if ( !file.is_open() )
	{
		file = ifstream{ g_StdIncludePath + '/' + filePath };

		if ( !file.is_open() )
		{
			cerr << "Cannot open " << filePath << endl;
			return;
		}
	}

	const streamsize bufferSize { 256 };

	char buffer[bufferSize];
	string readWord;
	string includeWord { "#include" };

	while ( !file.eof() )
	{
		file.getline( buffer, bufferSize );
		stringstream bufferStream { buffer };
		bufferStream >> readWord;

		// Starts with "#include"
		if ( readWord.substr( 0, includeWord.length() ) == includeWord )
		{
			// Check if include file name is concatenated
			auto lastChar = readWord.at( readWord.length() - 1 );
			if ( lastChar == '"' || lastChar == '>' )
			{
				readWord = readWord.substr( includeWord.length() + 1, readWord.length() - includeWord.length() - 2 );
			}
			else // Read next word
			{
				bufferStream >> readWord;
				readWord = readWord.substr( 1, readWord.length() - 2 );
			}

			cout << indent << readWord << endl;

			// Recursively call itself
			analyse( rootPath, readWord, indent + "- " );
		}
	}
}


/// Program that reads a source file and writes out the names of files #included.
/// It indents file names to show files #included by included files.
int main( int argc, const char** argv )
{
	if ( argc != 3 )
	{
		cerr << "Usage: " << argv[0] << " <root path> <file path>" << endl;
		return EXIT_FAILURE;
	}

	auto rootPath = string{ argv[1] };
	auto filePath = string{ argv[2] };

	analyse( rootPath, filePath );

	return EXIT_SUCCESS;
}