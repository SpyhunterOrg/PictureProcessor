#pragma once

#include <gdiplus.h>
#include <GdiPlusHeaders.h>

#include <iostream>
#include "IImgProc.h"	// interface of library
//--------------------------------------------------------------------------------------------------------------------------------
class ImagesProcessor : public IImgProc
{
public:
	virtual void GetImgExifDate(IN std::string & ImgPath, OUT std::string & RetDateTime, OUT std::string & Date, OUT std::string & Time);	// getting Foto date, saved in EXIF
	//virtual void SetImgExifDateTime(IN std::string & ImgPath, IN std::string & NewImgDate, IN std::string & NewImgTime);

	virtual void GetImgExifDateTime(IN std::string & ImgPath, OUT time_ns::timestamp & TimeStamp);
	virtual void SetImgExifDateTime(IN std::wstring & ImgPath, IN const std::wstring & outputFilePath, IN const time_ns::timestamp & newImgDateTime);

private:
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	void ReplaceDateTimePropertyValue(Gdiplus::Bitmap * bitmap, PROPID propId, const std::string & newDateTimeStr);

	std::string mImgPath;
};
//--------------------------------------------------------------------------------------------------------------------------------