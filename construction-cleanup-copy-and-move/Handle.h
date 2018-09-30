#ifndef EXCPP_HANDLE_H_
#define EXCPP_HANDLE_H_


#include <algorithm>


namespace excpp
{


template <typename T>
class Handle
{
  public:
	Handle( T* pObject ) : m_pObject { pObject } {}
	~Handle() { if ( m_pObject != nullptr ) delete m_pObject; }

	Handle( Handle<T>&& other );
	Handle<T>& operator=( Handle<T>&& other );

	T& operator*() { return *m_pObject; }

  private:
	T* m_pObject { nullptr };
};


template <typename T>
Handle<T>::Handle( Handle<T>&& other )
:	m_pObject { other.m_pObject }
{
	other.m_pObject = nullptr;
}


template <typename T>
Handle<T>& Handle<T>::operator=( Handle<T>&& other )
{
	std::swap( m_pObject, other.m_pObject );
	return *this;
}



}


#endif // EXCPP_HANDLE_H_
