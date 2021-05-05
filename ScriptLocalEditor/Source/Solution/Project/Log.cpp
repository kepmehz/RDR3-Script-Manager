#include "script.h"

bool firstLine = true;

void Log::LogToFile(const char* log)
{
	const std::string fileName = ".\\MenyaBaes.log";

	std::ofstream logFile;
	logFile.open(fileName, std::ios_base::app);
	if (firstLine)
	{
		logFile << "// Menya Baes (build Aug 18 2020, v.1.0.1311.12)" << std::endl;
		firstLine = false;
	}
	logFile << log;
}

void Log::Clear()
{
	const std::string path = ".\\MenyaBaes.log";
	std::filesystem::remove(path.c_str());
}

const std::string GetTimeFormatted()
{
	struct tm timeStruct;
	time_t currTime = time(NULL);
	localtime_s(&timeStruct, &currTime);

	char buff[48];
	sprintf_s(buff, "|%02d:%02d:%02d|", timeStruct.tm_hour, timeStruct.tm_min, timeStruct.tm_sec);
	return buff;
}

void Log::Print(const char* fmt, ...)
{
	char buf[2048] = { 0 };
	va_list va_alist;

	va_start(va_alist, fmt);
	vsprintf_s(buf, fmt, va_alist);
	va_end(va_alist);

	char buff[2048] = { 0 };
	sprintf_s(buff, "%s |Msg| %s\n", GetTimeFormatted().c_str(), buf);
}

void Log::Debug(const char* fmt, ...)
{
	char buf[2048] = { 0 };
	va_list va_alist;

	va_start(va_alist, fmt);
	vsprintf_s(buf, fmt, va_alist);
	va_end(va_alist);

	char buff[2048] = { 0 };
	sprintf_s(buff, "%s |Dbg| %s\n", GetTimeFormatted().c_str(), buf);
}

void Log::Warning(const char* fmt, ...)
{
	char buf[2048] = { 0 };
	va_list va_alist;

	va_start(va_alist, fmt);
	vsprintf_s(buf, fmt, va_alist);
	va_end(va_alist);

	char buff[2048] = { 0 };
	sprintf_s(buff, "%s |Wrn| %s\n", GetTimeFormatted().c_str(), buf);
}

void Log::Error(const char* fmt, ...)
{
	char buf[2048] = { 0 };
	va_list va_alist;

	va_start(va_alist, fmt);
	vsprintf_s(buf, fmt, va_alist);
	va_end(va_alist);

	char buff[2048] = { 0 };
	sprintf_s(buff, "%s |Err| %s\n", GetTimeFormatted().c_str(), buf);
}