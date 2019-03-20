#include <Windows.h>
#include <string>
#include <comdef.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "Logger.h"

#define CONVERT_STR(var) #var
const std::string DEBUG_FILE_NAME = "./DebugLog";

namespace K3D12{
	struct ConsoleScreen
	{
		CONSOLE_SCREEN_BUFFER_INFO  ScreenBuffer;

		void BindColor(LogLevel level);
		void UnBindColor();
	};

	void ConsoleScreen::BindColor(LogLevel level)
	{
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(handle, &ScreenBuffer);

		unsigned short attribute = ScreenBuffer.wAttributes;
		switch (level)
		{
		case LogLevel::Details:
			attribute = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
			break;

		case LogLevel::Info:
			attribute = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			break;

		case LogLevel::Debug:
			attribute = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			break;

		case LogLevel::Warning:
			attribute = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
			break;

		case LogLevel::Error:
			attribute = FOREGROUND_RED | FOREGROUND_INTENSITY;
			break;
		}

		SetConsoleTextAttribute(handle, attribute);

	}
	void ConsoleScreen::UnBindColor() {
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(handle, ScreenBuffer.wAttributes);
	}
}


K3D12::SystemLogger::SystemLogger() : _LogFilter(LogLevel::Details)
{
#ifdef _DEBUG
	CreateDebugFile();

#endif // _DEBUG

	CreateDebugFile();

}


K3D12::SystemLogger::~SystemLogger()
{
#ifdef _DEBUG
	UnMap();
#endif // _DEBUG
}


void K3D12::SystemLogger::CreateDebugFile()
{
	time_t localTime = time(nullptr);
	std::string ltt;
	tm lt;
	localtime_s(&lt, &localTime);
	std::stringstream ss;
	ss << DEBUG_FILE_NAME.c_str();
	ss << ".txt";
	std::string res(ss.str());

	_debugFile = std::ofstream(res.c_str());

	Map(res);
}

void K3D12::SystemLogger::Map(std::string fileName)
{
#ifdef _DEBUG
	if (this->_debugFile.is_open()) {
		_debugFile.close();
	}
	this->_debugFile.open(fileName.c_str(), std::ios::out | std::ios::ate);
#endif // _DEBUG
}

void K3D12::SystemLogger::UnMap()
{
#ifdef _DEBUG
	if (this->_debugFile.is_open()) {
		_debugFile.close();
	}
#endif // _DEBUG
}

void K3D12::SystemLogger::Log(LogLevel level , HRESULT hr)
{
#ifdef _DEBUG
	if (level >= _LogFilter)
	{
		_com_error err(hr);
		LPCTSTR errMsg = err.ErrorMessage();
		ConsoleScreen screen;
		screen.BindColor(level);
		OutputDebugStringW(errMsg);
		screen.UnBindColor();
		_debugFile << errMsg;
	}
#endif
}

void K3D12::SystemLogger::Log(LogLevel level, std::string format)
{
#ifdef _DEBUG
	Log(level, format.c_str());
#endif // _DEBUG
}

void K3D12::SystemLogger::Log(LogLevel level, const char * format, ...)
{
	
#ifdef _DEBUG

	if (level >= _LogFilter)
	{
		ConsoleScreen screen;
		// ログ出力.
		screen.BindColor(level);
		{
			char msg[2048] = "\0";
			va_list arg;

			va_start(arg, format);
			vsprintf_s(msg, format, arg);
			va_end(arg);

			printf_s("%s", msg);

			OutputDebugStringA(msg);
		}
		screen.UnBindColor();
	}
#endif	
}

void K3D12::SystemLogger::LogW(LogLevel level, std::wstring format)
{
#ifdef _DEBUG

	LogW(level, format.c_str());
#endif
}

void K3D12::SystemLogger::LogW(LogLevel level, const wchar_t * format, ...)
{
#ifdef _DEBUG

	if (level >= _LogFilter)
	{
		ConsoleScreen screen;
		screen.BindColor(level);
		// ログ出力.
		{
			wchar_t msg[2048] = L"\0";
			va_list arg;

			va_start(arg, format);
			vswprintf_s(msg, format, arg);
			va_end(arg);

			wprintf_s(L"%s", msg);

			OutputDebugStringW(msg);
			_debugFile << msg;
		}
		screen.UnBindColor();
	}
#endif
}

void K3D12::SystemLogger::SetFilter(LogLevel level)
{
	_LogFilter = level;
}

K3D12::LogLevel K3D12::SystemLogger::GetFilter()
{
	return _LogFilter;
}

void K3D12::SystemLogger::SetDebugFilePath(std::string filePath)
{
}

K3D12::SystemLogger & K3D12::SystemLogger::GetInstance()
{
	 
	static K3D12::SystemLogger instance;
	return instance;
}


#ifdef _DEBUG
#define LOG(message,LogLevel) SystemLogger::GetInstance.Log(LogLevel,"%s","%d","%s" ,__FILE__,__LINE__, message.c_str())
#endif // _DEBUG

#ifdef _DEBUG
	#define LOGW(message,LogLevel) SystemLogger::GetInstance.LogW(LogLevel,"%ls","%ld","%ls" ,__FILE__,__LINE__, message.c_str())
#endif // _DEBUG
