#pragma once

class Log
{
	static void LogToFile(const char* log);
	static void Clear();

	static void Print(const char* fmt, ...);
	static void Debug(const char* fmt, ...);

	static void Warning(const char* fmt, ...);
	static void Error(const char* fmt, ...);
};