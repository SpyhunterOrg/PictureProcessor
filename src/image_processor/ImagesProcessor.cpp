#include "..\errors_processing\Errors.h"	// Error descriptor

#include <memory>
#include <gdiplus.h>
#include <GdiPlusHeaders.h>
#pragma comment(lib,"gdiplus.lib")

#include "exif_reader\EXIF.H"	// Exif work
#include "..\common\commoncode.h"
#include "..\common\GlobalSettings.h"	// Settings of application

#include "ImagesProcessor.h"

//--------------------------------------------------------------------------------------------------------------------------------
void ImagesProcessor::GetImgExifDate(IN std::string & ImgPath, OUT std::string & RetDateTime, OUT std::string & Date, OUT std::string & Time)	// getting Foto date, saved in EXIF
{
	CErrorsTransport Err(LogFile, LogOut);
	RetDateTime.clear();

	this->mImgPath.clear();
	this->mImgPath.append(ImgPath);

	CommonCode::ReplaceSymbolInString(this->mImgPath,'/','\\');	// only right symbols in path

	FILE * fp;
	fp = fopen(this->mImgPath.c_str(),"rb");
	if (fp == 0)	// if we can't open file
	{
		Err.clear();
		Err.ErrorOccured("FileDoesNotExists", "ImagesProcessor::GetImgExifDate", APPLICATION);
		Err.ErrorOccured(this->mImgPath.c_str(), "ImagesProcessor::GetImgExifDate", APPLICATION);

		std::string Descr(this->mImgPath);
		Descr.append(" ");
		Descr.append("File does not exists");
		Err.SetErrorDescription(Descr);
		throw Err;
	}


	Cexif exif;
	//CxImageJPG exif;
	
	if (!exif.DecodeExif(fp))	// If no EXIF data in image
	{
		Err.clear();
		Err.ErrorOccured("ThereIsNoEXIFDataInImage", "ImagesProcessor::GetImgExifDate", APPLICATION);
		Err.ErrorOccured(this->mImgPath.c_str(), "ImagesProcessor::GetImgExifDate", APPLICATION);
		Err.SetErrorDescription("There is no EXIF data in image.");
		fclose(fp);
		throw Err;
	}

	fclose(fp);

	RetDateTime.append(exif.m_exifinfo->DateTime);

	if (RetDateTime.size() == 0)
	{
		Err.clear();
		Err.ErrorOccured("ThereIsNoEXIFDateAndTimeInImage", "ImagesProcessor::GetImgExifDate", APPLICATION);
		Err.ErrorOccured(this->mImgPath.c_str(), "ImagesProcessor::GetImgExifDate", APPLICATION);
		Err.SetErrorDescription("There is no EXIF Date and Time in image.");
		RetDateTime.clear();
		throw Err;
	}

	CommonCode::GetSeparateDateAndTimeFromDateTime(RetDateTime, Date, Time);
}
//--------------------------------------------------------------------------------------------------------------------------------
void ImagesProcessor::GetImgExifDateTime(IN std::string & ImgPath, OUT time_ns::timestamp & TimeStamp)
{
	std::string DateTime, Date, Time;
	GetImgExifDate(ImgPath, DateTime, Date, Time);

	if (!CommonCode::ParseDateTime(DateTime, TimeStamp))
	{
		CErrorsTransport Err(LogFile, LogOut);
		Err.clear();
		Err.ErrorOccured("UnableToParseFileDateTime", "ImagesProcessor::GetImgExifDateTime", APPLICATION);
		Err.ErrorOccured(ImgPath.c_str(), "ImagesProcessor::GetImgExifDateTime", APPLICATION);
		Err.SetErrorDescription("Unable to parse file date time stamp.");
		throw Err;
	}
}
//--------------------------------------------------------------------------------------------------------------------------------

void ImagesProcessor::SetImgExifDateTime(IN std::string & ImgPath, IN const std::string & outputFolder, IN const time_ns::timestamp & newImgDateTime/*, IN const std::string & OutDir*/)
{
	CErrorsTransport Err(LogFile, LogOut);

	Gdiplus::Status stat;
	CommonCode::ReplaceSymbolInString(ImgPath,'/','\\');
	std::string newImgDateTimeStr = CommonCode::TimeStampToExifString(newImgDateTime);

	// Initialize GDI+
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	
	CLSID clsid;
	// Get the CLSID of the JPEG encoder.
	int retVal = GetEncoderClsid(L"image/jpeg", &clsid);
	if (retVal == -1)
	{
		Err.clear();
		Err.ErrorOccured("UnableGetEncoderClsid", "ImagesProcessor::SetImgExifDateTime", USER);
		Err.SetErrorDescription("Unable to get clsid of jpeg encoder");
		throw Err;
	}

	std::wstring imgPathWstr = CommonCode::StringToWString(ImgPath);
	Gdiplus::Bitmap * bitmap = new Gdiplus::Bitmap(imgPathWstr.c_str());
	
	ReplaceDateTimePropertyValue(bitmap, PropertyTagExifDTOrig, newImgDateTimeStr);
	ReplaceDateTimePropertyValue(bitmap, PropertyTagExifDTDigitized, newImgDateTimeStr);
	ReplaceDateTimePropertyValue(bitmap, PropertyTagDateTime, newImgDateTimeStr);

	stat = bitmap->Save(imgPathWstr.c_str(), &clsid, NULL);
	if(stat != Gdiplus::Ok)
	{
		std::string errorMessage = CommonCode::GetLastErrorStdStr();

		Err.clear();
		Err.ErrorOccured("UnableToSaveFileWithNewDateTime", "ImagesProcessor::SetImgExifDateTime", USER);
		Err.ErrorOccured(ImgPath.c_str(), "ImagesProcessor::SetImgExifDateTime", USER);
		std::string ErrMess("Unable to write date time stamp to the file .");
		ErrMess.append(ImgPath.c_str());
		ErrMess.append("]. New date and time sting = [");
		ErrMess.append(newImgDateTimeStr.c_str());
		ErrMess.append("]. Reason: [");
		ErrMess.append(errorMessage.c_str());
		ErrMess.append("].");
		Err.SetErrorDescription(ErrMess);

		delete bitmap;
		Gdiplus::GdiplusShutdown(gdiplusToken);

		throw Err;
	}

	delete bitmap;
	Gdiplus::GdiplusShutdown(gdiplusToken);
}

//--------------------------------------------------------------------------------------------------------------------------------
int ImagesProcessor::GetEncoderClsid(const WCHAR * format, CLSID * pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes
	Gdiplus::ImageCodecInfo * pImageCodecInfo = NULL;
	Gdiplus::Status stat;

	stat = Gdiplus::GetImageEncodersSize(&num, &size);
	if(stat != Gdiplus::Ok)
		return -1;  // Failure

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;  // Failure

	stat = Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);
	if(stat != Gdiplus::Ok)
		return -1;  // Failure

	for(UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}    
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}
//--------------------------------------------------------------------------------------------------------------------------------
void ImagesProcessor::ReplaceDateTimePropertyValue(Gdiplus::Bitmap * bitmap, PROPID propId, const std::string & newDateTimeStr)
{
	UINT  size = 0;
	UINT  count = 0;
	Gdiplus::Status stat;

	#define MAX_PROPTYPE_SIZE 30
	WCHAR strPropertyType[MAX_PROPTYPE_SIZE] = L"";

	bitmap->GetPropertySize(&size, &count);

	// GetAllPropertyItems returns an array of PropertyItem objects.
	// Allocate a buffer large enough to receive that array.
	Gdiplus::PropertyItem * pPropBuffer =(Gdiplus::PropertyItem*)malloc(size);

	// Get the array of PropertyItem objects.
	bitmap->GetAllPropertyItems(size, count, pPropBuffer);

	for(UINT j = 0; j < count; ++j)
	{
		if (pPropBuffer[j].id == propId)
		{
			try
			{
				Gdiplus::PropertyItem tmpPropItem;
				tmpPropItem = pPropBuffer[j];
				char * tmpStr = (char*)malloc(newDateTimeStr.size()+1);
				strcpy(tmpStr, newDateTimeStr.c_str());
				tmpPropItem.value = tmpStr;
				stat = bitmap->SetPropertyItem(&tmpPropItem);
				free(tmpStr);

				break;
			}
			catch(CErrorsTransport Err)
			{
				Err.ErrorOccured("UnableToReplaceDateTimeValue", "ImagesProcessor::SetImgExifDateTime", USER);
				std::string ErrMess("Unable to replace date and time in EXIF. Filename [");
				ErrMess.append(newDateTimeStr.c_str());
				ErrMess.append("]");
				Err.SetErrorDescription(ErrMess);
				free(pPropBuffer);
				throw Err;
			}
		}
	} //for(UINT j = 0; j < count; ++j)

	free(pPropBuffer);
}
//--------------------------------------------------------------------------------------------------------------------------------