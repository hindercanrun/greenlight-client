#pragma once

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
