#pragma once
#ifndef COMMON_CODE_H
#define COMMON_CODE_H


#include <Windows.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <codecvt>

#include "..\common\defines.h"
#include "..\common\timestamp.h"


//typedef std::map <unsigned int, std::string> AlgListMap;	// Списки алгоритмов
//typedef std::map <unsigned int, std::string>::iterator AlgListMapIter;	// Итератор для списка алгоритмов

namespace CommonCode
{
	//---------------------------------------------------------------------------
	inline std::wstring StringToWString(IN std::string InputString)	// Конвертирование char to wchar
	{
		wchar_t * res;
		std::wstring OutputWString;
		OutputWString.clear();

		if (InputString.size() > 0)
		{
			int res_len = ::MultiByteToWideChar(CP_ACP,0,InputString.c_str(),InputString.size()/*+1*/,NULL,0);
			if (res_len == 0) 
			{
				MessageBoxA(0,"Не удалось получить длину строки UTF8", "CommonCode StringToWString",MB_OK);
				//DEBUG_ASSERT(0,"Failed to obtain utf8 std::string length","Error. VideoCapture.dll",MB_OK);
				//Err.clear();
				//Err.ErrorOccured("FailedToObtainUTF8FileSourceNameLength","VideoCapture.dll CFilterGraph::MakeGraph",APPLICATION);
				return OutputWString;
			}

			res = new wchar_t[res_len];
			::MultiByteToWideChar(CP_ACP,0,InputString.c_str(),InputString.size()/*+1*/,res,res_len/*+1*/);
			try
			{
				OutputWString.append(res,res_len);
			}
			catch(...)
			{
				MessageBoxA(0,"Не удалось получить строку UTF8", "DesktopCapture CPushSourceDesktopDC::SetImageForCaptureName",MB_OK);
				//Err.clear();
				//Err.ErrorOccured("FailedToObtainUTF8FileSourceName","VideoCapture.dll CFilterGraph::MakeGraph",APPLICATION);
				delete [] res;
				return OutputWString;
			}

			delete [] res;
		}//InputString.size() > 0


			return OutputWString;

	}
	//---------------------------------------------------------------------------
	inline std::string WStringToString(IN std::wstring InputString)	// Converts std::wstring to std::string
	{
		//setup converter
		typedef std::codecvt_utf8<wchar_t> convert_type;
		std::wstring_convert<convert_type, wchar_t> converter;

		//use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
		std::string OutputString = converter.to_bytes(InputString);\
		return OutputString;
	}
	//---------------------------------------------------------------------------
	inline long StringToLong(IN const std::string & InputString)
	{
		long value = atol(InputString.c_str());
		return value;
	}
	//---------------------------------------------------------------------------
	inline int ReplaceSymbolInString(std::string & Str, char From, char To)
	{
		size_t retval = 0;

		while(retval != std::string::npos)
		{
			retval = Str.find(From,0);
			if (retval != std::string::npos)
			{
				Str[retval] = To;
			}
		}

		return S_OK;
	}
	//---------------------------------------------------------------------------
	inline int ReplaceSymbolInString(std::wstring & Str, wchar_t From, wchar_t To)
	{
		size_t retval = 0;

		while(retval != std::wstring::npos)
		{
			retval = Str.find(From,0);
			if (retval != std::wstring::npos)
			{
				Str[retval] = To;
			}
		}

		return S_OK;
	}
	//---------------------------------------------------------------------------
	inline int ChangeAndAppendSlashes(std::string & InString)
	{
		CommonCode::ReplaceSymbolInString(InString,'/','\\');

		std::string SlashOrNot = InString.substr(InString.size()-1,1);
		if (SlashOrNot.c_str()[0] != '\\'/* && InString.c_str()[2] == '\\'*/)
		{
			InString.append("\\");
		}
		return 0;
	}
	//---------------------------------------------------------------------------
	inline bool GetSeparateDateAndTimeFromDateTime(IN const std::string & DateTime, OUT std::string & Date, OUT std::string & Time)
	{
		if (DateTime.size() < 19)//2DO - refactor DateTime in to the one format
			return false;

		Date.clear();
		Time.clear();
		Date = DateTime.substr(0,10);
		Time = DateTime.substr(11,8);
		
		return true;
	}
	//---------------------------------------------------------------------------
	inline bool ParseDateTime(IN const std::string & DateTime, OUT time_ns::timestamp & TimeStamp)
	{
		const int MAX_DATETIME_STR_LEN = 19;
		bool res = false;
		
		do 
		{
			if (DateTime.size() < MAX_DATETIME_STR_LEN)
				break;

			//2DO - refactor - use regexps.
			std::string Date;
			std::string Time;
			Date = DateTime.substr(0,10);
			Time = DateTime.substr(11,8);

			std::string Year, Month, Day;
			std::string Hour, Min, Sec;

			Year = Date.substr(0, 4);
			Month = Date.substr(5, 2);
			Day = Date.substr(8,2);

			Hour = Time.substr(0,2);
			Min = Time.substr(3,2);
			Sec = Time.substr(6,2);

			long YearL, MonthL, DayL;
			long HourL, MinL, SecL;
			YearL = StringToLong(Year);
			MonthL = StringToLong(Month);
			DayL = StringToLong(Day);
			HourL = StringToLong(Hour);
			MinL = StringToLong(Min);
			SecL = StringToLong(Sec);

			TimeStamp.set(YearL, MonthL, DayL, HourL, MinL, SecL);

			if (YearL < 1900 || YearL >2100 ||
				MonthL < 1 || MonthL > 12 ||
				DayL < 1 || DayL > 31 ||
				HourL < 0 || HourL > 24 ||
				MinL < 0 || MinL > 60 ||
				SecL < 0 || SecL > 60)
			{
				res = false;
			}
			else
			{
				res = true;
			}
		} while (false);

		return res;
	}
	//---------------------------------------------------------------------------
	inline std::string TimeStampToExifString(const time_ns::timestamp & TimeStamp)
	{
		long yearL = TimeStamp.year();
		long monthL = TimeStamp.month();
		long dayL = TimeStamp.day();
		long hourL = TimeStamp.hour();
		long minL = TimeStamp.minute();
		long secL = TimeStamp.second();

		std::string outString;
		//outString.push_back()

		std::stringstream ss;
		ss << std::setw(4) << std::setfill('0')  << static_cast<int>(yearL);
		ss << (":");
		ss << std::setw(2) << std::setfill('0')  << static_cast<int>(monthL);
		ss << (":");
		ss << std::setw(2) << std::setfill('0')  << static_cast<int>(dayL);
		
		ss << (" ");
		ss << std::setw(2) << std::setfill('0')  << static_cast<int>(hourL);
		ss << (":");
		ss << std::setw(2) << std::setfill('0')  << static_cast<int>(minL);
		ss << (":");
		ss << std::setw(2) << std::setfill('0')  << static_cast<int>(secL);

		outString = ss.str();

		return outString;
	}
	//---------------------------------------------------------------------------
	template <class TInterface, class T>
	class Instantiator
	{
	public:
		static TInterface * GetObject()
		{
			TInterface * tInterface = new T();
			return tInterface;
		}

		static void DeleteObject(TInterface * ObjectForDelete)
		{
			delete ObjectForDelete;
		}
	};
	//---------------------------------------------------------------------------
	// Create a string with last error message
	inline std::string GetLastErrorStdStr()
	{
		DWORD error = GetLastError();
		if (error)
		{
			LPVOID lpMsgBuf;
			DWORD bufLen = FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				error,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR) &lpMsgBuf,
				0, NULL );
			if (bufLen)
			{
				LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
				std::string result(lpMsgStr, lpMsgStr+bufLen);

				LocalFree(lpMsgBuf);

				return result;
			}
		}
		return std::string();
	}
	//---------------------------------------------------------------------------
}
#endif
