#ifndef EXCPP_OPTION_H_
#define EXCPP_OPTION_H_

#include "Result.h"


namespace excpp
{


template <typename T>
class Some : public Res<T>
{
  public:
	Some() {}
	Some( Res<T>&  o ) : Res<T>{ o } {}
	Some( Res<T>&& o ) : Res<T>{ std::move( o ) } {}
	Some( T*  v ) : Res<T>{ v } {}
	Some( T&  v ) : Res<T>{ v } {}
	Some( T&& v ) : Res<T>{ std::move( v ) } {}
};


class None {};


template <typename T>
class Option
{
  public:
	Option( Some<T>&& v ) : m_Val { std::move( v ) }, m_HasVal { true } {}
	Option( None&& n ) : m_Val {}, m_HasVal { false } {}

	bool IsSome() const { return m_HasVal;  }
	bool IsNone() const { return !m_HasVal; }

	T& GetVal() { return *m_Val; }

  private:
	Some<T> m_Val;

	bool m_HasVal;
};


}


#endif // EXCPP_OPTION_H_
