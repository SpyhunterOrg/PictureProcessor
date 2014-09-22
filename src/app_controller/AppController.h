#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include "IAppController.h"
#include "..\common\defines.h"


//--------------------------------------------------------------------------------------------------------------------------------
class CAppController : public IAppController
{
public:
	virtual void CopyAndRenameFilesToNewDestination(IN const std::string & PathToDirWithImageFiles /*, callback*/);
	virtual void ChangeFilesDate(IN const std::string & PathToDirWithImageFiles, IN const time_ns::duration & TimeDuration /*, callback*/);


private:
	virtual void GetNewFolderAndFileNames(IN std::string OldName,
											IN std::string NewSubfolderName,
											IN const std::string & ExifDate,
											IN unsigned int NumberForCurrentFolder,
											OUT std::string & NewName);

	std::string mNewPathForNewImages;
};
//--------------------------------------------------------------------------------------------------------------------------------


#endif



