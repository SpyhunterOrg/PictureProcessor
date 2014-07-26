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
class CFileSystemWork : public CFileSystemWorkInterface	// class for working with filesystem
{
public:
	CFileSystemWork();
	~CFileSystemWork();

	virtual void GetFilesList(IN const std::string & DirPath, const std::string & FileMask, IN bool IncludePath, OUT std::vector<std::string> & FileList);	// Getting list of files in dir
	virtual void GetJPEGFilesList(IN const std::string & DirPath, IN bool IncludePath, OUT std::vector<std::string> & FileList);	// Getting list of JPEG and JPG files in dir
	virtual void CopyOneFileToNewDir(IN const std::string & OldFilePath, OUT const std::string & NewFilePath);	// Copying one file to new path.

private:

	virtual std::string GetOnlyPathToFileFromFullPath(IN const std::string & FullPath);
};
//--------------------------------------------------------------------------------------------------------------------------------