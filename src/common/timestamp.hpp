//////////////////////////////////////////////////////////////////////////////
// $Id: timestamp.hpp,v 1.1 2004/09/28 06:19:26 pavel Exp $
//
// Timestamp library.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef _PM_TIMESTAMP_HPP_
#define _PM_TIMESTAMP_HPP_
#pragma once

#include <iosfwd>
#include <iomanip>
#include <windows.h>

namespace time
{

// forward
class duration;

/** Timstamp class
*	Holds number of hundrends nanoseconds since 01.01.1601 00:00
*	@sa Windows FILETIME struct
*/
class timestamp : public FILETIME
{
public:
	/// Date part of timestamp
	struct date
	{
		/// Construct date
		date(long year = 0, long month = 0, long day = 0) : year(year), month(month), day(day), day_of_week(0), day_of_year(0) {}
		long year, month, day, day_of_week, day_of_year;
	};
	/// Time part of timestamp
	struct time
	{
		/// Construct time
		time(long hour = 0, long minute = 0, long second = 0) : hour(hour), minute(minute), second(second) {}
		long hour, minute, second;
	};
	/// Fractional part of timestamp
	struct time_frac
	{
		time_frac(long milli =0, long micro = 0, long nano = 0) : milli(milli), micro(micro), nano(nano) {}
		long milli, micro, nano;
	};

	/// Default ctor - makes 01.01.1601 00:00 timestamp
	timestamp()
	{
		dwLowDateTime = dwHighDateTime = 0;
	}
	/// Construct from parts
	timestamp(long year, long month, long day, long hour = 0, long minute = 0, long second = 0, 
		long milli = 0, long micro = 0, long nano = 0)
	{
		//init();
		set(year, month, day, hour, minute, second, milli, micro, nano);
	}
	/// Construct from Windows FILETIME struct
	explicit timestamp(const FILETIME& ft)
	{
		dwLowDateTime = ft.dwLowDateTime;
		dwHighDateTime = ft.dwHighDateTime;
	}
	/// Assign Windows FILETIME struct
	timestamp& operator=(const FILETIME & ft)
	{
		dwLowDateTime = ft.dwLowDateTime;
		dwHighDateTime = ft.dwHighDateTime;
		return *this;
	}

	/// Construct from Windows SYSTEMTIME struct
	explicit timestamp(const SYSTEMTIME& st)
	{
		::SystemTimeToFileTime(&st, this);
	}

	/// Assign Windows SYSTEMTIME struct
	timestamp& operator=(const SYSTEMTIME& st)
	{
		::SystemTimeToFileTime(&st, this);
		return *this;
	}
	/// Convert to Windows SYSTEMTIME struct
	operator SYSTEMTIME() const
	//SYSTEMTIME as_SYSTEMTIME() const
	{
		SYSTEMTIME st;
		::FileTimeToSystemTime(this, &st);
		return st;
	}

	/// Get current time value
	static timestamp current()
	{
		timestamp result;
		::GetSystemTimeAsFileTime(&result);
		return result;
	}

	bool operator==(const timestamp& rhs) const { return ::CompareFileTime(this, &rhs) == 0; }
	bool operator!=(const timestamp& rhs) const { return ::CompareFileTime(this, &rhs) != 0; }
	bool operator<(const timestamp& rhs)  const { return ::CompareFileTime(this, &rhs) < 0; }
	bool operator>(const timestamp& rhs)  const { return ::CompareFileTime(this, &rhs) > 0; }
	bool operator<=(const timestamp& rhs) const { return ::CompareFileTime(this, &rhs) <= 0; }
	bool operator>=(const timestamp& rhs) const { return ::CompareFileTime(this, &rhs) >= 0; }

	/// Subtract duration
	timestamp& operator-=(const duration& span);
	/// Subtract duration
	timestamp operator-(const duration& span) const
	{
		timestamp result(*this);
		result -= span;
		return result;
	}

	/// Add duration
	timestamp& operator+=(const duration& span);
	/// Add duration
	timestamp operator+(const duration& span) const
	{
		timestamp result(*this);
		result+=span;
		return result;
	}

	/// Time difference
	duration operator-(const timestamp& t) const;

	/// Set up new timestamp
	void set(const date& d, const time& t, const time_frac& tf)
	{
		set(d.year, d.month, d.day,
			t.hour, t.minute, t.second,
			tf.milli, tf.micro, tf.nano);
	}
	/// Set up new date and time
	void set(const date& d, const time& t) { set(d, t, get_time_frac()); }
	/// Set up new date
	void set(const date& d) { set(d, get_time()); }
	void set(long year, long month, long day, long hour, long minute, long second, 
		long milli = 0, long micro = 0, long nano = 0)
	{
		micro	+= nano / 1000;		nano	%= 1000;
		milli	+= micro / 1000;	micro	%= 1000;
		second	+= milli / 1000;	milli	%= 1000;
		minute	+= second / 60;		second	%= 60;
		hour	+= minute / 60;		minute	%= 60;
		day		+= hour / 24;		hour	%= 24;
		
		
		// num days
		ULARGE_INTEGER impl;
		impl.QuadPart = (year * 365) + (year / 4) - (year / 100) + (year / 400) + 
							days_from_1st_jan(month-1) + day;

		bool is_leap_year = !(year & 3) && ( (year % 100) || !(year % 400) );
		if ( is_leap_year && month < 3 )
			--impl.QuadPart;

		// offset to 01.01.1601
		impl.QuadPart -= 584754L;
		// convert to seconds
		impl.QuadPart *= 86400L;
		impl.QuadPart += (hour * 3600) + (minute * 60) + second;
		// convert to hundreds of nanoseconds
		impl.QuadPart *= 10000000L;
		impl.QuadPart += (milli * 10000L) + (micro * 10L) + (nano + 50) / 100;
		set_impl(impl);
	}

	/// Get date part
	date get_date() const
	{
		date result;
		// Number of days since 1/1/0
		long days = static_cast<long>(get_impl().QuadPart / 10000000L / 86400L + 584754L);
		
//		days /= 86400L; days   += ;	//  Add days from 1/1/0 to 01/01/1601

		// Calculate the day of week (sun=1, mon=2...)
		//   -1 because 1/1/0 is Sat.  +1 because we want 1-based
		result.day_of_week = ((days - 1) % 7L) + 1;
		
		// Number of 400 year increments since 1/1/0
		// Leap years every 4 yrs except centuries not multiples of 400.
		long n400_years = days / 146097L; 
		// Set nDaysAbsolute to day within 400-year block
		days %= 146097L;

		// Number of 4 year increments since 1/1/0
		long n4_years = days / 1461L;
		// Day within 4 year block (0 is 1/1/yr1, 1460 is 12/31/yr4)
		long n4_day = days % 1461L;

		// Century within 400 year block (0,1,2 or 3)
		// -1 because first century has extra day
		long n400_century = (days - 1) / 36524L;

		bool is_leap = true;
		if ( n400_century )	// Non-leap century
		{
			// Set nDaysAbsolute to day within century
			days = (days - 1) % 36524L;
			// +1 because 1st 4 year increment has 1460 days
			n4_years = (days + 1) / 1461L;
			n4_day = n4_years? (days + 1) % 1461L : days;
			is_leap = 0 != n4_years;
		}

		// Year within 4 year block (0,1,2 or 3)
		long n4_yr = n4_day / 365;
		if ( is_leap )
		{
			n4_yr = (n4_day - 1 )/ 365; // -1 because first year has 366 days
			if ( n4_yr )
				n4_day = (n4_day - 1) % 365;
		}
		else
			n4_day %= 365;


		// n4Day is now 0-based day of year. Save 1-based day of year, year number
		result.day_of_year = n4_day + 1;
		result.year = n400_years * 400 + n400_century * 100 + n4_years * 4 + n4_yr;


		// Handle leap year: before, on, and after Feb. 29.
		if ( !n4_yr && is_leap ) // Leap Year
		{
			if ( days_from_1st_jan(2) == n4_day ) // Feb. 29
			{
				result.month = 2;
				result.day = 29;
				return result;
			}
			// Pretend it's not a leap year for month/day comp.
			if ( n4_day > days_from_1st_jan(2) )
				--n4_day;
		}

		// Make n4_day a 1-based day of non-leap year and compute
		//  month/day for everything but Feb. 29.
		++n4_day;

		// Month number always >= n/32, so save some loop time
		result.month = (n4_day >> 5) + 1;
		while ( n4_day > days_from_1st_jan(result.month) )
			++result.month;
		result.day = n4_day - days_from_1st_jan(result.month - 1);
		return result;
	}

	/// Get time part
	time get_time() const
	{
		time result;
		// Time in seconds since midnight
		result.second = static_cast<long>(get_impl().QuadPart / 10000000L % 86400L);
		if ( result.second )
		{
			result.minute = result.second / 60;	result.second %= 60;
			result.hour = result.minute / 60;	result.minute %= 60;
		}
		return result;
	}

	/// Get fractal time part
	time_frac get_time_frac() const
	{
		time_frac result;
		result.nano = static_cast<long>(get_impl().QuadPart % 10000000L);
		if ( result.nano )
		{
			result.milli = result.nano / 10;
			result.micro = result.milli % 1000;
			result.milli /= 1000;
			result.nano = (result.nano % 10) * 100;
		}
		return result;
	}

	/// Year value
	long year()        const { return get_date().year; }
	/// Month value
	long month()       const { return get_date().month; }
	/// Day value
	long day()         const { return get_date().day; }
	/// Day of week value
	long day_of_week() const { return get_date().day_of_week; }
	/// Day of year value
	long day_of_year() const { return get_date().day_of_year; }
	
	/// Hour value
	long hour()        const { return get_time().hour; }
	/// Minutes value
	long minute()      const { return get_time().minute; }
	/// Seconds value
	long second()      const { return get_time().second; }
	
	/// Milli seconds value
	int milli()       const { return get_time_frac().milli; }
	/// Micro seconds value
	int micro()       const { return get_time_frac().micro; }
	/// Nano seconds value
	int nano()        const { return get_time_frac().nano; }

	void clear()
	{
		ULARGE_INTEGER uli =  { 0, };
		set_impl(uli);
	}

	/*
	timestamp localize() const
	{
		FILETIME ft = *this;
		::FileTimeToLocalFileTime(&ft, &ft);
		return timestamp(ft);
	}

	timestamp delocalize() const
	{
		FILETIME ft = *this;
		::LocalFileTimeToFileTime(&ft, &ft);
		return timestamp(ft);
	}
*/
	timestamp& local()
	{
		::FileTimeToLocalFileTime(this, this);
		return *this;
	}
	timestamp& universal()
	{
		::LocalFileTimeToFileTime(this, this);
		return *this;
	}

	/// Print timestamp to output stream
	void print(std::ostream& os) const
	{
		using namespace std;
		date d = get_date();
		time t = get_time();
		time_frac tf = get_time_frac();
		os <<
			setw(2) << setfill('0') << d.day	<< '.' << 
			setw(2) << setfill('0') << d.month	<< '.' << 
			setw(4) << setfill('0') << d.year	<< ' ' <<
			setw(2) << setfill('0') << t.hour	<< ':' << 
			setw(2) << setfill('0') << t.minute	<< ':' <<
			setw(2) << setfill('0') << t.second
			//<< ' ' <<
			//setw(3) << setfill('0') << tf.milli	<< '.' <<
			//setw(3) << setfill('0') << tf.micro	<< '.' << 
			//setw(3) << setfill('0') << tf.nano
			;
	}
	/// Print timestamp to output stream
	inline friend std::ostream& operator<<(std::ostream& os, const timestamp& ts)
	{
		ts.print(os);
		return os;
	}

	void input(std::istream& is)
	{
		using namespace std;
		date d;
		is >> d.day;	is.seekg(1, ios_base::cur);
		is >> d.month;	is.seekg(1, ios_base::cur);
		is >> d.year;	is.seekg(1, ios_base::cur);
		time t;
		is >> t.hour;	is.seekg(1, ios_base::cur);
		is >> t.minute;	is.seekg(1, ios_base::cur);
		is >> t.second;	is.seekg(1, ios_base::cur);
		set(d, t);
	}
	inline friend std::istream& operator>>(std::istream& is, timestamp& ts)
	{
		ts.input(is);
		return is;
	}

	
	static const long long SECONDS_IN_MINUTE = 60LL;
	static const long long SECONDS_IN_HOUR = SECONDS_IN_MINUTE * 60LL;
	static const long long SECONDS_IN_DAY = SECONDS_IN_HOUR * 24LL;
	static const long long SECONDS_RATIO = 10000000LL;

	timestamp& ceil(long long ratio = SECONDS_RATIO)
	{
		ULARGE_INTEGER impl = get_impl();
		bool fract_exists = impl.QuadPart % ratio;
		impl.QuadPart = (impl.QuadPart / ratio + fract_exists) * ratio;
		set_impl(impl);
		return *this;
	}

	timestamp& floor(long long ratio = SECONDS_RATIO)
	{
		ULARGE_INTEGER impl = get_impl();
		impl.QuadPart = (impl.QuadPart / ratio) * ratio;
		set_impl(impl);
		return *this;
	}

	timestamp& ceil_millis()	{ return ceil(SECONDS_RATIO); }
	timestamp& floor_millis()	{ return floor(SECONDS_RATIO); }
	timestamp& ceil_seconds()	{ return ceil(SECONDS_RATIO * SECONDS_IN_MINUTE); }
	timestamp& floor_seconds()	{ return floor(SECONDS_RATIO * SECONDS_IN_MINUTE); }
	timestamp& ceil_minutes()	{ return ceil(SECONDS_RATIO * SECONDS_IN_HOUR); }
	timestamp& floor_minutes()	{ return floor(SECONDS_RATIO * SECONDS_IN_HOUR); }
	timestamp& ceil_hours()		{ return ceil(SECONDS_RATIO * SECONDS_IN_DAY); }
	timestamp& floor_hours()	{ return floor(SECONDS_RATIO * SECONDS_IN_DAY); }
private:
	ULARGE_INTEGER get_impl() const
	{
		ULARGE_INTEGER impl;
		impl.LowPart = dwLowDateTime;
		impl.HighPart = dwHighDateTime;
		return impl;
	}
	void set_impl(const ULARGE_INTEGER& impl)
	{
		dwLowDateTime = impl.LowPart;
		dwHighDateTime = impl.HighPart;
	}

	/// Number of days from year begin
	static long days_from_1st_jan(int month)
	{
		static long days[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
		return days[month];
	}
};

/// Time duration
class duration
{
	friend class timestamp;
public:
	/// Default ctor
	duration()
	{
		value_.QuadPart = 0;
	}
	/// Construct from parts
	duration(long days, long hours = 0, long minutes = 0, long seconds = 0,
				long millis = 0, long micros = 0, long nanos = 0)
	{
		set(days, hours, minutes, seconds, millis, micros, nanos);
	}

	/// Set up new duration value
	void set(long days, long hours = 0, long minutes = 0, long seconds = 0,
				long millis = 0, long micros = 0, long nanos = 0)
	{
		nanos = (nanos + (nanos < 0? -50 : 50)) / 100;
		micros  += nanos / 10;		nanos %= 10;
		millis  += micros / 1000;	micros %= 1000;
		seconds += millis / 1000;	millis %= 1000;
		minutes += seconds / 60;	seconds %= 60;
		hours	+= minutes / 60;	minutes %= 60;
		days	+= hours / 24;		hours %= 24;
	
		value_.QuadPart = days * 86400L;
		value_.QuadPart += hours * 3600L + minutes * 60  + seconds;
		value_.QuadPart *= 10000000L;
		value_.QuadPart += millis * 10000L + micros * 10L + nanos;
	}
	
	bool operator==(const duration& rhs) const { return value_.QuadPart == rhs.value_.QuadPart; }
	bool operator!=(const duration& rhs) const { return value_.QuadPart != rhs.value_.QuadPart; }
	bool operator<(const duration& rhs)  const { return value_.QuadPart < rhs.value_.QuadPart; }
	bool operator>(const duration& rhs)  const { return value_.QuadPart > rhs.value_.QuadPart; }
	bool operator<=(const duration& rhs) const { return value_.QuadPart <= rhs.value_.QuadPart; }
	bool operator>=(const duration& rhs) const { return value_.QuadPart >= rhs.value_.QuadPart; }
	/// Subtract duration
	duration& operator-=(const duration& span)
	{
		value_.QuadPart -= span.value_.QuadPart;
		return *this;
	}
	/// Subtract duration
	duration operator-(const duration& span) const
	{
		duration result(*this);
		result -= span;
		return result;
	}
	/// Add duration
	duration& operator+=(const duration& span)
	{
		value_.QuadPart += span.value_.QuadPart;
		return *this;
	}
	/// Add duration
	duration operator+(const duration& span) const
	{
		duration result(*this);
		result += span;
		return result;
	}

	/// Divide duration
	duration& operator /=(double ratio)
	{
		value_.QuadPart = static_cast<LONGLONG>(value_.QuadPart / ratio);
		return *this;
	}
	inline friend duration operator /(const duration& d, double ratio)
	{
		duration result(d);
		result /= ratio;
		return result;
	}
	inline friend duration operator /(double ratio, const duration& d)
	{
		duration result(d);
		result /= ratio;
		return result;
	}
	inline friend double operator/(const duration& d1, const duration& d2)
	{
		return static_cast<double>(d1.value_.QuadPart) / static_cast<double>(d2.value_.QuadPart);
	}
	/// Mulitiple duration
	duration& operator *=(double ratio)
	{
		value_.QuadPart = static_cast<LONGLONG>(value_.QuadPart * ratio);
		return *this;
	}
	inline friend duration operator *(const duration& d, double ratio)
	{
		duration result(d);
		result *= ratio;
		return result;
	}
	inline friend duration operator *(double ratio, const duration& d)
	{
		duration result(d);
		result *= ratio;
		return result;
	}

	/// Span in days
	long long total_days() const { return days(); }
	/// Days part in span
	long long days() const { return total_seconds() / timestamp::SECONDS_IN_DAY; }
	/// Hours in span
	long long total_hours() const { return total_seconds() / timestamp::SECONDS_IN_HOUR; }
	/// Hours part in span
	long long hours() const { return (total_seconds() % timestamp::SECONDS_IN_DAY) / timestamp::SECONDS_IN_HOUR; }
	/// Minutes in span
	long long total_minutes() const { return total_seconds() / timestamp::SECONDS_IN_MINUTE; }
	/// Minutes part in span
	long long minutes() const { return (total_seconds() % timestamp::SECONDS_IN_HOUR) / timestamp::SECONDS_IN_MINUTE; }
	/// Seconds in span
	long long total_seconds() const { return value_.QuadPart / timestamp::SECONDS_RATIO; }
	/// Seconds part in span
	long long seconds() const { return total_seconds() % timestamp::SECONDS_IN_MINUTE; }
	/// Milli seconds in span
	long long total_millis() const { return value_.QuadPart / 10000LL; }
	/// Milli seconds part in span
	long long millis() const { return (value_.QuadPart % timestamp::SECONDS_RATIO) / 10000LL; }
	/// Micro seconds in span
	long long total_micros() const { return value_.QuadPart / 10LL; }
	/// Micro seconds part in span
	long long micros() const { return (value_.QuadPart % 10000LL) / 10L; }
	/// Nano seconds in span
	long long total_nanos() const { return value_.QuadPart * 100LL; }
	/// Nano seconds part in span
	long long nanos() const { return (value_.QuadPart % 10LL) * 100LL; }
private:
	LARGE_INTEGER value_;
};

// implementation

/// Subtract duration
inline timestamp& timestamp::operator-=(const duration& span)
{
	ULARGE_INTEGER imp = get_impl();
	imp.QuadPart -= span.value_.QuadPart;
	set_impl(imp);
	return *this;
}

/// Add duration
inline timestamp& timestamp::operator+=(const duration& span)
{
	ULARGE_INTEGER imp = get_impl();
	imp.QuadPart += span.value_.QuadPart;
	set_impl(imp);
	return *this;
}

/// Time difference
inline duration timestamp::operator-(const timestamp& t) const
{
	ULARGE_INTEGER imp1 = get_impl(), imp2 = t.get_impl();
	duration result;
	result.value_.QuadPart = imp1.QuadPart - imp2.QuadPart;
	return result;
}

} //namespace time


#endif // #ifndef _PM_TIMESTAMP_HPP_