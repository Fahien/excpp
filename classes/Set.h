#ifndef EXCPP_SET_H_
#define EXCPP_SET_H_


#include "Common.h"
#include <memory>
#include <algorithm>

#include "Result.h"


namespace excpp
{


enum class SetError
{
	CapacityOverflow
};


const size_t kDefaultCapacity { 8 };


/// @brief Unordered collection of elements without duplicates
template <typename T>
class Set
{
  public:
	Set( const size_t capacity = kDefaultCapacity );
	Set( std::initializer_list<T>&& elements );
	~Set();

	/// @return The size of the set
	size_t GetSize() const { return m_pSpace - m_pFirst; }

	/// @brief Inserts an element
	Result<T*, SetError> Insert( const T& elem );

	/// @brief Replaces this with the union of this and other
	Result<Set<T>, SetError> Union( const Set<T>& other );

	/// @brief Replaces this with the intersection of this and other
	void Intersect( const Set<T>& other );

	/// @brief Replaces this with the subtraction of this and other
	void Subtract( const Set<T>& other );

  private:
	T* begin() const { return m_pFirst; }
	T* end()   const { return m_pSpace; }
	T* find( const T& elem ) const { return std::find( begin(), end(), elem ); }

	/// @brief Release memory
	void release();

	/// @return The capacity
	size_t getCapacity() const { return m_pLast - m_pFirst; }

	/// @return Whether it is full
	bool isFull() const { return GetSize() == getCapacity(); }

	/// @brief Modify the capacity
	void resize( const size_t capacity );

	std::allocator<T> m_Allocator;

	// First        Space       Last
	// |--------------|            |

	T* m_pFirst;
	T* m_pLast;
	T* m_pSpace;
};


template <typename T>
Set<T>::Set( const size_t capacity )
:	m_Allocator {}
,	m_pFirst { m_Allocator.allocate( capacity ) }
,	m_pLast  { m_pFirst + capacity }
,	m_pSpace { m_pFirst }
{}


template <typename T>
Set<T>::Set( std::initializer_list<T>&& elements )
:	m_Allocator {}
,	m_pFirst { nullptr }
,	m_pLast  { nullptr }
,	m_pSpace { nullptr }
{
	auto capacity = kDefaultCapacity;
	auto size = elements.size();
	while ( size > capacity )
	{
		capacity *= 2;
	}

	m_pFirst = m_Allocator.allocate( capacity );
	m_pLast  = m_pFirst + capacity;
	m_pSpace = m_pFirst + size;

	std::uninitialized_copy( elements.begin(), elements.end(), m_pFirst );
}


template <typename T>
Set<T>::~Set()
{
	release();
}

template <typename T>
void Set<T>::release()
{
	for ( auto pElem = m_pFirst; pElem < m_pSpace; ++pElem )
	{
		std::allocator_traits<std::allocator<T>>::destroy( m_Allocator, pElem );
	}

	m_Allocator.deallocate( m_pFirst, m_pLast - m_pFirst );
}


template <typename T>
void Set<T>::resize( const size_t capacity )
{
	if ( capacity == getCapacity() )
	{
		return;
	}

	// Resize down
	if ( capacity < getCapacity() )
	{
		if ( capacity < GetSize() )
		{
			// Destroy some elements
			for ( auto pElem = m_pFirst + capacity; pElem < m_pSpace; ++pElem )
			{
				std::allocator_traits<std::allocator<T>>::destroy( m_Allocator, pElem );
			}
			m_pSpace = m_pFirst + capacity;
		}
		m_pLast = m_pFirst + capacity;
	}
	else // resize up
	{
		auto pNewFirst = m_Allocator.allocate( capacity );
		// TODO enable with CXX_STANDARD 17
		// std::uninitialized_move( m_pFirst, m_pSpace, pNewFirst );
		std::uninitialized_copy( begin(), end(), pNewFirst );
		release();
		m_pSpace = pNewFirst + GetSize();
		m_pFirst = pNewFirst;
		m_pLast  = pNewFirst + capacity;
	}
}


template <typename T>
Result<T*, SetError> Set<T>::Insert( const T& elem )
{
	auto pElem = find( elem );
	if ( pElem == end() )
	{
		if ( isFull() )
		{
			auto oldCapacity = getCapacity();
			auto newCapacity = oldCapacity * 2;
			// Overflow check
			if (newCapacity < oldCapacity)
			{
				return Err<SetError>{ SetError::CapacityOverflow };
			}
			resize( getCapacity() * 2 );
		}

		std::allocator_traits<std::allocator<T>>::construct( m_Allocator, m_pSpace, elem );
		pElem = m_pSpace;
		m_pSpace += 1;
	}
	return Ok<T*>{ pElem };
}


// This is inefficient: a set could be implemented
// as an ordered list of nodes to improve this method
template <typename T>
Result<Set<T>,SetError> Set<T>::Union( const Set<T>& other )
{
	// Make a temp set without elements already in this set,
	Set<T> temp {};
	for ( auto& elem : other )
	{
		if ( find( elem ) == end() )
		{
			temp.Insert( elem );
		}
	}

	// Make space
	auto tempSize = temp.GetSize();
	auto size = GetSize() + tempSize;
	auto capacity = getCapacity();
	while ( size > capacity )
	{
		auto newCapacity = capacity * 2;
		if ( newCapacity < capacity )
		{
			return Err<SetError>{ SetError::CapacityOverflow };
		}
		capacity = newCapacity;
	}
	resize( capacity );

	// Append temp set to the end of this
	std::uninitialized_copy( temp.begin(), temp.end(), m_pSpace );
	m_pSpace += tempSize;

	return Ok<Set<T>>{ this };
}


}


#endif // EXCPP_SET_H_
