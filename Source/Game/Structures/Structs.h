#pragma once

namespace Structs
{
	struct cmd_function_s
	{
		cmd_function_s* next;

		const char* name;
		const char* autoCompleteDir;
		const char* autoCompleteExt;
		void(__cdecl* function)();
	};

	enum errorParm_t : __int32
	{
		ERR_FATAL = 0x0,
		ERR_DROP = 0x1,
		ERR_SERVERDISCONNECT = 0x2,
		ERR_DISCONNECT = 0x3,
		ERR_SCRIPT = 0x4,
		ERR_SCRIPT_DROP = 0x5,
		ERR_LOCALIZATION = 0x6,
		ERR_MAPLOADERRORSUMMARY = 0x7,
	};

	enum DvarFlags : std::uint16_t
	{
		DVAR_NOFLAG = 0, // No flags
		DVAR_ARCHIVE = 1 << 0, // Save to config.cfg
		DVAR_LATCH = 1 << 1, // Unchange-able unless if game does a Dvar_Get()
		DVAR_CHEAT = 1 << 2, // Can not be changed if cheats are disabled
		DVAR_CODINFO = 1 << 3, // On change, this is sent to all clients (if you are host)
		DVAR_SCRIPTINFO = 1 << 4, // Unknown
		DVAR_TEMP = 1 << 5, // You can probably guess what this does
		DVAR_SAVED = 1 << 6, // Unknown
		DVAR_INTERNAL = 1 << 7, // You can probably guess what this does
		DVAR_EXTERNAL = 1 << 8, // Created by set command
		DVAR_USERINFO = 1 << 9, // Sent to server on connect or change
		DVAR_SERVERINFO = 1 << 10, // Sent in response to front end requests
		DVAR_INIT = 1 << 11, // Don't allow change from console at all
		DVAR_SYSTEMINFO = 1 << 12, // Duplicated on all clients
		DVAR_ROM = 1 << 13, // Unchange-able, cannot be set by user at all
		DVAR_CHANGEABLE_RESET = 1 << 14, // Unknown
		DVAR_AUTOEXEC = 1 << 15, // Never be set by game since isLoadingAutoExecGlobalFlag is always false
	};

	enum dvarType_t : __int32
	{
		DVAR_TYPE_BOOL = 0x0,
		DVAR_TYPE_FLOAT = 0x1,
		DVAR_TYPE_FLOAT_2 = 0x2,
		DVAR_TYPE_FLOAT_3 = 0x3,
		DVAR_TYPE_FLOAT_4 = 0x4,
		DVAR_TYPE_INT = 0x5,
		DVAR_TYPE_ENUM = 0x6,
		DVAR_TYPE_STRING = 0x7,
		DVAR_TYPE_COLOR = 0x8,
		DVAR_TYPE_INT64 = 0x9,
		DVAR_TYPE_LINEAR_COLOR_RGB = 0xA,
		DVAR_TYPE_COLOR_XYZ = 0xB,
		DVAR_TYPE_COUNT = 0xC,
	};

	union DvarValue
	{
		bool enabled;
		int integer;
		unsigned int unsignedInt;
		__int64 integer64;
		unsigned __int64 unsignedInt64;
		float value;
		float vector[4];
		const char* string;
		unsigned __int8 color[4];
	};

	struct dvar_t
	{
		const char* name;
		const char* description;
		int hash;
		unsigned int flags;
		dvarType_t type;

		bool modified;
		bool loadedFromSaveGame;

		DvarValue current;
		DvarValue latched;
		DvarValue reset;
		DvarValue saved;

		union domain
		{
			struct enumeration
			{
				int stringCount;
				const char** strings;
			};

			struct integer
			{
				int min;
				int max;
			};

			struct integer64
			{
				__int64 min;
				__int64 max;
			};

			struct value
			{
				float min;
				float max;
			};

			struct vector
			{
				float min;
				float max;
			};
		};

		dvar_t* hashNext;
	};

	enum bdLogMessageType : __int32
	{
		BD_LOG_INFO = 0x0,
		BD_LOG_WARNING = 0x1,
		BD_LOG_ERROR = 0x2,
	};

	enum scriptInstance_t : __int32
	{
		SCRIPTINSTANCE_SERVER = 0x0,
		SCRIPTINSTANCE_CLIENT = 0x1,
		SCRIPT_INSTANCE_MAX = 0x2,
	};

	struct fileRetryInfo
	{
		int lastAttemptTime;
		int lastAttemptInterval;
		int retryCount;
	};

	struct ScreenPlacement
	{
		float scaleVirtualToReal[2];
		float scaleVirtualToFull[2];
		float scaleRealToVirtual[2];
		float virtualViewableMin[2];
		float virtualViewableMax[2];
		float virtualTweakableMin[2];
		float virtualTweakableMax[2];
		float realViewportBase[2];
		float realViewportSize[2];
		float realViewportMid[2];
		float realViewableMin[2];
		float realViewableMax[2];
		float realTweakableMin[2];
		float realTweakableMax[2];
		float subScreen[2];
	};

	struct CG_PerfData
	{
		int history[32];
		int count;
		int index;
		int instant;
		int total;
		float average;
		float variance;
		int min;
		int max;
	};

	struct CG_PerfInfo
	{
		bool initialized;
		CG_PerfData frame;
		CG_PerfData script;
		CG_PerfData cscript;
		CG_PerfData server;
		CG_PerfData renderExec;
		CG_PerfData renderSwap;
	};

	enum fsMode_t : __int32
	{
		FS_READ = 0x0,
		FS_WRITE = 0x1,
		FS_APPEND = 0x2,
		FS_APPEND_SYNC = 0x3,
	};

	enum FsThread : __int32
	{
		FS_THREAD_MAIN = 0x0,
		FS_THREAD_STREAM = 0x1,
		FS_THREAD_DATABASE = 0x2,
		FS_THREAD_BACKEND = 0x3,
		FS_THREAD_SERVER = 0x4,
		FS_THREAD_COUNT = 0x5,
		FS_THREAD_INVALID = 0x6,
	};
}
