//For expat branch
#include "..\errors_processing\Errors.h"	// Error descriptor
#include "..\common\GlobalSettings.h"	// Settings of application
#include "..\common\commoncode.h"

#include "exif_reader\EXIF.H"	// Exif work
//#include "ximajpg.h" - from CxImage

//#include <image.hpp>
//#include <jpgimage.hpp>

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

void ImagesProcessor::SetImgExifDateTime(IN std::string & ImgPath, IN const time_ns::timestamp & newImgDateTime/*, IN const std::string & OutDir*/)
{
	CErrorsTransport Err(LogFile, LogOut);

	std::string ImagePath = ImgPath;
	CommonCode::ReplaceSymbolInString(ImagePath,'/','\\');

	//Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(ImagePath.c_str());
	//if (image.get() == 0)
	//{
	//	CErrorsTransport Err(LogFile, LogOut);
	//	Err.clear();
	//	Err.ErrorOccured("UnableToOpenImage", "ImagesProcessor::SetImgExifDateTime", USER);
	//	Err.ErrorOccured(ImagePath.c_str(), "ImagesProcessor::SetImgExifDateTime", APPLICATION);

	//	Err.SetErrorDescription("Unable to open image file.");
	//	throw Err;
	//}

	
	//image->readMetadata();
	//Exiv2::ExifData &exifData = image->exifData();




}
//--------------------------------------------------------------------------------------------------------------------------------