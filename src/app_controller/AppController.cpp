#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>

#include "..\fswork\IFileSystemWork.h"
#include "..\fswork\FileSystemWork.h"

#include "..\image_processor\IImgProc.h"
#include "..\image_processor\ImagesProcessor.h"

#include "..\errors_processing\Errors.h"	// Error descriptor /
#include "..\common\GlobalSettings.h"	// Settings of application
#include "..\common\commoncode.h"

#include "AppController.h"

//--------------------------------------------------------------------------------------------------------------------------------

typedef std::pair<std::string, std::string> StringStringPair;
typedef std::vector<StringStringPair> StringStringPairsVector;
//--------------------------------------------------------------------------------------------------------------------------------

bool CompareStringStringPairs(StringStringPair pair1, StringStringPair pair2)
{
	return pair1.second < pair2.second;
}

//using namespace std;
//--------------------------------------------------------------------------------------------------------------------------------
void CAppController::CopyAndRenameFilesToNewDestination(IN const std::string & PathToDirWithImageFiles /*, callback*/)
{
	this->mNewPathForNewImages.clear();
	this->mNewPathForNewImages.append(PathToDirWithImageFiles);

	CommonCode::ChangeAndAppendSlashes(mNewPathForNewImages);

	//----------------------------------------------------------------------------------------------------------
	//1.Get Files List
	auto FSObject = std::unique_ptr<CFileSystemWorkInterface>(new CFileSystemWork());
	std::vector<std::string> lst;

	try
	{
		FSObject->GetJPEGFilesList(mNewPathForNewImages,TRUE,lst);
	}
	catch(CErrorsTransport Err)
	{
		Err.ErrorOccured("UnableToGetFilesList","CAppController::CopyAndRenameFilesToNewDestination",APPLICATION);
		throw Err;
	}

	//----------------------------------------------------------------------------------------------------------
	//2. Get Array Of Dates and Times - filenames
	auto ImpProc = std::unique_ptr<IImgProc>(new ImagesProcessor());

	StringStringPairsVector ImgPathAndDateVector;
	size_t ListSize = lst.size();
	
	//----- Extract files dates and times in vector for sorting. (Sorting needs to enumerate files with one date in right order)
	for (size_t i= 0; i < ListSize; i++)
	{
		std::string DateTimeOfImage;
		try
		{
			std::string Date;
			std::string Time;

			ImpProc->GetImgExifDate(lst[i], DateTimeOfImage, Date, Time);

			StringStringPair oneImg = std::make_pair(lst[i], DateTimeOfImage);
			ImgPathAndDateVector.push_back(oneImg);
		}
		catch(CErrorsTransport Err)
		{
			//In case of error do nothing, just skip file
		}
	}
 
	//------------------------Sort file list by date and time
	std::sort(ImgPathAndDateVector.begin(), ImgPathAndDateVector.end(), CompareStringStringPairs);
	size_t SortedListSize = ImgPathAndDateVector.size();

	if (SortedListSize != ListSize)
	{
		CErrorsTransport Err(LogFile, LogOut);
		Err.clear();
		Err.ErrorOccured("SortedListSizeIncorrectSize", "CAppController::CopyAndRenameFilesToNewDestination", APPLICATION);
		Err.SetErrorDescription("SortedListSize != ListSize.");
		throw Err;
	}

	//------------------------
	//Generate new file names 
	std::string PrevDate;
	int CurrentDateImgCounter = 0;
	std::vector<std::string> NewFilePathsAndNames;


	for (size_t i= 0; i < SortedListSize; i++)
	{
		std::string ImgDateTime = ImgPathAndDateVector[i].second;
		std::string ImgOldPath = ImgPathAndDateVector[i].first;

		std::string Date, Time;
		CommonCode::GetSeparateDateAndTimeFromDateTime(ImgDateTime, Date, Time);

		if (PrevDate == Date)
		{
			++CurrentDateImgCounter;
		}
		else
		{
			CurrentDateImgCounter = 1;
			PrevDate = Date;
		}


		std::string ResultName;
		this->GetNewFolderAndFileNames(ImgOldPath, "OUT", ImgDateTime, CurrentDateImgCounter, ResultName);	// Get new path and name of file
		NewFilePathsAndNames.push_back(ResultName);

	}

	//----------------------------------------------------------------------------------------------------------
	//3. Copy all files to new destination with new names.
	for (size_t i= 0; i < ListSize; i++)
	{
		try
		{
			if (NewFilePathsAndNames[i].size() > 0)
			{
				std::string oldFilePath = ImgPathAndDateVector[i].first;
				FSObject->CopyOneFileToNewDir(oldFilePath, NewFilePathsAndNames[i]);
			}
		}
		catch(CErrorsTransport Err)
		{
			Err.ErrorOccured("UnableToCopyFile","CAppController::CopyAndRenameFilesToNewDestination",APPLICATION);
		}
	}
	//----------------------------------------------------------------------------------------------------------
	
}

//--------------------------------------------------------------------------------------------------------------------------------

void CAppController::ChangeFilesDate(IN const std::string & PathToDirWithImageFiles, IN const std::string & OutputFolder,  IN const time_ns::duration & TimeDuration /*, callback*/)
{
	CErrorsTransport Err(LogFile, LogOut);

	//----------------------------------------------------------------------------------------------------------
	//1.Get Files List
	std::vector<std::string> lst;
	{
		auto FSObject = std::unique_ptr<CFileSystemWorkInterface>(new CFileSystemWork());

		try
		{
			FSObject->GetJPEGFilesList(PathToDirWithImageFiles,TRUE,lst);
		}
		catch(CErrorsTransport Err)
		{
			Err.ErrorOccured("UnableToGetFilesList","CAppController::CopyAndRenameFilesToNewDestination",APPLICATION);
			throw Err;
		}
	}

	//----------------------------------------------------------------------------------------------------------
	//2. Get EXIF date and time

	auto ImpProc = std::unique_ptr<IImgProc>(new ImagesProcessor());

	size_t ListSize = lst.size();

	for (size_t i= 0; i < ListSize; i++)
	{
		try
		{
			time_ns::timestamp ImageDateTime;
			ImpProc->GetImgExifDateTime(lst[i], ImageDateTime);
			//long day = ImageDateTime.day();
			//long hour = ImageDateTime.hour();
			//long minute = ImageDateTime.minute();
			//long sec = ImageDateTime.second();
			
			time_ns::timestamp NewImageDateTime = ImageDateTime + TimeDuration;
			//long day1 = NewImageDateTime.day();
			//long hour1 = NewImageDateTime.hour();
			//long minute1 = NewImageDateTime.minute();
			//long sec1 = NewImageDateTime.second();

			ImpProc->SetImgExifDateTime(lst[i], OutputFolder, NewImageDateTime);

		}
		catch(CErrorsTransport Err)
		{
			Err.ErrorOccured("UnableToChangeFileDateAndTime", "CAppController::ChangeFilesDate", APPLICATION);
			Err.SetErrorDescription("Unable To Change File Date And Time.");
			throw Err;
		}
	}

}

//--------------------------------------------------------------------------------------------------------------------------------
void CAppController::GetNewFolderAndFileNames(IN std::string OldName,
												IN std::string NewSubfolderName,
												IN const std::string & ExifDate,
												IN UINT NumberForCurrentFolder,
												OUT std::string & NewName )
{
	CErrorsTransport Err(LogFile, LogOut);

	//Parse time and Date - 2DOo: check on other images
	//"2009:05:09 14:23:39" - on Samsung i900



	// Moved to special method
	std::string Date;
	std::string Time;
	CommonCode::GetSeparateDateAndTimeFromDateTime(ExifDate, Date, Time);

	CommonCode::ReplaceSymbolInString(Date, ':', '_');
	CommonCode::ReplaceSymbolInString(Time, ':', '_');
	CommonCode::ReplaceSymbolInString(OldName,'/','\\');

	// Get Old Extension
	size_t DotPos = OldName.find_last_of('.');
	if (DotPos == 0 )
	{
		Err.clear();
		Err.ErrorOccured("FileHaseNotExtension", "AppController.dll CAppController::GetNewFolderAndFileNames", APPLICATION);
		std::string ErrMess("There is no extension in file name ");
		ErrMess.append(OldName);
		Err.SetErrorDescription(ErrMess);
		throw Err;
	}

	size_t ExtensionLength = OldName.size()-DotPos-1;
	std::string OldExtension = OldName.substr(DotPos+1,ExtensionLength);
		
	//GetOldNameWithoutPathAndExtension
	size_t LastSlashPos = OldName.find_last_of('\\');
	size_t OldNameLength = (OldName.size() - LastSlashPos - 1) - (OldExtension.size() + 1);
	std::string OldNameWithoutPathAndExtension = OldName.substr(LastSlashPos+1,OldNameLength);
	

	std::stringstream stream;
	stream << NumberForCurrentFolder;
	std::string NumberForCurrentFolderString = stream.str();


	// GetOldPath
	std::string OldPath = OldName.substr(0,LastSlashPos+1);

//IMG_2682-2010_02_28-09_08.jpg
         //2008_03_07-05_40_46-0.jpg


//2008_03_07__05_40_46__0.jpg
	

	NewName.clear();
	NewName.append(OldPath);
	NewName.append(NewSubfolderName);
	CommonCode::ChangeAndAppendSlashes(NewName);	// add '\\'
	NewName.append(Date);
	CommonCode::ChangeAndAppendSlashes(NewName);	// add '\\'
	NewName.append(Date);
	NewName.append("-");
	NewName.append(Time);
	NewName.append("-");
	NewName.append(NumberForCurrentFolderString);
	NewName.append(".");
	NewName.append(OldExtension);

}

//--------------------------------------------------------------------------------------------------------------------------------
