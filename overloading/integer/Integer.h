#ifndef EXCPP_INTEGER_H_
#define EXCPP_INTEGER_H_

namespace excpp
{


class Integer
{
  public:
	/// Constructs an integer
	/// @param v Initial value
	Integer( const int v = 0 ) : value { v } {}

	// User-defined implicit conversion from int
	operator int() { return value; }

	Integer& operator +=( const Integer& rhs ) { value += rhs; return *this; }
	Integer& operator -=( const Integer& rhs ) { value -= rhs; return *this; }
	Integer& operator *=( const Integer& rhs ) { value *= rhs; return *this; }
	Integer& operator /=( const Integer& rhs ) { value /= rhs; return *this; }
	Integer& operator %=( const Integer& rhs ) { value %= rhs; return *this; }

  private:
	int value = 0;

	friend void operator+=( int& lhs, const Integer& rhs );
	friend void operator-=( int& lhs, const Integer& rhs );
	friend void operator*=( int& lhs, const Integer& rhs );
	friend void operator/=( int& lhs, const Integer& rhs );
	friend void operator%=( int& lhs, const Integer& rhs );
};

void operator+=( int& lhs, const Integer& rhs )
{
	lhs += rhs.value;
}

void operator-=( int& lhs, const Integer& rhs )
{
	lhs -= rhs.value;
}

void operator*=( int& lhs, const Integer& rhs )
{
	lhs *= rhs.value;
}

void operator/=( int& lhs, const Integer& rhs )
{
	lhs /= rhs.value;
}

void operator%=( int& lhs, const Integer& rhs )
{
	lhs %= rhs.value;
}

} // namespace excpp

#endif // EXCPP_INTEGER_H_
