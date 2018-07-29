#ifndef CPP_VECTOR_H_
#define CPP_VECTOR_H_

#include <iterator>
#include <memory>
#include <stdexcept>


namespace excpp
{


template <typename T>
class Vector
{
  public:
	/// Constructs a vector
	Vector();

	/// Constructs a vector
	/// @param[in] capacity Initial capacity
	Vector( size_t capacity );

	/// Contructs a copy of a vector
	/// @param[in] v Vector to copy
	Vector( const Vector& v );

	/// Copy a vector
	/// @param[in] other Vector to copy
	Vector<T>& operator=( const Vector<T>& v );

	/// Destructs a vector
	~Vector();

	/// Push a copy of the element at the end
	/// @param[in] t Element
	void Push( T& t );

	/// Push the element at the end
	/// @param[in] t Element
	void Push( T&& t );

	/// @return the size
	size_t Size();

	/// @return whether it is empty
	bool IsEmpty();

	/// @param[in] p Position
	/// @return the element at position p
	T& At( const size_t p );

	/// @return the element at the front
	T& Front();

	/// @return the element at the back
	T& Back();

  private:
	T* begin() const;
	T* end() const;
	void resize( size_t capacity );

	const size_t kDefaultCapacity{ 16 };

	size_t mCapacity{ kDefaultCapacity };

	std::unique_ptr<T[]> mBase{ new T[kDefaultCapacity] };

	size_t mSize{ 0 };
};



template <typename T>
Vector<T>::Vector() {}


template <typename T>
Vector<T>::Vector( size_t capacity )
:	mCapacity{ capacity        }
,	mBase    { new T[capacity] }
{}


template <typename T>
Vector<T>::Vector( const Vector& v )
:	mCapacity{ v.mCapacity        }
,	mBase    { new T[v.mCapacity] }
,	mSize    { v.mSize            }
{
	std::uninitialized_copy( v.begin(), v.end(), begin() );
}


template <typename T>
Vector<T>& Vector<T>::operator=( const Vector<T>& v )
{
	if ( this != &v )
	{
		if ( mCapacity != v.mCapacity )
		{
			mCapacity = v.mCapacity;
			mBase.reset( new T[mCapacity] );
			std::uninitialized_copy( v.begin(), v.end(), begin() );
		}
		else // recycle current base
		{
			std::copy( v.begin(), v.end(), begin() );
		}
		mSize = v.mSize;
	}
	
	return *this;
}


template <typename T>
Vector<T>::~Vector()
{}


template <typename T>
T* Vector<T>::begin() const
{
	return mBase.get();
}


template <typename T>
T* Vector<T>::end() const
{
	return begin() + mSize;
}


template <typename T>
void Vector<T>::resize( size_t capacity )
{
	if ( capacity > mCapacity )
	{
		mCapacity = capacity;
		// Make a new base
		T* newBase = new T[mCapacity];
		if (!newBase)
		{
			throw length_error{ "Error while resizing: No more space left" };
		}
		std::uninitialized_copy( begin(), end(), newBase );
		// Substitute
		mBase.reset( newBase );
	}
}


template <typename T>
void Vector<T>::Push( T& t )
{
	if ( mSize == mCapacity )
	{
		resize( mCapacity * 2 );
	}
	mBase[mSize++] = t;
}


template <typename T>
void Vector<T>::Push( T&& t )
{
	if ( mSize == mCapacity )
	{
		resize( mCapacity * 2 );
	}
	mBase[mSize++] = t;
}



template <typename T>
size_t Vector<T>::Size()
{
	return mSize;
}


template <typename T>
bool Vector<T>::IsEmpty()
{
	return mSize == 0;
}


template <typename T>
T& Vector<T>::At( const size_t p )
{
	if ( p > mSize )
	{
		throw out_of_range{ "Position out of vector range" };
	}

	return mBase[p];
}


template <typename T>
T& Vector<T>::Front()
{
	if ( mSize == 0 )
	{
		throw out_of_range{ "Vector is empty" };
	}
	return mBase[0];
}


template <typename T>
T& Vector<T>::Back()
{
	if ( mSize == 0 )
	{
		throw out_of_range{ "Vector is empty" };
	}
	return mBase[mSize - 1];
}


}


#endif // CPP_VECTOR_H_
