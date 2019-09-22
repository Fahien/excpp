#include <cstring>

#include "ExString.h"


namespace excpp
{
String::String( const char* str )
: String( str, std::strlen( str ) )
{}


String::String( const char* str, const size_t len, const size_t minCapacity )
:	m_Capacity { kDefaultCapacity }
,	m_Length { len }
{
	init( str, minCapacity );
}


void String::init( const char* str, size_t minCapacity )
{
	if ( minCapacity < m_Length + 1 )
	{
		minCapacity = m_Length + 1;
	}

	while ( m_Capacity < minCapacity )
	{
		m_Capacity *= 2;
	}

	m_Buffer = m_Allocator.allocate( m_Capacity );
	std::uninitialized_copy_n( str, m_Length + 1, m_Buffer );
}


String::String( const String& other, size_t minCapacity )
:	m_Capacity { other.m_Capacity }
,	m_Length { other.m_Length }
{
	init( other.get_c_str(), minCapacity );
}


String& String::operator+=( const char* rhs )
{
	auto capacity = m_Capacity;

	// Check there is enough space
	auto rhsLength = std::strlen( rhs );
	while ( capacity < m_Length + rhsLength )
	{
		capacity *= 2;
	}

	// If there is no space create a new buffer
	if ( capacity != m_Capacity )
	{
		auto tempBuffer = m_Allocator.allocate( capacity );
		std::uninitialized_copy_n( m_Buffer, m_Length, tempBuffer );
		std::allocator_traits<std::allocator<uint8_t>>::destroy( m_Allocator, m_Buffer );
		m_Buffer = tempBuffer;
	}

	// Append
	std::uninitialized_copy_n( rhs, rhsLength + 1, m_Buffer + m_Length );
	m_Length += rhsLength;

	return *this;
}


bool String::operator==( const char* other ) const
{
	return std::strncmp( get_c_str(), other, std::min( get_length(), std::strlen( other ) ) ) == 0;
}


bool String::operator==( const String& other ) const
{
	return *this == other.get_c_str();
}


String String::operator()( size_t begin, size_t length )
{
	if ( begin < get_length() && length <= get_length() - begin && length > 0 )
	{
		auto substr_begin = get_c_str() + begin;
		// TODO: Fix this, length is capacity
		return String{ substr_begin, length };
	}

	return {};
}


String operator"" _str( const char* str, size_t len )
{
	return String{ str, len };
}


String operator+( const String& lhs, const char* rhs )
{
	String ret { lhs, lhs.get_length() + std::strlen( rhs ) };
	return ret += rhs;
}


String operator+( const String& lhs, const String& rhs )
{
	String ret { lhs, lhs.get_length() + rhs.get_length() };
	return ret += rhs.get_c_str();
}


std::ostream &operator<<(std::ostream& os, String &str)
{
	std::for_each_n( str.get_c_str(), str.get_length(), [&os]( const char c ) { os << c; } );
	return os;
}


} // namespace excpp
