#ifndef CPP_INTEGER_H_
#define CPP_INTEGER_H_

#include <iostream>
#include <stdexcept>
#include <string>


class OverflowException : public std::overflow_error
{
  public:
	OverflowException() : std::overflow_error{ "Overflow" } {};
};


class UnderflowException : public std::underflow_error
{
  public:
	UnderflowException() : std::underflow_error{ "Underflow" } {};
};


class DivideByZeroException : public std::runtime_error
{
  public:
	DivideByZeroException() : std::runtime_error{ "Divide by zero" } {};
};


/// Integer class that acts exactly like the built-in type int, except that
/// it throws exceptions rather than overflowing or underflowing
class Integer
{
  public:
	Integer(int value);
	Integer();

	int GetValue() { return mValue; }

	Integer& operator++();
	Integer& operator--();
	Integer  operator++(int);
	Integer  operator--(int);

	Integer& operator+=(const int& rhs);
	Integer& operator-=(const int& rhs);
	Integer& operator*=(const int& rhs);
	Integer& operator/=(const int& rhs);
	Integer& operator+=(const Integer& rhs);
	Integer& operator-=(const Integer& rhs);
	Integer& operator*=(const Integer& rhs);
	Integer& operator/=(const Integer& rhs);

	friend bool operator==(const Integer& lhs, const Integer& rhs) { return lhs.mValue == rhs.mValue; }
	friend bool operator!=(const Integer& lhs, const Integer& rhs) { return !(lhs == rhs); }

	friend bool operator< (const Integer& lhs, const Integer& rhs) { return lhs.mValue < rhs.mValue; }
	friend bool operator> (const Integer& lhs, const Integer& rhs) { return rhs < lhs; }
	friend bool operator<=(const Integer& lhs, const Integer& rhs) { return !(lhs > rhs); }
	friend bool operator>=(const Integer& lhs, const Integer& rhs) { return !(lhs < rhs); }

  private:
	int checkedIncrement(const int& rhs);
	int checkedMultiplication(const int& rhs);

	friend inline Integer operator+(Integer lhs, const Integer& rhs);
	friend inline Integer operator-(Integer lhs, const Integer& rhs);
	friend inline Integer operator*(Integer lhs, const Integer& rhs);
	friend inline Integer operator/(Integer lhs, const Integer& rhs);
	friend inline Integer operator+(Integer lhs, const int& rhs);
	friend inline Integer operator-(Integer lhs, const int& rhs);
	friend inline Integer operator*(Integer lhs, const int& rhs);
	friend inline Integer operator/(Integer lhs, const int& rhs);

	/// Stream insertion
	friend std::ostream& operator<<(std::ostream& os, const Integer& i);

	/// Stream extraction
	friend std::istream& operator>>(std::istream& is, Integer& i);

	int mValue;
};


Integer::Integer(int value)
:	mValue  { value }
{}


Integer::Integer()
:	Integer{ 0 }
{}


int Integer::checkedIncrement(const int& rhs)
{
	int tmp{ mValue + rhs };

	if (rhs > 0 && tmp < mValue)
	{
		throw OverflowException{};
	}
	else if (rhs < 0 && tmp > mValue)
	{
		throw UnderflowException{};
	}

	return tmp;
}


int Integer::checkedMultiplication(const int& rhs)
{
	int tmp{ mValue * rhs };

	if (mValue > 0)
	{
		if (rhs > 0 && tmp < mValue)
		{
			throw OverflowException{};
		}
		else if (rhs < 0 && tmp > mValue)
		{
			throw UnderflowException{};
		}
	}
	else if (mValue < 0)
	{
		if (rhs > 0 && tmp > mValue)
		{
			throw UnderflowException{};
		}
		else if (rhs < 0 && tmp < mValue)
		{
			throw OverflowException{};
		}
	}

	return tmp;
}


Integer& Integer::operator++()
{
	mValue = checkedIncrement(1);
	return *this;
}


Integer& Integer::operator--()
{
	mValue = checkedIncrement(-1);
	return *this;
}


Integer Integer::operator++(int)
{
	Integer tmp{ *this }; // copy
	operator++();         // pre-increment
	return tmp;           // return old value
}


Integer Integer::operator--(int)
{
	Integer tmp{ *this };
	operator--();
	return tmp;
}


/// Compound assignment
Integer& Integer::operator+=(const int& rhs)
{
	mValue = checkedIncrement(rhs);
	return *this;
}


Integer& Integer::operator-=(const int& rhs)
{
	return operator+=(-rhs);
}


Integer& Integer::operator*=(const int& rhs)
{
	mValue = checkedMultiplication(rhs);
	return *this;
}


Integer& Integer::operator/=(const int& rhs)
{
	if (rhs == 0)
	{
		throw DivideByZeroException{};
	}
	mValue /= rhs;
	return *this;
}


Integer& Integer::operator+=(const Integer& rhs)
{
	*this += rhs.mValue;
	return *this;
}


Integer& Integer::operator-=(const Integer& rhs)
{
	*this -= rhs.mValue;
	return *this;
}


Integer& Integer::operator*=(const Integer& rhs)
{
	*this *= rhs.mValue;
	return *this;
}


Integer& Integer::operator/=(const Integer& rhs)
{
	*this /= rhs.mValue;
	return *this;
}


Integer operator+(Integer lhs, const int& rhs)
{
	lhs += rhs; // reuse compound assignment
	return lhs; // return the result by value (uses move constructor)
}


Integer operator-(Integer lhs, const int& rhs)
{
	lhs -= rhs;
	return lhs;
}


Integer operator*(Integer lhs, const int& rhs)
{
	lhs *= rhs;
	return lhs;
}


Integer operator/(Integer lhs, const int& rhs)
{
	lhs /= rhs;
	return lhs;
}


// Passing lhs by value helps optimize chained a+b+c
Integer operator+(Integer lhs, const Integer& rhs)
{
	lhs += rhs;
	return lhs;
}


Integer operator-(Integer lhs, const Integer& rhs)
{
	lhs -= rhs;
	return lhs;
}


Integer operator*(Integer lhs, const Integer& rhs)
{
	lhs *= rhs;
	return lhs;
}


Integer operator/(Integer lhs, const Integer& rhs)
{
	lhs /= rhs;
	return lhs;
}


std::ostream& operator<<(std::ostream& os, const Integer& i)
{
	return os << i.mValue;
}


std::istream& operator>>(std::istream& is, Integer& i)
{
	return is >> i.mValue;
}


#endif // CPP_INTEGER_H_
