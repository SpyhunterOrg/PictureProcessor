//#include "stdafx.h"
#include "FileSystemWork.h"


//--------------------------------------------------------------------------------------------------------------------------------
CFileSystemWork::CFileSystemWork()
{
}
//--------------------------------------------------------------------------------------------------------------------------------
CFileSystemWork::~CFileSystemWork()
{
	int a = 0;
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

	// get path to new file
	size_t Slash = NewFilePath.find_last_of('\\');

	std::string Path = NewFilePath.substr(0,Slash+1);

	
	std::wstring WPath = CommonCode::StringToWString(Path);
	
	int ret = SHCreateDirectory(0,WPath.c_str());
	
	CopyFileA(OldFilePath.c_str(), NewFilePath.c_str(), FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------
void CFileSystemWork::CreateSubfoldersForFilePath(IN const std::wstring & FilePath)
{
	CErrorsTransport Err(LogFile, LogOut);

	std::wstring FileFoldePath = GetFileFolderPath(FilePath);

	if (!DoesFolderExist(FileFoldePath))
	{
		int retCode = SHCreateDirectory(0,FileFoldePath.c_str());
		if (retCode != 0)
		{
			std::string ErrorStr = CommonCode::GetLastErrorStdStr();

			Err.clear();
			Err.ErrorOccured("UnableToCreateFoldersPath", "fswork.dll CFileSystemWork::CreateSubfoldersForFilePath", USER);
			std::string ErrMess("Path ");
			ErrMess.append(CommonCode::WStringToString(FilePath));
			Err.SetErrorDescription(ErrMess);
			throw Err;
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------------------
std::wstring CFileSystemWork::GetFileFolderPath(IN const std::wstring & FullPath)
{
	CErrorsTransport Err(LogFile, LogOut);

	auto SlashPos = FullPath.rfind('\\');

	std::wstring OnlyPath;
	if (std::wstring::npos != SlashPos)
	{
		OnlyPath = FullPath.substr(0, SlashPos);
	}
	else
	{
		Err.clear();
		Err.ErrorOccured("NoSlashesInFilePath", "fswork.dll CFileSystemWork::GetFileFolderPath", USER);
		std::string ErrMess("File ");
		ErrMess.append(CommonCode::WStringToString(FullPath));
		ErrMess.append(" does not exist");
		Err.SetErrorDescription(ErrMess);
		throw Err;
	}

	return OnlyPath;
}
////--------------------------------------------------------------------------------------------------------------------------------
std::wstring CFileSystemWork::GetFileNameFromPath(IN const std::wstring & FilePath)
{
	CErrorsTransport Err(LogFile, LogOut);

	std::wstring fileName;

	std::wstring CorrectFilePath(FilePath);
	CommonCode::ReplaceSymbolInString(CorrectFilePath, L'/', L'\\');

	const size_t last_slash_idx = FilePath.rfind('\\');
	if (std::string::npos != last_slash_idx)
	{
		fileName = FilePath.substr(last_slash_idx + 1, FilePath.length() - last_slash_idx - 1);
	}
	else
	{
		Err.clear();
		Err.ErrorOccured("NoSlashesInFilePath", "fswork.dll CFileSystemWork::GetFileNameFromPath", USER);
		std::string ErrMess("File ");
		ErrMess.append(CommonCode::WStringToString(FilePath));
		ErrMess.append(" does not exist");
		Err.SetErrorDescription(ErrMess);
		throw Err;
	}
	return fileName;
}
//--------------------------------------------------------------------------------------------------------------------------------
bool CFileSystemWork::DoesFolderExist(const std::wstring & FolderPath)
{
		DWORD dwAttrib = GetFileAttributesW(FolderPath.c_str());

		return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
			(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}
//--------------------------------------------------------------------------------------------------------------------------------
//
//inline std::wstring GetFileFolderPath(const std::wstring & FilePath)
//{
//	std::wstring folderPath;
//
//	//using only "//" characters
//	const size_t last_slash_idx_bs = FilePath.rfind(L'/');
//	if (last_slash_idx_bs != std::wstring::npos)
//	{
//		std::wstring emptyStr;
//		return emptyStr;
//	}
//
//	const size_t last_slash_idx = FilePath.rfind('\\');
//	if (std::string::npos != last_slash_idx)
//	{
//		folderPath = FilePath.substr(0, last_slash_idx);
//	}
//
//	return folderPath;
//}
////---------------------------------------------------------------------------
//inline std::wstring GetFileNameFromPath(const std::wstring & FilePath)
//{
//	std::wstring fileName;
//
//	//using only "//" characters
//	const size_t last_slash_idx_bs = FilePath.rfind(L'/');
//	if (last_slash_idx_bs != std::wstring::npos)
//	{
//		std::wstring emptyStr;
//		return emptyStr;
//	}
//
//	const size_t last_slash_idx = FilePath.rfind('\\');
//	if (std::string::npos != last_slash_idx)
//	{
//		size_t ll = FilePath.length() - 1;
//		int a = last_slash_idx + 1;
//		int b = FilePath.length() - last_slash_idx;
//
//		fileName = FilePath.substr(last_slash_idx + 1, FilePath.length() - last_slash_idx - 1);
//	}
//
//	return fileName;
//}
//---------------------------------------------------------------------------