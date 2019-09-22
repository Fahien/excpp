#ifndef EXCPP_STRING_H_
#define EXCPP_STRING_H_

#include <memory>
#include <ostream>


namespace excpp
{


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
	bool operator==( const char* other ) const;
	bool operator==( const String& other ) const;

	/// Substring operator
	String operator()( size_t begin, size_t length );

  private:
	void init( const char* str, size_t minCapacity );

	static constexpr uint16_t kDefaultCapacity { 16 };

	/// Capacity of the Buffer
	size_t m_Capacity;

	/// Length of the string
	size_t m_Length;

	std::allocator<uint8_t> m_Allocator;

	uint8_t* m_Buffer { nullptr };
};


String operator+( const String& lhs, const char* rhs );
String operator+( const String& lhs, const String& rhs );

String operator"" _str( const char* str, size_t len );

std::ostream &operator<<(std::ostream& os, String &str);

}


#endif // EXCPP_STRING_H_
