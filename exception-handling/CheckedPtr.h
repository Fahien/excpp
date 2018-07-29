#ifndef CPP_CHECKEDPTR_H_
#define CPP_CHECKEDPTR_H_

#include <cstddef>
#include <stdexcept>
#include <string>


class OutOfRange : public std::runtime_error
{
public:
	OutOfRange(std::string message)
	:	std::runtime_error{ message }
	{}
};


template<typename T>
class CheckedPtr
{
public:
	template<size_t N>
	CheckedPtr(T (&array)[N]);

	T& operator[](const size_t index);
	const T& operator[](const size_t index) const;

private:
	void checkIndex(const size_t index);

	T* mArray;
	size_t mSize;

};


template<typename T>
template<size_t N>
CheckedPtr<T>::CheckedPtr(T (&array)[N])
:	mArray{ array }
,	mSize{ N }
{}


template<typename T>
void CheckedPtr<T>::checkIndex(const size_t index)
{
	if (index >= mSize)
	{
		std::string message{ "Index [" +
		                     std::to_string(index) +
		                     "] must be less than the array size [" +
		                     std::to_string(mSize) +
		                     "]" };
		throw OutOfRange{ message };
	}
}


template<typename T>
T& CheckedPtr<T>::operator[](const size_t index)
{
	checkIndex(index);
	return mArray[index];
}


template<typename T>
const T& CheckedPtr<T>::operator[](const size_t index) const
{
	checkIndex(index);
	return mArray[index];
}


#endif // CPP_CHECKEDPTR_H_