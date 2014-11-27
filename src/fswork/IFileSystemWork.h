#pragma once

#include <iostream>
#include <vector>
#include "..\common\defines.h"


//--------------------------------------------------------------------------------------------------------------------------------
class IFileSystemWork// : public CommonCode::Instantiator // Interface for class for working with filesystem
{
public:
	IFileSystemWork(){}
	virtual ~IFileSystemWork(){}

	virtual void GetFilesList(IN const std::string & DirPath, const std::string & FileMask, IN bool IncludePath, OUT std::vector<std::string> & FileList) = 0;	// Getting list of files in dir
	virtual void GetJPEGFilesList(IN const std::string & DirPath, IN bool IncludePath, OUT std::vector<std::string> & FileList) = 0;	// Getting list of JPEG and JPG files in dir
	virtual void CopyOneFileToNewDir(IN const std::string & OldFilePath, OUT const std::string & NewFilePath) = 0;	// Copying one file to new path.

	virtual void CreateSubfoldersForFilePath(IN const std::wstring & FilePath) = 0;

	virtual std::wstring GetFileNameFromPath(IN const std::wstring & FilePath) = 0;
	virtual std::wstring GetFileFolderPath(IN const std::wstring & FullPath) = 0;

};
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
