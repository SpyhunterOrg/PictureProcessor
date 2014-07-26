#pragma once

#include <iostream>
#include <vector>
#include "..\common\defines.h"


//--------------------------------------------------------------------------------------------------------------------------------
class CFileSystemWorkInterface// : public CommonCode::Instantiator // Interface for class for working with filesystem
{
public:
	virtual void GetFilesList(IN const std::string & DirPath, const std::string & FileMask, IN bool IncludePath, OUT std::vector<std::string> & FileList) = 0;	// Getting list of files in dir
	virtual void GetJPEGFilesList(IN const std::string & DirPath, IN bool IncludePath, OUT std::vector<std::string> & FileList) = 0;	// Getting list of JPEG and JPG files in dir
	virtual void CopyOneFileToNewDir(IN const std::string & OldFilePath, OUT const std::string & NewFilePath) = 0;	// Copying one file to new path.
};
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
