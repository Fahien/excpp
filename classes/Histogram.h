#ifndef EXCPP_HISTOGRAM_H_
#define EXCPP_HISTOGRAM_H_


#include <map>


namespace excpp
{


template <typename K, typename V>
class Histogram
{
  public:
	Histogram( std::map<K,V> h = {} ) : m_Map { h } {}

	auto begin() { return m_Map.begin(); }
	auto end()   { return m_Map.end();   }

	void Print() const;

  private:
	std::map<K,V> m_Map;
};


}


#endif // EXCPP_HISTOGRAM_H_
