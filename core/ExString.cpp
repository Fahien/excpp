#include <cstring>
#include <cassert>
#include <algorithm>

#include "ExString.h"


namespace excpp
{

StringView::StringView( String& s, const size_t b, const size_t l )
:	str { s }
,	begin { b }
,	length { l }
{}


void StringView::operator=( const String& other )
{
	// Save string before view
	auto before = str( 0, begin );
	// Save string after view
	auto after = str( begin + length, str.get_length() - ( begin + length ) );
	// Put other in str
	str = before + other + after;
	// Update length
	length = other.get_length();
}


bool StringView::operator==( const char* other ) const
{
	return std::strncmp( get_c_str(), other, std::min( get_length(), std::strlen( other ) ) ) == 0;
}


bool StringView::operator==( const String& other ) const
{
	return std::strncmp( get_c_str(), other.get_c_str(), std::min( get_length(), other.get_length() ) ) == 0;
}


char& StringView::operator[]( const size_t pos )
{
	assert( pos < get_length() );
	return *( get_mut_c_str() + pos );
}


char StringView::operator[]( const size_t pos ) const
{
	assert( pos < get_length() );
	return *( get_c_str() + pos );
}



StringView StringView::operator()( const size_t begin, const size_t length ) const
{
	if ( begin < get_length() && length <= get_length() - begin && length > 0 )
	{
		return StringView{ str, begin + begin, length };
	}

	return { *this };
}


const String& StringView::get_str() const
{
	return str;
}


const char* StringView::get_c_str() const
{
	return str.get_c_str() + begin;
}


char* StringView::get_mut_c_str()
{
	return str.get_mut_c_str() + begin;
}


size_t StringView::get_length() const
{
	return length;
}


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


String& String::operator=( const String& other )
{
	if (m_Capacity < other.m_Capacity )
	{
		m_Capacity = other.m_Capacity;
		std::allocator_traits<std::allocator<uint8_t>>::destroy( m_Allocator, m_Buffer );
		m_Buffer = m_Allocator.allocate( m_Capacity );
	}

	m_Length = other.m_Length;
	std::copy( other.m_Buffer, other.m_Buffer + m_Length, m_Buffer );

	return *this;
}


void String::concat( const char* rhs, const size_t rhsLength )
{
	auto capacity = m_Capacity;

	// Check there is enough space
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
}


String& String::operator+=( const char* rhs )
{
	concat( rhs, std::strlen( rhs ) );
	return *this;
}


String& String::operator+=( const String& rhs )
{
	concat( rhs.get_c_str(), rhs.get_length() );
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


StringView String::operator()( const size_t begin, const size_t length )
{
	if ( begin < get_length() && length <= get_length() - begin && length > 0 )
	{
		return StringView{ *this, begin, length };
	}

	return { *this };
}


String::operator char*() const
{
	auto ret = new char[m_Length + 1];
	ret[m_Length] = 0;
	std::copy_n( get_c_str(), m_Length, ret );
	return ret;
}


String operator"" _str( const char* str, const size_t len )
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


String operator+( const String& lhs, const StringView& rhs )
{
	String ret { lhs, lhs.get_length() + rhs.get_length() };
	ret.concat( rhs.get_c_str(), rhs.get_length() );
	return ret;
}


String operator+( const StringView& lhs, const String& rhs )
{
	String ret { "",  0, lhs.get_length() + rhs.get_length() };
	ret.concat( lhs.get_c_str(), lhs.get_length() );
	return ret += rhs;
}


std::ostream &operator<<(std::ostream& os, const String &str)
{
	std::for_each( str.get_c_str(), str.get_c_str() + str.get_length(), [&os]( const char c ) { os << c; } );
	return os;
}


std::ostream &operator<<(std::ostream& os, const StringView &str)
{
	std::for_each( str.get_c_str(), str.get_c_str() + str.get_length(), [&os]( const char c ) { os << c; } );
	return os;
}


} // namespace excpp
