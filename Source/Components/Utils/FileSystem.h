#pragma once

namespace Utils
{
	namespace FileSystem
	{
		bool WriteFileToDisk(const char* filePath, const char* data, size_t dataSize);
		char* ReadFileToString(const char* filePath);
		bool FileExists(const char* filePath);
	}
}