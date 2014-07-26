#ifndef I_APP_CONTROLLER_H
#define I_APP_CONTROLLER_H

#include <iostream>
#include "..\common\defines.h"
//--------------------------------------------------------------------------------------------------------------------------------
class IAppController
{
public:
	virtual void CopyAndRenameFilesToNewDestination(IN const std::string & PathToDirWithImageFiles /*, callback*/) = 0;
	virtual void ChangeFilesDate(IN const std::string & PathToDirWithImageFiles, IN const time::duration & TimeDuration /*, callback*/) = 0;

};
//--------------------------------------------------------------------------------------------------------------------------------
#endif
