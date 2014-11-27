#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include "IAppController.h"
#include "..\common\defines.h"


//--------------------------------------------------------------------------------------------------------------------------------
class CAppController : public IAppController
{
public:
	CAppController();
	virtual ~CAppController(){};
	virtual void CopyAndRenameFilesToNewDestination(IN const std::string & PathToDirWithImageFiles /*, callback*/);
	virtual void ChangeFilesDate(IN const std::string & PathToDirWithImageFiles, IN const std::wstring & OutputFolder, IN const time_ns::duration & TimeDuration /*, callback*/);


private:
	virtual void GetNewFolderAndFileNames(IN std::string OldName,
											IN std::string NewSubfolderName,
											IN const std::string & ExifDate,
											IN unsigned int NumberForCurrentFolder,
											OUT std::string & NewName);

	virtual std::wstring CalculateImageNewPath(const std::wstring & OldFilePath, const std::wstring & OutputFolderName);

	std::string mNewPathForNewImages;
	std::unique_ptr<IFileSystemWork> FSObject;
};
//--------------------------------------------------------------------------------------------------------------------------------


#endif



