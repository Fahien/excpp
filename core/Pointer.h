#ifndef EXCPP_POINTER_H_
#define EXCPP_POINTER_H_

#include <cstddef>
#include <stdexcept>


namespace excpp
{


template <typename T>
class Pointer
{
  public:
	template <size_t n>
	Pointer( T(&a)[n] );

	Pointer( T* a = nullptr, size_t n = 0 );

	void operator=( const Pointer<T>& other );

	Pointer<T>& operator++();
	Pointer<T>& operator--();
	Pointer<T> operator++(int);
	Pointer<T> operator--(int);

	void check();

	T& operator*();
	T* operator->();

  private:
	T* pointer;
	T* array;
	size_t size;
};


template <typename T>
template <size_t n>
Pointer<T>::Pointer( T(&a)[n] ) : pointer { a }, array { a }, size { n } {}


template <typename T>
Pointer<T>::Pointer( T* a, size_t n ) : pointer { a }, array { a }, size { n } {}


template <typename T>
void Pointer<T>::operator=( const Pointer<T>& other )
{
	pointer = other.pointer;
	array = other.array;
	size = other.size;
}


template <typename T>
Pointer<T>& Pointer<T>::operator++()
{
	++pointer;
	return *this;
}


template <typename T>
Pointer<T>& Pointer<T>::operator--()
{
	--pointer;
	return *this;
}


template <typename T>
Pointer<T> Pointer<T>::operator++(int)
{
	Pointer<T> tmp = *this;
	++pointer;
	return tmp;
}


template <typename T>
Pointer<T> Pointer<T>::operator--(int)
{
	Pointer<T> tmp = *this;
	++pointer;
	return tmp;
}


template <typename T>
void Pointer<T>::check()
{
	if ( pointer >= array + size )
	{
		throw std::runtime_error{ "Out of bounds" };
	}
}


template <typename T>
T& Pointer<T>::operator*()
{
	check();
	return *pointer;
}


template <typename T>
T* Pointer<T>::operator->()
{
	check();
	return pointer;
}


} // namespace excpp

#endif // EXCPP_POINTER_H_
