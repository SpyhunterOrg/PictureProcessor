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
void CErrorsTransport::ClearObject()	// ������� �������.
{
	this->HasErrorOccured = 0;
	this->Error.clear();		// ������� ��������� ������	
	this->ErrorsList.clear();	// ������� ������ ������
}
//---------------------------------------------------------------------------------------------------------------------------------------
void CErrorsTransport::ErrorOccured(string ErrID, string ErrPoint, EErrorLevel Errlevel) 
{
	this->HasErrorOccured = 1;
	this->SetErrorID(ErrID);		// ��������� �������������� ������
	this->SetErrorPoint(ErrPoint);	// ��������� ����� ������������� ������
	this->SetErrorLevel(Errlevel);	// ��������� ������ ������

	this->ErrorsList.push_back(this->Error);	// ��������� ������ � ������

	this->WriteErrorToLogFile();	// ������ ���������� �� ������ � Log-����

}
//---------------------------------------------------------------------------------------------------------------------------------------
void CErrorsTransport::WriteErrorToLogFile()	// ������ ���������� �� ������ � Log-����
{
	//MessageBoxA(0,this->Error.ErrorID.c_str(),this->Error.ErrorPoint.c_str(),0);

	string ErrMess(Error.ErrorPoint);
	ErrMess.append(" ");
	ErrMess.append(Error.ErrorID);

	this->Logger.AppendData(ErrMess);
	this->Logger.FlushData();

	// ����� � ����
	//	#include <fstream>
	//	std::ofstream outFile("E:/filename.dat", std::ios::out | std::ios::binary);
	//	outFile.write((char *) SourceFileNameWstring.c_str(), SourceFileNameWstring.length() * sizeof(wchar_t));

	// ��� ����� ������������ �����-�� ���������� ������ ��� ������ ������ � ����
}
//---------------------------------------------------------------------------------------------------------------------------------------
void CErrorsTransport::SetErrorID(string ID)	// ��������� �������������� ������
{
	this->Error.ErrorID.clear();
	this->Error.ErrorID.append(ID);
}
//---------------------------------------------------------------------------------------------------------------------------------------
string CErrorsTransport::GetCurrentErrorID()	// ��������� �������������� ������������ ������
{	
	string emptyString;
	emptyString.clear();

	if (this->HasErrorOccured)	
	{
		if (ErrorsList.size() > 0)
		{
			return ErrorsList[0].ErrorID;	// ������ ������ ������������ ������
		}
		else
		{
			return this->Error.ErrorID;	// ���� ������ ���������, �� ���������� �� �������������
		}
	}

	return emptyString;	// ���� ������ �� ���� ���������� ������ ������
}

//---------------------------------------------------------------------------------------------------------------------------------------
string CErrorsTransport::GetCurrentErrorPoint()	//��������� ���������� � �����, ��� ��������� ������
{
	string emptyString;
	emptyString.clear();

	if (this->HasErrorOccured)	
	{
		return this->Error.ErrorPoint;	// ���� ������ ���������, �� ���������� �����, ��� ��� ���������

	}

	return emptyString;	// ���� ������ �� ���� ���������� ������ ������
}
//---------------------------------------------------------------------------------------------------------------------------------------
void CErrorsTransport::SetErrorPoint(string Point)	// ��������� ����� ������������� ������
{
	this->Error.ErrorPoint.clear();
	this->Error.ErrorPoint.append(Point);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void CErrorsTransport::SetErrorLevel(EErrorLevel ErrLev)	// ��������� ������ ������
{
	this->Error.ErrorLevel = ErrLev;
}
//---------------------------------------------------------------------------------------------------------------------------------------
EErrorLevel CErrorsTransport::GetCurrentErrorLevel()	// ��������� ������ ��������� ������
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
