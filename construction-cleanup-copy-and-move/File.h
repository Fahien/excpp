#ifndef EXCPP_FILE_H_
#define EXCPP_FILE_H_

#include <fstream>
#include <string>
#include <vector>
#include <map>


namespace excpp
{


class File
{
  public:
	File( const char* name );

	void Save() { m_Fstream.flush(); }
	void Rewind() { m_Fstream.seekg( m_Begin ); }

	/// @return Whether the stream is at the end
	bool IsEof() const { return m_Fstream.eof(); }

	/// @return The stream size
	std::streamsize GetSize() const { return static_cast<std::streamsize>( m_End - m_Begin ); }

	/// @return An entire line without the newline caracter
	std::string ReadLine();
	std::string ReadLine( const size_t linenum );

	/// @brief Insert a string at line number n
	void WriteLine( std::string line, const size_t linenum );

	/// @brief Deletes line n
	void DeleteLine( const size_t linenum );

  private:

	void gotoLine( const size_t linenum );
	void checkLinenum( const size_t linenum );
	void skipLines( const size_t count );

	/// @brief Reads until end of file
	/// @return A buffer with the characters read
	std::vector<char> readAll();

	std::fstream m_Fstream;
	std::streampos m_Begin;
	std::streampos m_End;
	std::vector<char> m_Buffer;

	size_t m_Linenum { 0 };
	std::streampos m_Pos { 0 };
	std::map<size_t, std::streampos> m_LinePos;
};


}


#endif // EXCPP_FILE_H_
