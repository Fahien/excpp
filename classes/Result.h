#ifndef EXCPP_RESULT_H_
#define EXCPP_RESULT_H_


namespace excpp
{


template <typename T>
class Res
{
  public:
	Res() : m_pVal { nullptr } {}
	Res( Res<T>&  o ) : m_pVal { o.m_pVal }, m_OwnVal { o.m_OwnVal } { o.m_OwnVal = false; }
	Res( Res<T>&& o ) : Res<T>{ o } {}

	Res( T* v )  : m_pVal{ v } {}
	Res( T& v )  : m_pVal { new T { v } }, m_OwnVal { true } {}
	Res( T&& v ) : m_pVal { new T { std::move( v ) } }, m_OwnVal { true } {}
	T& operator*() { return *m_pVal; }

	~Res() { if ( m_OwnVal ) { delete m_pVal; } }

  protected:
	T* m_pVal = nullptr;
	bool m_OwnVal = false;
};


template <typename T>
class Ok : public Res<T>
{
  public:
	Ok() {}
	Ok( Res<T>&  o ) : Res<T>{ o } {}
	Ok( Res<T>&& o ) : Res<T>{ std::move( o ) } {}
	Ok( T*  v ) : Res<T>{ v } {}
	Ok( T&  v ) : Res<T>{ v } {}
	Ok( T&& v ) : Res<T>{ std::move( v ) } {}
};


template <typename E>
class Err : public Res<E>
{
  public:
	Err() {}
	Err( Res<E>&  o ) : Res<E>{ o } {}
	Err( Res<E>&& o ) : Res<E>{ std::move( o ) } {}
	Err( E* v )  : Res<E>{ v } {}
	Err( E& v )  : Res<E>{ v } {}
	Err( E&& v ) : Res<E>{ std::move( v ) } {}
};


template <typename T, typename E>
class Result
{
  public:
	Result( Ok<T>&& v )  : m_Val { std::move( v ) }, hasVal { true }  {}
	Result( Err<E>&& e ) : m_Err { std::move( e ) }, hasVal { false } {}

	bool IsOk()  const { return hasVal;  }
	bool IsErr() const { return !hasVal; }

	T& GetVal() { return *m_Val; }
	E& GetErr() { return *m_Err; }

  private:
	Ok<T>  m_Val;
	Err<E> m_Err;

	bool hasVal;
};


}


#endif // EXCPP_RESULT_H_
