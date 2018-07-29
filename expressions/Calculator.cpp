#include <cstdlib>
#include <iostream>
#include <sstream>
#include <memory>
#include "Calculator.hpp"


clc::Token clc::TokenStream::GetNext()
{
	char ch = 0;
	auto& f = std::use_facet<std::ctype<char>>(std::locale());

	// Skip whitespaces except '\n'
	do
	{
		if (!mIs.get(ch))
		{
			return mCurrent = { clc::Kind::end };
		}
	}
	while (ch != '\n' && f.is(std::ctype_base::space, ch));

	switch (ch)
	{
		case 0:
		{
			return mCurrent = { clc::Kind::end };
		}
		case ';':
		case '\n':
		{
			return mCurrent = { clc::Kind::print };
		}
		case '*':
		case '/':
		case '+':
		case '-':
		case '(':
		case ')':
		case '=':
		{
			return mCurrent = { static_cast<clc::Kind>(ch) };
		}
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '.':
		{
			// Put the first digit (or .) back into the stream
			mIs.putback(ch);
			// Read the number into the current token
			mIs >> mCurrent.nValue;
			mCurrent.kind = clc::Kind::number;
			return mCurrent;
		}
		default:
		{
			if (f.is(std::ctype_base::alpha, ch))
			{
				mCurrent.sValue = ch;
				while (mIs.get(ch) && f.is(std::ctype_base::alnum, ch))
				{
					// Append ch to string value
					mCurrent.sValue += ch;
				}
				mIs.putback(ch);
				mCurrent.kind = clc::Kind::name;
				return mCurrent;
			}
			throw CalculatorException{ "Bad token" };
		}
	}
}


double clc::Calculator::Prim(bool get)
{
	if (get)
	{
		mTokenStream.GetNext();
	}

	bool readToken{ true };

	switch (mTokenStream.GetCurrent().kind)
	{
		case clc::Kind::number:
		{
			double value{ mTokenStream.GetCurrent().nValue };
			mTokenStream.GetNext();
			return value;
		}
		case clc::Kind::name:
		{
			double& value{ mTable[mTokenStream.GetCurrent().sValue] };
			if (mTokenStream.GetNext().kind == clc::Kind::assign)
			{
				value = Expr(readToken);
			}
			return value;
		}
		case clc::Kind::minus:
		{
			return -Prim(readToken);
		}
		case clc::Kind::lp:
		{
			double e{ Expr(readToken) };
			if (mTokenStream.GetCurrent().kind != clc::Kind::rp)
			{
				throw CalculatorException(") expected");
			}
			mTokenStream.GetNext(); // eat )
			return e;
		}
		default:
		{
			throw CalculatorException("Primary expected");
		}
	}
}


double clc::Calculator::Term(bool get)
{
	double left = Prim(get);

	bool readToken{ true };

	while (true)
	{
		switch (mTokenStream.GetCurrent().kind)
		{
			case clc::Kind::mul:
			{
				left *= Prim(readToken);
				break;
			}
			case clc::Kind::div:
			{
				if (auto d = Prim(readToken))
				{
					left /= d;
					break;
				}
				throw clc::CalculatorException("Divide by 0");
			}
			default:
			{
				return left;
			}
		}
	}
}


/// Handles addition and subtraction
double clc::Calculator::Expr(bool get)
{
	double left{ Term(get) };

	bool readToken{ true };

	while (true)
	{
		switch(mTokenStream.GetCurrent().kind)
		{
			case clc::Kind::plus:
			{
				left += Term(readToken);
				break;
			}
			case clc::Kind::minus:
			{
				left -= Term(readToken);
				break;
			}
			default:
			{
				return left;
			}
		}
	}
}


void clc::Calculator::Calculate()
{
	while (true)
	{
		mTokenStream.GetNext();
		auto& token = mTokenStream.GetCurrent();
		if (token.kind == clc::Kind::end)
		{
			break;
		}
		if (token.kind == clc::Kind::print)
		{
			continue;
		}
		// Read the expression and write out the answer
		bool readToken{ false };
		std::cout << Expr(readToken) << std::endl;
	}
}


clc::Calculator::Calculator()
:	clc::Calculator{ clc::TokenStream{ std::cin } }
{}

clc::Calculator::Calculator(clc::TokenStream&& tokenStream)
:	mTokenStream{ tokenStream }
{
	mTable["pi"] = 3.1415926535897932385;
	mTable["e"]  = 2.7182818284590452354;
}

clc::Calculator::Calculator(clc::TokenStream& tokenStream)
:	mTokenStream{ tokenStream }
{
	mTable["pi"] = 3.1415926535897932385;
	mTable["e"]  = 2.7182818284590452354;
}


int main(int argc, char** argv)
{
	std::unique_ptr<clc::TokenStream> pTokenStream;
	std::unique_ptr<std::istringstream> pStream;

	switch (argc)
	{
		case 1:
		{
			pTokenStream.reset(new clc::TokenStream{ std::cin });
			break;
		}
		case 2:
		{
			pStream.reset(new std::istringstream{ argv[1] });
			pTokenStream.reset(new clc::TokenStream{ *pStream });
			break;
		}
		case 3:
		{
			std::cerr << "Too many arguments\n";
			return EXIT_FAILURE;
		}
	}

	clc::Calculator calc{ *pTokenStream };
	try
	{
		calc.Calculate();
	}
	catch (const clc::CalculatorException& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return EXIT_SUCCESS;
}