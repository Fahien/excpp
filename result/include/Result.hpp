#ifndef RESULT_H_
#define RESULT_H_

template<typename T, typename E>
class Result;

template<typename T>
class Res
{
  public:
	Res();
	Res(Res&& o) : val{ std::move(o.val) } {}
	Res(T&& v) : val{ move(v) } {}
	T& operator*() { return val; }

  protected:
	T val;
};


template<typename T>
class Ok : public Res<T>
{};


template<typename E>
class Err : public Res<E>
{};


template<typename T, typename E>
class Result
{
  public:
	Result(Ok<T>&& v) : val{ move(v) }, hasVal{ true }  {}
	Result(Err<E>&& e) : err{ move(e) }, hasVal{ false } {}

	bool IsOk() const { return hasVal; }

	T& GetVal() { return *val; }
	E& GetErr() { return *err; }

  private:
	::Ok<T> val;
	::Err<E> err;

	bool hasVal;
};


#endif // RESULT_H_
