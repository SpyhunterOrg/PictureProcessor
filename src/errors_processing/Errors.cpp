//#include "stdafx.h"
#include "Errors.h"

using namespace std;
//---------------------------------------------------------------------------------------------------------------------------------------
CErrorsTransport::CErrorsTransport()
{
	this->ClearObject();

	//this->Logger.SetOutputFileParams("C:/CustomLog.log");
	this->Logger.SetOutputDestination(FALSE, FALSE, FALSE);

}
//---------------------------------------------------------------------------------------------------------------------------------------
CErrorsTransport::CErrorsTransport(COutputLogFilePath OutputFileParams, CLogDestination LogsOutput)
{
	this->ClearObject();
	this->Logger.SetOutputFileParams(OutputFileParams.LogFileOutput);
	this->Logger.SetOutputDestination(LogsOutput.GetToFile(), LogsOutput.GetToDB(),LogsOutput.GetMsgBox());

}
//---------------------------------------------------------------------------------------------------------------------------------------
CErrorsTransport::~CErrorsTransport()
{


}
//---------------------------------------------------------------------------------------------------------------------------------------
void CErrorsTransport::ClearObject()	// Очистка объекта.
{
	this->HasErrorOccured = 0;
	this->Error.clear();		// Очистка последней ошибки	
	this->ErrorsList.clear();	// Очистка списка ошибок
}
//---------------------------------------------------------------------------------------------------------------------------------------
void CErrorsTransport::ErrorOccured(string ErrID, string ErrPoint, EErrorLevel Errlevel) 
{
	this->HasErrorOccured = 1;
	this->SetErrorID(ErrID);		// Установка идентификатора ошибки
	this->SetErrorPoint(ErrPoint);	// Установка места возникновения ошибки
	this->SetErrorLevel(Errlevel);	// Установка уровня ошибки

	this->ErrorsList.push_back(this->Error);	// Добавляем ошибку в список

	this->WriteErrorToLogFile();	// Запись информации об ошибке в Log-файл

}
//---------------------------------------------------------------------------------------------------------------------------------------
void CErrorsTransport::WriteErrorToLogFile()	// Запись информации об ошибке в Log-файл
{
	//MessageBoxA(0,this->Error.ErrorID.c_str(),this->Error.ErrorPoint.c_str(),0);

	string ErrMess(Error.ErrorPoint);
	ErrMess.append(" ");
	ErrMess.append(Error.ErrorID);

	this->Logger.AppendData(ErrMess);
	this->Logger.FlushData();

	// Вывод в файл
	//	#include <fstream>
	//	std::ofstream outFile("E:/filename.dat", std::ios::out | std::ios::binary);
	//	outFile.write((char *) SourceFileNameWstring.c_str(), SourceFileNameWstring.length() * sizeof(wchar_t));

	// тут нужно использовать какой-то глобальный объект для записи ошибок в файл
}
//---------------------------------------------------------------------------------------------------------------------------------------
void CErrorsTransport::SetErrorID(string ID)	// Установка идентификатора ошибки
{
	this->Error.ErrorID.clear();
	this->Error.ErrorID.append(ID);
}
//---------------------------------------------------------------------------------------------------------------------------------------
string CErrorsTransport::GetCurrentErrorID()	// Получение идентификатора произошедшей ошибки
{	
	string emptyString;
	emptyString.clear();

	if (this->HasErrorOccured)	
	{
		if (ErrorsList.size() > 0)
		{
			return ErrorsList[0].ErrorID;	// Отдаем первую произошедшую ошибку
		}
		else
		{
			return this->Error.ErrorID;	// Если ошибка произошла, то возвращаем ее идентификатор
		}
	}

	return emptyString;	// Если ошибки не было возвращаем пустую строку
}

//---------------------------------------------------------------------------------------------------------------------------------------
string CErrorsTransport::GetCurrentErrorPoint()	//Получение информации о месте, где произошла ошибка
{
	string emptyString;
	emptyString.clear();

	if (this->HasErrorOccured)	
	{
		return this->Error.ErrorPoint;	// Если ошибка произошла, то возвращаем место, где она произошла

	}

	return emptyString;	// Если ошибки не было возвращаем пустую строку
}
//---------------------------------------------------------------------------------------------------------------------------------------
void CErrorsTransport::SetErrorPoint(string Point)	// Установка места возникновения ошибки
{
	this->Error.ErrorPoint.clear();
	this->Error.ErrorPoint.append(Point);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void CErrorsTransport::SetErrorLevel(EErrorLevel ErrLev)	// Установка уровня ошибки
{
	this->Error.ErrorLevel = ErrLev;
}
//---------------------------------------------------------------------------------------------------------------------------------------
EErrorLevel CErrorsTransport::GetCurrentErrorLevel()	// Получение уровня возникшей ошибки
{
	return this->Error.ErrorLevel;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void CErrorsTransport::SetErrorDescription(const string & ErrorDescription)
{
	this->Error.ErrorDescription.clear();
	this->Error.ErrorDescription.append(ErrorDescription);
	return;
}
//---------------------------------------------------------------------------------------------------------------------------------------
string CErrorsTransport::GetCurrentErrorDescription()
{
	return this->Error.ErrorDescription;
}
//---------------------------------------------------------------------------------------------------------------------------------------
