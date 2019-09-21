#ifndef EXCPP_STRING_H_
#define EXCPP_STRING_H_

#include <memory>


namespace excpp
{


class String
{
  public:
	String( const char* str = "", size_t minCapacity = kDefaultCapacity );
	String( const String& other, size_t minCapacity = kDefaultCapacity );

	size_t GetLength() const { return m_Length; }
	const char* GetCStr() const { return reinterpret_cast<const char*>( m_Buffer ); }
	char* get_mut_c_str() { return reinterpret_cast<char*>( m_Buffer ); }

	String& operator+=( const char* rhs );
	bool operator==( const char* other ) const;
	bool operator==( const String& other ) const;

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


}


#endif // EXCPP_STRING_H_
