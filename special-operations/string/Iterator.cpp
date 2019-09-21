#include <Iterator.h>

#include <ExString.h>

namespace excpp
{

StringIterator::StringIterator( String& s )
:	it { s.get_mut_c_str(), s.get_length() }
{}

char& StringIterator::next()
{
	auto& ret = *it;
	++it;
	return ret;
}


}