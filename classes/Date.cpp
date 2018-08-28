#include <cassert>
#include <cstdlib>

#include "Date.h"


using namespace excpp;
using namespace std;


const unordered_map<Month, u8> Date::kDaysInMonth {
	{ Month::January  , 31 },
	{ Month::February , 28 },
	{ Month::March    , 31 },
	{ Month::April    , 30 },
	{ Month::May      , 31 },
	{ Month::June     , 30 },
	{ Month::July     , 31 },
	{ Month::August   , 31 },
	{ Month::September, 30 },
	{ Month::October  , 31 },
	{ Month::November , 30 },
	{ Month::December , 31 },
};


/// @return Whether an year is a leap year
bool Date::IsLeap( const i32 year )
{
	// 1. If the year is evenly divisible by 4, go to step 2. Otherwise, go to step 5.
	if ( year % 4 == 0 )
	{
		// 2. If the year is evenly divisible by 100, go to step 3. Otherwise, go to step 4.
		if ( year % 100 == 0 )
		{
			// 3. If the year is evenly divisible by 400, go to step 4. Otherwise, go to step 5.
			if ( year % 400 == 0 )
			{
				// 4. The year is a leap year (it has 366 days).
				return true;
			}
		}
		else
		{
			// 4. The year is a leap year (it has 366 days).
			return true;
		}
	}
	// 5. The year is not a leap year (it has 365 days).
	return false;
}


u8 Date::DaysIn( const Month month, const i32 year )
{
	if ( month == Month::February && IsLeap( year ) )
	{
		return 29;
	}
	
	return kDaysInMonth.at( month );
}


Date::Date( const u8 day, const Month month, const i32 year )
:	m_Day   { day   }
,	m_Month { month }
,	m_Year  { year  }
{
	assert( day > 0 );
	assert( month != Month::Invalid );
	assert( day <= DaysIn( month, year ) );
}


void Date::AddYears( const i32 years )
{
	m_Year += years;

	if ( m_Month == Month::February && m_Day == 29 )
	{
		if ( !IsLeap( m_Year ) )
		{
			m_Day = 28;
		}
	}
}


int main()
{
	Date d {};
	assert( d.GetDay() == 2 && "Day is not 1" );
	assert( d.GetMonth() == Month::January && "Month is not January" );
	assert( d.GetYear() == 1970 && "Year is not 1970" );

	return EXIT_SUCCESS;
}