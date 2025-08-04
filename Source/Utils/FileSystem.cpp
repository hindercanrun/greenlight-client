#include "FileSystem.h"

namespace Utils
{
	namespace FileSystem
	{
		// Taken from: https://github.com/michaeloliverx/iw3xe/blob/main/src/filesystem.cpp
		// Modified to my own liking.

		void CreateNestedDirectories(const char* path)
		{
			if (!path || !path[0])
			{
				return;
			}

			char tempPath[256];
			size_t len = strlen(path);
			if (len >= sizeof(tempPath))
			{
				return;
			}

			strcpy(tempPath, path);

			char* p = tempPath;

			// Skip drive letter or known prefix like "game:\"
			if ((p[0] && p[1] == ':' && (p[2] == '\\' || p[2] == '/')))
			{
				p += 3;
			}
			else if (strncmp(p, "game:\\", 6) == 0)
			{
				p += 6;
			}

			for (; p[0]; ++p)
			{
				if (p[0] == '\\' || p[0] == '/')
				{
					char saved = p[0];
					p[0] = '\0';
					mkdir(tempPath);
					p[0] = saved;
				}
			}

			mkdir(tempPath); // Final directory
		}

		bool WriteFileToDisk(const char* filePath, const char* data, size_t dataSize)
		{
			if (!filePath || !data || dataSize == 0)
			{
				printf("WriteFileToDisk: Invalid parameters!\n");
				return FALSE;
			}

			// Extract directory path
			char dirPath[256];
			strncpy(dirPath, filePath, sizeof(dirPath) - 1);
			dirPath[sizeof(dirPath) - 1] = '\0';

			char* lastSlash = strrchr(dirPath, '\\');
			if (lastSlash)
			{
				lastSlash[0] = '\0';
				CreateNestedDirectories(dirPath);
			}

			// Write data
			FILE* file = fopen(filePath, "wb");
			if (file)
			{
				fwrite(data, 1, dataSize, file);
				fclose(file);

				printf("Successfully wrote file: '%s'\n", filePath);
				return TRUE;
			}
			else
			{
				printf("Failed to write file: '%s'\n", filePath);
				return FALSE;
			}
		}

		char* ReadFileToString(const char* filePath)
		{
			// Read data
			FILE* file = fopen(filePath, "rb");
			if (!file)
			{
				printf("ReadFileToString: Failed to open file: %s\n", filePath);
				return NULL;
			}

			fseek(file, 0, SEEK_END);
			long size = ftell(file);
			rewind(file);

			char* buffer = (char*)malloc(size + 1);
			if (!buffer)
			{
				fclose(file);
				printf("ReadFileToString: Failed to allocate memory for file: %s\n", filePath);
				return NULL;
			}

			fread(buffer, 1, size, file);
			buffer[size] = '\0';

			fclose(file);
			return buffer;
		}

		bool FileExists(const char* filePath)
		{
			// Read data
			FILE* file = fopen(filePath, "r");
			if (file)
			{
				fclose(file);
				return TRUE;
			}
			return FALSE;
		}
	}
}