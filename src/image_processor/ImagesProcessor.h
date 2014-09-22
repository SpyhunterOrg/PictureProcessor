#pragma once
#include <iostream>
#include "IImgProc.h"	// interface of library
//--------------------------------------------------------------------------------------------------------------------------------
class ImagesProcessor : public IImgProc
{
public:
	virtual void GetImgExifDate(IN std::string & ImgPath, OUT std::string & RetDateTime, OUT std::string & Date, OUT std::string & Time);	// getting Foto date, saved in EXIF
	//virtual void SetImgExifDateTime(IN std::string & ImgPath, IN std::string & NewImgDate, IN std::string & NewImgTime);

	virtual void GetImgExifDateTime(IN std::string & ImgPath, OUT time_ns::timestamp & TimeStamp);
	virtual void SetImgExifDateTime(IN std::string & ImgPath, IN const time_ns::timestamp & newImgDateTime);

private:
	std::string mImgPath;
};
//--------------------------------------------------------------------------------------------------------------------------------