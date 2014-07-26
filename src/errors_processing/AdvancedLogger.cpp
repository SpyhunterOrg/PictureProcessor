//#include "stdafx.h"
#include "AdvancedLogger.h"

using namespace std;
//------------------------------------------------------------------------------
IAdvancedLogger::~IAdvancedLogger()
{

}
//------------------------------------------------------------------------------
CAdvancedLogger::CAdvancedLogger()
{
	this->BufferForWriting.clear();
	this->FileNameForWriting.clear();
	this->LogDestination.clear();

}
//------------------------------------------------------------------------------
CAdvancedLogger::~CAdvancedLogger()
{

}
//------------------------------------------------------------------------------
HRESULT CAdvancedLogger::SetOutputDestination(IN bool To_File, IN bool To_DB, IN bool Message_Box)
{
	this->LogDestination.SetToFile(To_File);
	this->LogDestination.SetToDB(To_DB);
	this->LogDestination.SetMsgBox(Message_Box);
	return S_OK;
}
//------------------------------------------------------------------------------
HRESULT CAdvancedLogger::SetOutputFileParams(IN string FileName/*, PARAMSS?*/)	// Установка файла для записи и параметров записи
{
	this->FileNameForWriting.clear();
	this->FileNameForWriting.append(FileName);
	return S_OK;
}
//------------------------------------------------------------------------------
HRESULT CAdvancedLogger::FlushData()	// Выгрузка накопленных данных в хранилище
{
	if (this->LogDestination.GetToFile())	// Запись в файл
	{

		if (this->FileNameForWriting.size() == 0)
		{
			return S_FALSE;
		}

		ofstream OutFileStream(this->FileNameForWriting.c_str(),ios::app);

		for (size_t i = 0; i < this->BufferForWriting.size(); i++)
		{
			OutFileStream << this->BufferForWriting[i].c_str();
			OutFileStream << endl;
		}

		OutFileStream.close();

	}
	else if (this->LogDestination.GetToDB())	// Запись в БД
	{
		MessageBoxA(0,"Запись в БД не реализована","CAdvancedLogger::FlushData",MB_OK);
	}
	else if (this->LogDestination.GetMsgBox())	// MessageBox
	{
		for (size_t i = 0; i < this->BufferForWriting.size(); i++)
		{
			MessageBoxA(0,this->BufferForWriting[i].c_str(),"LogMessage",MB_OK);
		}
	}
	BufferForWriting.clear();
	return S_OK;
}
//------------------------------------------------------------------------------
HRESULT CAdvancedLogger::AppendData(string InputData)	// Добавление данных в объект
{
	char str[256];
	memset(str,0,256);
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	sprintf(str, "[%02d.%02d.%d, %02d:%02d:%02d:%03d]: ", systime.wDay, systime.wMonth, systime.wYear, systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);
	
	string Time(str);

	string outString(Time);
	outString.append(InputData);

	this->BufferForWriting.push_back(outString);
	return S_OK;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
COutputLogFilePath::COutputLogFilePath(IN const string & FileName)
{
	this->LogFileOutput.clear();
	this->LogFileOutput.append("C:/");
	this->LogFileOutput.append(FileName);


	HRESULT hr = S_OK;
	DWORD dwType = REG_SZ;
	char sz[255];
	DWORD dwSize = 255;

	HKEY ReadKey;

	string CurretPathToExe = GetCurrentProcessSubfolder();




	//2DO: вынести пути в отдельный файл настроек
	hr = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\CIKT\\VCFConnect",REG_OPTION_NON_VOLATILE,KEY_READ, &ReadKey);
	if(hr == ERROR_SUCCESS)
	{
		hr = RegQueryValueExA(ReadKey,"LogSubFolder", NULL, &dwType, (LPBYTE)sz, &dwSize);
		if (hr == ERROR_SUCCESS)
		{
			this->LogFileOutput.clear();
			size_t SlashPos = CurretPathToExe.find_last_of('\\');
			string NewPath = CurretPathToExe.substr(0, SlashPos);
			NewPath.append("\\");
			NewPath.append(sz);//, dwSize);
			CreateDirectoryA(NewPath.c_str(),NULL);
			this->LogFileOutput.append(NewPath);
			this->LogFileOutput.append("\\");
			this->LogFileOutput.append(FileName);
			//ofstream paths("E:/pathrs.txt", ios::app);
			//paths << "OutFilename = "  << this->LogFileOutput.c_str() << endl;
			//paths.close();

			return;
		}
		else
		{

			//MessageBoxA(0,"Error RegQueryValueExA","",0);
		}
	}
	else
	{
		//MessageBoxA(0,"Error RegOpenKeyExA","",0);
	}

	this->LogFileOutput.clear();
	size_t SlashPos = CurretPathToExe.find_last_of('\\');
	string NewPath = CurretPathToExe.substr(0, SlashPos);
	NewPath.append("\\");
	NewPath.append("LOG");
	CreateDirectoryA(NewPath.c_str(),NULL);
	this->LogFileOutput.append(NewPath);
	this->LogFileOutput.append("\\");
	this->LogFileOutput.append(FileName);
} 
//------------------------------------------------------------------------------
string COutputLogFilePath::GetCurrentProcessSubfolder()
{
	char szFile[MAX_PATH];
	DWORD Size = 0;
	Size = GetModuleFileNameA(0,szFile,MAX_PATH);
	//if((GetModuleFileName(0,szFile,MAX_PATH)!=0) &&
	//	(GetShortPathName(szFile,szFile,MAX_PATH)!=0))

	string outString(szFile, Size);

	return outString;
}
//------------------------------------------------------------------------------


