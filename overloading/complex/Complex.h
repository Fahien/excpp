#ifndef EXCPP_COMPLEX_H_
#define EXCPP_COMPLEX_H_

#include "Imaginary.h"


namespace excpp
{


class Complex
{
  public:
	Complex( int r = 0, const Imaginary& i = 0_i ) : re{ r }, im{ i } {}

	int get_re() const { return re; }
	int get_im() const { return im.get_value(); }

	Complex& operator+=( const Complex& other ) { re += other.re; im += other.im; return *this; }
	Complex& operator-=( const Complex& other ) { re -= other.re; im -= other.im; return *this; }
	Complex& operator*=( const Complex& other );

  private:
	int re;
	Imaginary im;

	friend bool operator==( const Complex& lhs, const Complex& rhs );
	friend bool operator==( const Complex& c, const int i );
};

Complex& Complex::operator*=( const Complex& other )
{
	int tmp_re = re * other.re - get_im() * other.get_im();
	im.set_value( re * other.get_im() + get_im() * other.re );
	re = tmp_re;
	return *this;
}

bool operator==( const Complex& lhs, const Complex& rhs )
{
	return lhs.re == rhs.re && lhs.im == rhs.im;
}

bool operator==( const Complex& c, const int i )
{
	return c.im == 0 && c.re == i;
}


} // namespace excpp

#endif // EXCPP_COMPLEX_H_
