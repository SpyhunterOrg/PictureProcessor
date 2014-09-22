// Interface
#pragma once
#include <iostream>
#include "..\common\defines.h"
#include "..\common\timestamp.hpp"

//--------------------------------------------------------------------------------------------------------------------------------
class IImgProc
{
public:
	IImgProc(){}
	virtual ~IImgProc(){}

	virtual void GetImgExifDate(IN std::string & ImgPath, OUT std::string & RetDateTime, OUT std::string & Date, OUT std::string & Time) = 0;	// get picture date, saved in EXIF
	virtual void GetImgExifDateTime(IN std::string & ImgPath, OUT time_ns::timestamp & TimeStamp) = 0;
	virtual void SetImgExifDateTime(IN std::string & ImgPath, IN const time_ns::timestamp & newImgDateTime) = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------