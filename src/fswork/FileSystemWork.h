#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <shlobj.h>

#include "IFileSystemWork.h"	// Interface

#include "..\errors_processing\Errors.h"	// Error descriptor
#include "..\common\GlobalSettings.h"		// Settings of application
#include "..\common\commoncode.h"
#include "..\common\defines.h"


//--------------------------------------------------------------------------------------------------------------------------------
class CFileSystemWork : public IFileSystemWork	// class for working with filesystem
{
public:
	CFileSystemWork();
	virtual ~CFileSystemWork();

	virtual void GetFilesList(IN const std::string & DirPath, const std::string & FileMask, IN bool IncludePath, OUT std::vector<std::string> & FileList);	// Getting list of files in dir
	virtual void GetJPEGFilesList(IN const std::string & DirPath, IN bool IncludePath, OUT std::vector<std::string> & FileList);	// Getting list of JPEG and JPG files in dir
	virtual void CopyOneFileToNewDir(IN const std::string & OldFilePath, OUT const std::string & NewFilePath);	// Copying one file to new path.

	virtual void CreateSubfoldersForFilePath(IN const std::wstring & FilePath);

	virtual std::wstring GetFileNameFromPath(IN const std::wstring & FilePath);	//Extract only filename from full file path. (C:\111\text.txt -> test.txt)
	virtual std::wstring GetFileFolderPath(IN const std::wstring & FullPath);	//Extract full folder path from full file path. (C:\111\text.txt -> C:\111)

private:
	bool DoesFolderExist(const std::wstring & FolderPath);
	
};
//--------------------------------------------------------------------------------------------------------------------------------