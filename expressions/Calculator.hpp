#include <string>
#include <stdexcept>
#include <map>


namespace clc
{

class CalculatorException : public std::runtime_error
{
public:
	CalculatorException(const char* msg)
	:	std::runtime_error{ msg }
	{}
};


enum class Kind : char
{
	name,
	number,
	end,
	plus   = '+',
	minus  = '-',
	mul    = '*',
	div    = '/',
	print  = ';',
	assign = '=',
	lp     = '(',
	rp     = ')'
};


struct Token
{
	Kind        kind;
	std::string sValue;
	double      nValue;
};


class TokenStream
{
public:
	TokenStream(std::istream& is) : mIs{ is } {}

	/// Returns the most recently read token
	const Token& GetCurrent() { return mCurrent; }

	/// Reads and returns the next token
	Token GetNext();

	// ...
private:
	std::istream& mIs;
	Token mCurrent{ Kind::end };
};


class Calculator
{
public:
	Calculator();
	Calculator(TokenStream& tokenStream);

	double Prim(bool get);

	/// Handles multiplication and division
	double Term(bool get);

	/// Handles addition and subtraction
	double Expr(bool get);

	/// Handles the actual calculation
	void Calculate();

private:
	TokenStream& mTokenStream;
	std::map<std::string, double> mTable;
};


}
