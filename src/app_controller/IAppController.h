#ifndef I_APP_CONTROLLER_H
#define I_APP_CONTROLLER_H

#include <iostream>
#include "..\common\defines.h"
//--------------------------------------------------------------------------------------------------------------------------------
class IAppController
{
public:
	IAppController(){}
	virtual ~IAppController(){}

	virtual void CopyAndRenameFilesToNewDestination(IN const std::string & PathToDirWithImageFiles /*, callback*/) = 0;
	virtual void ChangeFilesDate(IN const std::string & PathToDirWithImageFiles, IN const std::wstring & OutputFolder, IN const time_ns::duration & TimeDuration /*, callback*/) = 0;

};
//--------------------------------------------------------------------------------------------------------------------------------
#endif
