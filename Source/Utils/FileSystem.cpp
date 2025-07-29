#include "../Std_Include.h"
#include "Hook.h"

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
				Symbols::Com_Printf(9, "WriteFileToDisk: Invalid parameters!\n");
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

				Symbols::Com_Printf(9, "Successfully wrote file: '%s'\n", filePath);
				return TRUE;
			}
			else
			{
				Symbols::Com_Printf(9, "Failed to write file: '%s'\n", filePath);
				return FALSE;
			}
		}
	}
}