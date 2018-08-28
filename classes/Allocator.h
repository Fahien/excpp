#ifndef EXCPP_ALLOCATOR_H_
#define EXCPP_ALLOCATOR_H_


namespace excpp
{


template <typename T>
class Allocator
{
  public:
	Allocator( const size_t capacity = kDefaultCapacity );
	~Allocator();

	static const kDefaultCapacity { 8 };

  private:

	std::unique_ptr<T[]> m_pMemory;
};


template <typename T>
Allocator<T>::Allocator( const size_t capacity )
:	m_Capacity { capacity }
,	m_pMemory { new T[capacity] }





}


#endif // EXCPP_ALLOCATOR_H_
