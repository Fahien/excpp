#ifndef EXCPP_TABLE_H_
#define EXCPP_TABLE_H_

#include <vector>
#include <utility>

#include "Common.h"


namespace excpp
{

enum class TableError
{
	KeyNotFound
};


/// @brief Unordered Map of pairs key-value
template <typename K, typename V>
class Table
{
  public:
	Table( std::vector<std::pair<K,V>> t = {} ) : m_Table { t } {}

	size_t GetSize() const { return m_Table.size(); }

	bool IsEmpty() const { return GetSize() == 0; }

	Result<std::pair<K,V>, TableError> Get( const K& key );

	Option<V> Put( const K& key, const V& value );

	Option<std::pair<K,V>> Remove( const K& key );

  private:
	std::vector<std::pair<K,V>> m_Table;
};


template <typename K, typename V>
Result<std::pair<K,V>, TableError> Table<K,V>::Get( const K& key )
{
	for ( auto& p : m_Table )
	{
		if ( key == p.first )
		{
			return Ok<std::pair<K,V>>( &p );
		}
	}

	return Err<TableError>( TableError::KeyNotFound );
}


template <typename K, typename V>
Option<V> Table<K,V>::Put( const K& key, const V& value )
{
	auto result = Get( key );

	if ( result.IsOk() )
	{
		auto& p = result.GetVal();
		auto oldValue = p.second;
		p.second = value;

		return Some<V>( oldValue );
	}
	else
	{
		auto p = std::pair<K,V>{ key, value };
		m_Table.push_back( p );
		return None{};
	}
}

template <typename K, typename V>
Option<std::pair<K,V>> Table<K,V>::Remove( const K& key )
{
	for ( size_t i { 0 }; i < m_Table.size(); ++i )
	{
		if ( key == m_Table[i].first )
		{
			auto p = m_Table[i];
			m_Table.erase( m_Table.begin() + i );
			return Some<std::pair<K,V>>{ p };
		}
	}

	return None{};
}

}


#endif // EXCPP_TABLE_H_
