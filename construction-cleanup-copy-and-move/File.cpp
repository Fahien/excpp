#include "File.h"
#include <cmath>

using namespace excpp;


File::File( const char* name )
:	m_Fstream { name, std::ios::in | std::ios::out }
,	m_Begin { m_Fstream.tellg() }
,	m_Pos { m_Begin }
,	m_LinePos{ std::pair<size_t,std::streampos>{ m_Linenum, m_Pos } }
{
	m_Fstream.seekg( 0, std::ios::end );
	m_End = m_Fstream.tellg();
	Rewind();

	m_Buffer.resize( 16 );
}


void File::checkLinenum( const size_t linenum )
{
	// Fill linepos map if empty
	if ( m_LinePos.count( m_Linenum ) == 0 )
	{
		m_LinePos[m_Linenum] = m_Fstream.tellg();
	}
}


void File::skipLines( const size_t count )
{
	for ( size_t i { 0 }; i < count; ++i )
	{
		m_Fstream.ignore( GetSize(), '\n' );
		++m_Linenum;
		checkLinenum( m_Linenum );
	}

	// Update position
	m_Pos = m_Fstream.tellg();
}


void File::gotoLine( const size_t linenum )
{
	auto itPair = m_LinePos.lower_bound( linenum );
	if ( itPair != m_LinePos.end() )
	{
		m_Linenum = itPair->first;
		m_Pos = itPair->second;
		m_Fstream.seekg( m_Pos );
	}
	else
	{
		skipLines( linenum - m_Linenum );
	}
}


std::string File::ReadLine( const size_t linenum )
{
	gotoLine( linenum );
	return ReadLine();
}


std::string File::ReadLine()
{
	std::string line {};

	while ( true )
	{
		if ( m_Fstream.peek() == EOF )
		{
			m_Fstream.get();
			break;
		}

		m_Fstream.getline( &m_Buffer[0], m_Buffer.capacity() );
		if ( !m_Fstream.good() )
		{
			line += &m_Buffer[0];
			if ( m_Fstream.eof() )
			{
				break;
			}
			m_Buffer.resize( m_Buffer.capacity() * 2 );
			m_Fstream.clear();
		}
		else
		{
			line += &m_Buffer[0];
			break;
		}
	}

	++m_Linenum;
	checkLinenum( m_Linenum );

	return line;
}

std::vector<char> File::readAll()
{
	// Get rest of file
	std::vector<char> buffer;
	auto size = m_End - m_Pos;
	buffer.resize( size );
	m_Fstream.read( buffer.data(), size );
	m_Fstream.clear();
	buffer[size - 1] = EOF;
	return buffer;
}


void File::WriteLine( std::string line, const size_t linenum )
{
	gotoLine( linenum );
	auto buffer = readAll();
	// Overwrite line
	m_Fstream.seekp( m_Pos );
	m_Fstream << line << std::endl;
	// Write rest of the file
	m_Fstream << buffer.data();
}


void File::DeleteLine( const size_t linenum )
{
	gotoLine( linenum );
	auto linePos = m_Pos;
	skipLines( 1 );
	auto buffer = readAll();
	// Overwrite line to delete
	m_Fstream.seekp( linePos );
	m_Fstream.write( buffer.data(), buffer.size() );
	// TODO set end of file
}
