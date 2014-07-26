//------------------------------------------------------------------------------
// File: CAdvancedLogger.h
//------------------------------------------------------------------------------

//! ������ ��� ����������������.
/*!
    \author User
    \file CAdvancedLogger.h
*/

#pragma once

#include <iostream>
#include <vector>
#include <fstream>	// for ofstream
#include <windows.h>



/**
* @class COutputLogFilePath 
* ������ ����� ������������ ��� ��������� � �������� ���� � ������ ����������������.
*/
/// ����� ������������ ��� ��������� � �������� ���� � ������ ����������������.
//------------------------------------------------------------------------------
class COutputLogFilePath
{
public:
	COutputLogFilePath(IN const std::string & FileName);	// � ������������ �������� ������� ���� � ����� �� �������
	std::string GetCurrentProcessSubfolder();	/**<
											* \brief ��������� ���� �������� ��������
											*
											* @return ����, �� �������� ���� �������� ������� ����������
											*/

	std::string LogFileOutput;	///< ���� � ����� ����������������
};
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* @class CLogDestination 
* ������ ����� ������������ ��� ���������� �����������, � ������� ����� �������������� ����������������.
* ��������� ���������� �����������:
<ul>
<li>����;
<li>���� ������;
<li>���� � ��������� ����������.
</ul>
*/
///����� ������������ ��� ���������� �����������, � ������� ����� �������������� ����������������.
class CLogDestination	// Descriptor of destination of logging
{
public:

	/// @name ���c��������
	/// @{
	CLogDestination(){ ToFile = FALSE; ToDB = FALSE; MsgBox = FALSE; }	///< ����������� �� ���������
	CLogDestination(bool ToFil, bool ToDB, bool MessBox)
	{
		this->SetToFile(ToFil);
		this->SetToDB(ToDB);
		this->SetMsgBox(MessBox);
	}	/**<
		* \brief �������� ������� CLogDestination � �������������� ����������� ��� ����������������
		*
		* @param ToFil - ���������������� � ����
		* @param ToDB - ���������������� � ���� ������
		* @param MessBox - ����� ���� � ��������� ����������
		*/
	/// @}

	bool GetToDB(){ return ToDB;}	/**<
									* \brief ��������� ���������, ������������� ����������� ���������������� � ����
									*
									* @return ����������� ��� ������������� ���������������� � ����
									*/

	bool GetToFile(){ return ToFile;}/**<
									* \brief ��������� ���������, ������������� ����������� ���������������� � ���� ������
									*
									* @return ����������� ��� ������������� ���������������� � ���� ������
									*/

	bool GetMsgBox() { return MsgBox;}/**<
									* \brief ��������� ���������, ������������� ����������� ������ ��������� �� �������������� ��������� � ���� ���� � ��������� ����������
									*
									* @return ����������� ��� ������������� ������ ��������� �� �������������� ��������� � ���� ���� � ��������� ����������
									*/

	void SetToDB(bool val){ToDB = val;}/**<
									* \brief ��������� ���������, ������������� ����������� ���������������� � ���� ������
									*
									* @param val - ����������� ��� ������������� ���������������� � ���� ������
									*/

	void SetToFile(bool val){ ToFile = val;}/**<
									* \brief ��������� ���������, ������������� ����������� ���������������� � ����
									*
									* @param val - ����������� ��� ������������� ���������������� � ����
									*/
	void SetMsgBox(bool val){ MsgBox = val; }/**<
									* \brief ��������� ���������, ������������� ����������� ������ ��������� �� �������������� ��������� � ���� ���� � ��������� ����������
									*
									* @param val - ����������� ��� ������������� ������ ��������� �� �������������� ��������� � ���� ���� � ��������� ����������
									*/

	void clear(){ ToDB = FALSE; ToFile = FALSE; MsgBox = FALSE;}	/**<
																	* \brief ��������� �������� �������� �������
																	*
																	*/

private:
	bool ToFile;	///< ����������� ��� ������������� ���������������� � ����
	bool ToDB;		///< ����������� ��� ������������� ���������������� � ���� ������
	bool MsgBox;	///< ����������� ��� ������������� ������ ��������� �� �������������� ��������� � ���� ���� � ��������� ����������
};
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
* @interface IAdvancedLogger 
* ��������� ������ CAdvancedLogger, ������� ��������� ��������� ������ ����������������.
*
*/
///��������� ������ CAdvancedLogger, ������� ��������� ��������� ������ ����������������.
class IAdvancedLogger
{
public:


	/// @name ����������
	/// @{
	virtual ~IAdvancedLogger() = 0;
	/// @}

	// ------�������������------
	virtual HRESULT SetOutputDestination(IN bool To_File, IN bool To_DB, IN bool Message_Box) = 0;	/**<
										* \brief ��������� ����������� ��� ����������������
										*
										* @param To_File - ���������������� � ����
										* @param To_DB - ���������������� � ���� ������
										* @param Message_Box - ����� ���� � ��������� ����������
										*/

	virtual HRESULT SetOutputFileParams(IN std::string FileName/*, PARAMSS?*/) = 0;	/**<
														* \brief  ��������� ���������� ��� ���������������� � ����
														*
														* @param FileName - ��� � ���� � ����� ��� ����������������
														*/
	//virtual HRESULT SetDBParams() = 0;	// ��������� ���������� ��

	// ------������ � �������------
	virtual HRESULT FlushData() = 0;	/**<
										* \brief �������� ����������� ������ � ��������� ����������
										*
										*/

	virtual HRESULT AppendData(std::string InputData) = 0;	/**<
														* \brief ���������� ������ � ������
														*
														* @param InputData - ������ ��� ����������������
														*/
	//virtual HRESULT AppendData(UINT InputData) = 0;		// ���������� ������ � ������
	//virtual HRESULT AppendData(int InputData) = 0;		// ���������� ������ � ������
	//virtual HRESULT AppendData(char * InputData, UINT InputDataLength) = 0;	// ���������� ������ � ������

	//virtual HRESULT DirectWriteData(std::string InputData) = 0;	// ������ ������ ������
	//virtual HRESULT DirectWriteData(UINT InputData) = 0;	// ������ ������ ������
	//virtual HRESULT DirectWriteData(int InputData) = 0;		// ������ ������ ������
	//virtual HRESULT DirectWriteData(char * InputData, UINT InputDataLength) = 0;	// ������ ������ ������

};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/**
* @class CAdvancedLogger 
* ����� ��������� ��������� ������ ����������������.
*
*/
///����� ��������� ��������� ������ ����������������.
class CAdvancedLogger : public IAdvancedLogger
{
public:

	/// @name �����������/����������
	/// @{
	CAdvancedLogger();
	~CAdvancedLogger();
	/// @}

	virtual HRESULT SetOutputDestination(IN bool To_File, IN bool To_DB, IN bool Message_Box);		/**<
																* \brief ��������� ����������� ��� ����������������
																*
																* @param To_File - ���������������� � ����
																* @param To_DB - ���������������� � ���� ������
																* @param Message_Box - ����� ���� � ��������� ����������
																*/
	virtual HRESULT SetOutputFileParams(IN std::string FileName/*, PARAMSS?*/);	/**<
														* \brief  ��������� ���������� ��� ���������������� � ����
														*
														* @param FileName - ��� � ���� � ����� ��� ����������������
														*/
	//virtual HRESULT SetDBParams() = 0;	// ��������� ���������� ��

	virtual HRESULT FlushData();	/**<
									* \brief �������� ����������� ������ � ��������� ����������
									*
									*/

	virtual HRESULT AppendData(std::string InputData);	/**<
													* \brief ���������� ������ � ������
													*
													* @param InputData - ������ ��� ����������������
													*/
private:

	std::vector<std::string> BufferForWriting;	///< ����� ��� ���������� ���������
	std::string FileNameForWriting;			///< ��� ����� ��� ����������������
	CLogDestination LogDestination;		///< ������ CLogDestination, ������� ����������, ���� ����� �������������� ����������������
};
//------------------------------------------------------------------------------