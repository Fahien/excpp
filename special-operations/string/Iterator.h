#ifndef EXCPP_STRING_ITERATOR_H_
#define EXCPP_STRING_ITERATOR_H_

#include <Pointer.h>

namespace excpp
{

class String;

/// External iterator for class String
class StringIterator
{
  public:
	/// Constructs an iterator for string s
	StringIterator( String& s );

	/// @return A reference to the next element
	char& next();

  private:
	Pointer<char> it;
};

}

#endif // EXCPP_STRING_ITERATOR_H_
