// ConsoleLoader.cpp : Defines the entry point for the console application.
//
// 
#include <iostream>

#include "ConsoleLoader.h"

#include "..\image_processor\IImgProc.h"	// Interface for CxImage wrapper
#include "..\errors_processing\Errors.h"	// Error descriptor
#include "..\fswork\IFileSystemWork.h"		// Interface for class that works with file system


#include "..\app_controller\AppController.h"	// Interface for controller class

#include <iostream>


//-------------------------------------------------------------------------

enum MODE
{
	RENAME_BY_DATE = 0,
	SHIFT_DATE = 1
};

//-------------------------------------------------------------------------

int main(int argc, wchar_t * argv[])
{
	//const MODE mode = RENAME_BY_DATE;
	const MODE mode = SHIFT_DATE;

	IAppController * Interf = new CAppController();

	switch (mode)
	{
	case RENAME_BY_DATE:
		{

			//---------------------------------------------------------------------
			//Rename
			std::string Pth("D:/FOFOFO");
			try
			{
				Interf->CopyAndRenameFilesToNewDestination(Pth);
			}
			catch(CErrorsTransport Err)
			{
				MessageBoxA(0,"Не удалось скопировать файлы в новый каталог","Error in Loader", MB_OK);
			}

			MessageBoxA(0,"Файлы успешно скопированы!","Ок", MB_OK);


			//end of Rename
			//---------------------------------------------------------------------

		}
		break;

	case SHIFT_DATE:
		{

			//---------------------------------------------------------------------
			//Change EXIF date
			std::string Pth("D:/FOFOFO");

			const time_ns::duration TimeDuration(0,0,30);

			try
			{
				Interf->ChangeFilesDate(Pth, TimeDuration);
			}
			catch(CErrorsTransport Err)
			{
				MessageBoxA(0,"Не удалось изменить дату файлов","Error in Loader", MB_OK);
			}

			MessageBoxA(0,"Дата файлов успешно изменена!","Ок", MB_OK);

			//---------------------------------------------------------------------
			//end oif change EXIF date


		}
		break;
	}


	delete Interf;





	//string ImgPath = "D:\\Work\\MyWorks\\PictureProcessor\\TEST\\SNC00047.jpg";
	//CImgProcInterface * IgmObj = GetImgProcObject();

	//try
	//{
	//	IgmObj->LoadImage(ImgPath);
	//}
	//catch(CErrorsTransport Err)
	//{


	//}

	//ImgPath.clear();
	//ImgPath.append("D:\\Work\\MyWorks\\PictureProcessor\\TEST\\out.jpg");
	//try
	//{
	//	IgmObj->SaveImage(ImgPath);
	//}
	//catch(CErrorsTransport Err)
	//{


	//}


	//string aaa;
	//try
	//{
	//	IgmObj->GetImgExifDate(aaa);
	//}
	//catch(CErrorsTransport Err)
	//{
	//	//MessageBoxA(0,"1","1",MB_OK);
	//}


	//DelImgProcObject(IgmObj);


	//CFileSystemWorkInterface * FSObject = GetFileSystemWorkObject();
	//string path = "E:\\tmp\\";
	//list<string> lst;

	//try
	//{
	//	FSObject->GetFilesList(path,TRUE,lst);
	//}
	//catch(CErrorsTransport Err)
	//{


	//}

	//string SourceFileName ("E:\\tmp\\ppc.jpg");
	//string DestinationFileName("E:\\tmp\\1\\newfn.jpg");
	//try
	//{
	//	FSObject->CopyOneFileToNewDir(SourceFileName, DestinationFileName);
	//}
	//catch(CErrorsTransport Err)
	//{
	//	MessageBoxA(0,Err.GetCurrentErrorDescription().c_str(),"Error",MB_OK);
	//}

	//DelFileSystemWorkObject(FSObject);

	return 0;
}




