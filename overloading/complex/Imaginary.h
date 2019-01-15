#ifndef EXCPP_IMAGINARY_H_
#define EXCPP_IMAGINARY_H_

namespace excpp
{


class Imaginary
{
  public:
	/// Constructs an imaginary number
	/// @param v Initial value
	explicit Imaginary( const int v = 0 ) : value { v } {}

	Imaginary& operator +=( const Imaginary& rhs ) { value += rhs.value; return *this; }
	Imaginary& operator -=( const Imaginary& rhs ) { value -= rhs.value; return *this; }

	/// Unary prefix operator
	Imaginary operator+() const { return Imaginary{ value }; }
	Imaginary operator-() const { return Imaginary{ -value }; }

	int get_value() const { return value; }
	void set_value( const int v ) { value = v; }

  private:
	int value = 0;

	friend Imaginary operator+( const Imaginary& lhs, const Imaginary& rhs );
	friend Imaginary operator-( const Imaginary& lhs, const Imaginary& rhs );
	friend int operator*( const Imaginary& lhs, const Imaginary& rhs );
	friend int operator/( const Imaginary& lhs, const Imaginary& rhs );

	friend bool operator==( const Imaginary& lhs, const Imaginary& rhs );
	friend bool operator==( const Imaginary& lhs, const int rhs );
	friend bool operator==( const int lhs, const Imaginary& rhs );
};

inline Imaginary operator""_i( unsigned long long v )
{
	return Imaginary{ static_cast<int>(v) };
}

Imaginary operator+( const Imaginary& lhs, const Imaginary& rhs )
{
	return Imaginary{ lhs.value + rhs.value };
}

Imaginary operator-( const Imaginary& lhs, const Imaginary& rhs )
{
	return Imaginary{ lhs.value - rhs.value };
}

int operator*( const Imaginary& lhs, const Imaginary& rhs )
{
	return -lhs.value * rhs.value;
}

int operator/( const Imaginary& lhs, const Imaginary& rhs )
{
	return lhs.value / rhs.value;
}

bool operator==( const Imaginary& lhs, const Imaginary& rhs )
{
	return lhs.value == rhs.value;
}

bool operator==( const Imaginary& lhs, const int rhs )
{
	return rhs == 0 && lhs.value == 0;
}

bool operator==( const int lhs, const Imaginary& rhs )
{
	return lhs == 0 && rhs.value == 0;
}


} // namespace excpp

#endif // EXCPP_IMAGINARY_H_
