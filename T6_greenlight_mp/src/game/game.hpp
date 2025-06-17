#pragma once

namespace game
{
	// the functions below are either inlined or removed from the game exe

	int Com_Compress(char* data_p);

	void* Z_MallocGarbage(int size, const char *name, int type);
	void Z_Free(char *ptr, int type);
}

typedef struct _XEX_EXECUTION_ID
{
	DWORD MediaID;
	DWORD Version;
	DWORD BaseVersion;

	union
	{
		DWORD TitleID;

		struct
		{
			WORD PublisherID;
			WORD GameID;
		};
	};

	BYTE Platform;
	BYTE ExecutableType;
	BYTE DiscNum;
	BYTE DiscsInSet;
	DWORD SaveGameID;
} XEX_EXECUTION_ID, *PXEX_EXECUTION_ID;

C_ASSERT(sizeof(XEX_EXECUTION_ID) == 0x18);
