#ifndef EXCPP_DATE_H_
#define EXCPP_DATE_H_


#include <unordered_map>

#include "Common.h"


namespace excpp
{

enum class Month
{
	Invalid = 0,
	January,
	February,
	March,
	April,
	May,
	June,
	July,
	August,
	September,
	October,
	November,
	December
};

class Date
{
  public:
	Date( const u8 d = 1, const Month m = Month::January, const i32 y = 1970 );

	/// @return The day
	u8 GetDay() const { return m_Day; }

	/// @return The month
	Month GetMonth() const { return m_Month; }

	/// @return The year
	i32 GetYear() const { return m_Year; }

	/// @return The number of days in a month
	static u8 DaysIn( const Month month, const i32 year );

	/// @return Whether a year is a leap year
	static bool IsLeap( const i32 year );

	/// @brief Adds an year
	void AddYears( const i32 year = 1 );

  private:
	static const std::unordered_map<Month, u8> kDaysInMonth;

	u8 m_Day;
	Month m_Month;
	i32 m_Year;
};


}


#endif // ECPP_DATE_H_
