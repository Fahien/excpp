#ifndef EXCPP_STRING_H_
#define EXCPP_STRING_H_

#include <memory>
#include <ostream>


namespace excpp
{


class String;


class StringView
{
  public:
	StringView( String& str, size_t begin = 0, size_t length = 0 );

	void operator=( const String& other );
	bool operator==( const char* other ) const;
	bool operator==( const String& other ) const;

	const String& get_str() const;
	const char* get_c_str() const;

	size_t get_length() const;

  private:
	String& str;

	size_t begin = 0;

	size_t length = 0;
};


class String
{
  public:
	String( const char* str = "" );
	String( const char* str, size_t length, size_t minCapacity = kDefaultCapacity );
	String( const String& other, size_t minCapacity = kDefaultCapacity );

	size_t get_length() const { return m_Length; }
	const char* get_c_str() const { return reinterpret_cast<const char*>( m_Buffer ); }
	char* get_mut_c_str() { return reinterpret_cast<char*>( m_Buffer ); }

	String& operator+=( const char* rhs );
	String& operator+=( const String& rhs );
	bool operator==( const char* other ) const;
	bool operator==( const String& other ) const;

	/// Substring operator
	StringView operator()( size_t begin, size_t length );

  private:
	void init( const char* str, size_t minCapacity );
	
	void concat( const char* str, const size_t length );

	static constexpr uint16_t kDefaultCapacity { 16 };

	/// Capacity of the Buffer
	size_t m_Capacity;

	/// Length of the string
	size_t m_Length;

	std::allocator<uint8_t> m_Allocator;

	uint8_t* m_Buffer { nullptr };

	friend String operator+( const String& lhs, const StringView& rhs );
	friend String operator+( const StringView& lhs, const String& rhs );
};


String operator+( const String& lhs, const char* rhs );
String operator+( const String& lhs, const String& rhs );
String operator+( const String& lhs, const StringView& rhs );
String operator+( const StringView& lhs, const String& rhs );

String operator"" _str( const char* str, size_t len );

std::ostream &operator<<( std::ostream& os, const String &str );
std::ostream &operator<<( std::ostream& os, const StringView &str );

}


#endif // EXCPP_STRING_H_
