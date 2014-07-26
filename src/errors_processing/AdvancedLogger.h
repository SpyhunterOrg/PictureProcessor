//------------------------------------------------------------------------------
// File: CAdvancedLogger.h
//------------------------------------------------------------------------------

//! Классы для протоколирования.
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
* Данный класс предназначен для получения и хранения пути к файлам протоколирования.
*/
/// Класс предназначен для получения и хранения пути к файлам протоколирования.
//------------------------------------------------------------------------------
class COutputLogFilePath
{
public:
	COutputLogFilePath(IN const std::string & FileName);	// В конструкторе пытаемся считать путь к файлу из реестра
	std::string GetCurrentProcessSubfolder();	/**<
											* \brief Получение пути текущего процесса
											*
											* @return путь, из которого было запущено текущее приложение
											*/

	std::string LogFileOutput;	///< Путь к файлу протоколирования
};
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* @class CLogDestination 
* Данный класс предназначен для сохранения получателей, в которые будет осуществляться протоколирование.
* Позволяет установить получателей:
<ul>
<li>Файл;
<li>База данных;
<li>Окно с текстовым сообщением.
</ul>
*/
///Класс предназначен для сохранения получателей, в которые будет осуществляться протоколирование.
class CLogDestination	// Descriptor of destination of logging
{
public:

	/// @name Конcтрукторы
	/// @{
	CLogDestination(){ ToFile = FALSE; ToDB = FALSE; MsgBox = FALSE; }	///< Конструктор по умолчанию
	CLogDestination(bool ToFil, bool ToDB, bool MessBox)
	{
		this->SetToFile(ToFil);
		this->SetToDB(ToDB);
		this->SetMsgBox(MessBox);
	}	/**<
		* \brief Создание объекта CLogDestination с инициализацией получателей для протоколирования
		*
		* @param ToFil - протоколирование в файл
		* @param ToDB - протоколирование в базу данных
		* @param MessBox - вывод окна с текстовым сообщением
		*/
	/// @}

	bool GetToDB(){ return ToDB;}	/**<
									* \brief Получение параметра, определяющего возможность протоколирования в файл
									*
									* @return Возможность или невозможность протоколирования в файл
									*/

	bool GetToFile(){ return ToFile;}/**<
									* \brief Получение параметра, определяющего возможность протоколирования в базу данных
									*
									* @return Возможность или невозможность протоколирования в базу данных
									*/

	bool GetMsgBox() { return MsgBox;}/**<
									* \brief Получение параметра, определяющего возможность вывода сообщений об исключительных ситуациях в виде окна с текстовым сообщением
									*
									* @return Возможность или невозможность вывода сообщений об исключительных ситуациях в виде окна с текстовым сообщением
									*/

	void SetToDB(bool val){ToDB = val;}/**<
									* \brief Установка параметра, определяющего возможность протоколирования в базу данных
									*
									* @param val - Возможность или невозможность протоколирования в базу данных
									*/

	void SetToFile(bool val){ ToFile = val;}/**<
									* \brief Установка параметра, определяющего возможность протоколирования в файл
									*
									* @param val - Возможность или невозможность протоколирования в файл
									*/
	void SetMsgBox(bool val){ MsgBox = val; }/**<
									* \brief Установка параметра, определяющего возможность вывода сообщений об исключительных ситуациях в виде окна с текстовым сообщением
									*
									* @param val - Возможность или невозможность вывода сообщений об исключительных ситуациях в виде окна с текстовым сообщением
									*/

	void clear(){ ToDB = FALSE; ToFile = FALSE; MsgBox = FALSE;}	/**<
																	* \brief Установка исходных значений объекта
																	*
																	*/

private:
	bool ToFile;	///< Возможность или невозможность протоколирования в файл
	bool ToDB;		///< Возможность или невозможность протоколирования в базу данных
	bool MsgBox;	///< Возможность или невозможность вывода сообщений об исключительных ситуациях в виде окна с текстовым сообщением
};
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
* @interface IAdvancedLogger 
* Интерфейс класса CAdvancedLogger, который реализует различные методы протоколирования.
*
*/
///Интерфейс класса CAdvancedLogger, который реализует различные методы протоколирования.
class IAdvancedLogger
{
public:


	/// @name Деструктор
	/// @{
	virtual ~IAdvancedLogger() = 0;
	/// @}

	// ------Инициализация------
	virtual HRESULT SetOutputDestination(IN bool To_File, IN bool To_DB, IN bool Message_Box) = 0;	/**<
										* \brief Установка получателей для протоколирования
										*
										* @param To_File - протоколирование в файл
										* @param To_DB - протоколирование в базу данных
										* @param Message_Box - вывод окна с текстовым сообщением
										*/

	virtual HRESULT SetOutputFileParams(IN std::string FileName/*, PARAMSS?*/) = 0;	/**<
														* \brief  Установка параметров для протоколирования в файл
														*
														* @param FileName - Имя и путь к файлу для протоколирования
														*/
	//virtual HRESULT SetDBParams() = 0;	// Установка параметров БД

	// ------Работа с данными------
	virtual HRESULT FlushData() = 0;	/**<
										* \brief Выгрузка накопленных данных в указанные получатели
										*
										*/

	virtual HRESULT AppendData(std::string InputData) = 0;	/**<
														* \brief Добавление данных в объект
														*
														* @param InputData - данные для протоколирования
														*/
	//virtual HRESULT AppendData(UINT InputData) = 0;		// Добавление данных в объект
	//virtual HRESULT AppendData(int InputData) = 0;		// Добавление данных в объект
	//virtual HRESULT AppendData(char * InputData, UINT InputDataLength) = 0;	// Добавление данных в объект

	//virtual HRESULT DirectWriteData(std::string InputData) = 0;	// Прямая запись данных
	//virtual HRESULT DirectWriteData(UINT InputData) = 0;	// Прямая запись данных
	//virtual HRESULT DirectWriteData(int InputData) = 0;		// Прямая запись данных
	//virtual HRESULT DirectWriteData(char * InputData, UINT InputDataLength) = 0;	// Прямая запись данных

};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/**
* @class CAdvancedLogger 
* Класс реализует различные методы протоколирования.
*
*/
///Класс реализует различные методы протоколирования.
class CAdvancedLogger : public IAdvancedLogger
{
public:

	/// @name Конструктор/Деструктор
	/// @{
	CAdvancedLogger();
	~CAdvancedLogger();
	/// @}

	virtual HRESULT SetOutputDestination(IN bool To_File, IN bool To_DB, IN bool Message_Box);		/**<
																* \brief Установка получателей для протоколирования
																*
																* @param To_File - протоколирование в файл
																* @param To_DB - протоколирование в базу данных
																* @param Message_Box - вывод окна с текстовым сообщением
																*/
	virtual HRESULT SetOutputFileParams(IN std::string FileName/*, PARAMSS?*/);	/**<
														* \brief  Установка параметров для протоколирования в файл
														*
														* @param FileName - Имя и путь к файлу для протоколирования
														*/
	//virtual HRESULT SetDBParams() = 0;	// Установка параметров БД

	virtual HRESULT FlushData();	/**<
									* \brief Выгрузка накопленных данных в указанные получатели
									*
									*/

	virtual HRESULT AppendData(std::string InputData);	/**<
													* \brief Добавление данных в объект
													*
													* @param InputData - данные для протоколирования
													*/
private:

	std::vector<std::string> BufferForWriting;	///< Буфер для накопления сообщений
	std::string FileNameForWriting;			///< Имя файла для протоколирования
	CLogDestination LogDestination;		///< Объект CLogDestination, который определяет, куда будет осуществляться протоколирование
};
//------------------------------------------------------------------------------