//#include "stdafx.h"
#include "FileSystemWork.h"


//--------------------------------------------------------------------------------------------------------------------------------
CFileSystemWork::CFileSystemWork()
{

}
//--------------------------------------------------------------------------------------------------------------------------------
void CFileSystemWork::GetFilesList(IN const std::string & DirPath, const std::string & FileMask, IN bool IncludePath, OUT std::vector<std::string> & FileList)	// Getting list of files in dir
{
	std::string DirPathNoBadSymbols(DirPath);
	CommonCode::ChangeAndAppendSlashes(DirPathNoBadSymbols);

	std::string SearchPath(DirPathNoBadSymbols);
	std::string NewDirPath(DirPathNoBadSymbols);

	if (FileMask.empty())
		SearchPath.append("*");
	else
		SearchPath.append(FileMask);

	CErrorsTransport Err(LogFile, LogOut);
	FileList.clear();

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFileA(SearchPath.c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)	// If cant find any files in dir or occured some error while getting First File
	{
		Err.clear();
		Err.ErrorOccured("FindFirstFileFailed", "fswork.dll CFileSystemWork::GetFilesList", USER);
		Err.SetErrorDescription("FindFirstFile failed");
		FindClose(hFind);
		//2DO: Make GetLastError and GetError Description
		//GetLastError();
		throw Err;
	}

	// If first file in the dir exists and it is not "." or ".."
	if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && strcmp(FindFileData.cFileName,"..") && strcmp(FindFileData.cFileName,"."))
	{
		if (IncludePath == TRUE)
		{
			std::string tmpstr(NewDirPath);
			tmpstr.append(FindFileData.cFileName);  
			FileList.push_back(tmpstr);  
		}
		else
		{
			FileList.push_back(FindFileData.cFileName);  
		}
		
	}

	BOOL a = TRUE;
	while(a != FALSE)
	{
		a = FindNextFile(hFind, &FindFileData);
		if (a != FALSE)
		{
			if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && strcmp(FindFileData.cFileName,"..") && strcmp(FindFileData.cFileName,"."))
			{	
				if (IncludePath == TRUE)
				{
					std::string tmpstr(NewDirPath);
					tmpstr.append(FindFileData.cFileName);  
					FileList.push_back(tmpstr);  
				}
				else
				{
					FileList.push_back(FindFileData.cFileName);  
				}
			}
		}
	}

	FindClose(hFind);

	if (FileList.size() == (size_t)0)
	{
		Err.clear();
		Err.ErrorOccured("NoFilesInDir", "fswork.dll CFileSystemWork::GetFilesList", USER);
		Err.SetErrorDescription("No files in dir");
		throw Err;
	}
}
//--------------------------------------------------------------------------------------------------------------------------------
void CFileSystemWork::GetJPEGFilesList(IN const std::string & DirPath, IN bool IncludePath, OUT std::vector<std::string> & FileList)
{
	this->GetFilesList(DirPath,JPG_FILETYPE,IncludePath,FileList);
	std::vector<std::string> lstJpeg;
	this->GetFilesList(DirPath,JPEG_FILETYPE,IncludePath,lstJpeg);
	FileList.insert(FileList.end(), lstJpeg.begin(), lstJpeg.end());
}
//--------------------------------------------------------------------------------------------------------------------------------
void CFileSystemWork::CopyOneFileToNewDir(IN const std::string & OldFilePath, OUT const std::string & NewFilePath)
{
	CErrorsTransport Err(LogFile, LogOut);


	//try to open sourcefile
	std::ifstream ifs(OldFilePath.c_str());
	if (ifs == NULL)
	{
		Err.clear();
		Err.ErrorOccured("FileIsNotExists", "fswork.dll CFileSystemWork::CopyOneFileToNewDir", USER);
		std::string ErrMess("File ");
		ErrMess.append(OldFilePath);
		ErrMess.append(" is not exists");
		Err.SetErrorDescription(ErrMess);
		throw Err;
	}


	//try
	//{
	//	string PathToFile = this->GetOnlyPathToFileFromFullPath(OldFilePath);
	//}
	//catch(CErrorsTransport Err)
	//{
	//	Err.ErrorOccured("FileIsNotExists", "fswork.dll CFileSystemWork::CopyOneFileToNewDir", USER);
	//	string ErrMess("File ");
	//	ErrMess.append(OldFilePath);
	//	ErrMess.append(" is not exists");
	//	Err.SetErrorDescription(ErrMess);
	//	throw Err;
	//}

	//CreateDirectoryA()
	//SHCreateDirectoryEx()

	// get path to new file
	size_t Slash = NewFilePath.find_last_of('\\');

	std::string Path = NewFilePath.substr(0,Slash+1);

	
	std::wstring WPath = CommonCode::StringToWString(Path);
	
	//WPath.append(L"/");
	int ret = SHCreateDirectory(0,WPath.c_str());
	//int ret = SHCreateDirectory(0,L"E:/tmp/test/123/321");
	
	CopyFileA(OldFilePath.c_str(), NewFilePath.c_str(), FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------
std::string CFileSystemWork::GetOnlyPathToFileFromFullPath(IN const std::string & FullPath)
{
	CErrorsTransport Err(LogFile, LogOut);

	//try to open sourcefile
	std::ifstream ifs(FullPath.c_str());
	if (ifs == NULL)
	{
		Err.clear();
		Err.ErrorOccured("FileIsNotExists", "fswork.dll CFileSystemWork::GetOnlyPathToFileFromFullPath", USER);
		std::string ErrMess("File ");
		ErrMess.append(FullPath);
		ErrMess.append(" is not exists");
		Err.SetErrorDescription(ErrMess);
		throw Err;
	}

	//If file exists get it path
	int SlashPos = FullPath.find_last_of('\\');

	std::string OnlyPath = FullPath.substr(0, SlashPos+1);

	return OnlyPath;
}
//--------------------------------------------------------------------------------------------------------------------------------