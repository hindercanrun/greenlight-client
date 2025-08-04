#pragma once

namespace Structs
{
	typedef enum LocalClientNum_t
	{
		INVALID_LOCAL_CLIENT		= -1,

		LOCAL_CLIENT_FIRST			= 0,
		LOCAL_CLIENT_0				= 0,
		ONLY_LOCAL_CLIENT			= 0,

		LOCAL_CLIENT_COUNT			= 1
	} LocalClientNum;

	typedef enum ControllerIndex_t
	{
		INVALID_CONTROLLER_PORT		= -1,

		CONTROLLER_INDEX_FIRST		= 0,
		CONTROLLER_INDEX_0			= 0,

		CONTROLLER_INDEX_COUNT		= 1
	} ControllerIndex;

	typedef struct cmd_function_s
	{
		struct cmd_function_s* next;
		const char* name;
		const char* autoCompleteDir;
		const char* autoCompleteExt;
		void (__cdecl* function)(void);
	} cmd_function_t;

	typedef enum errorParm_e
	{
		ERR_FATAL					= 0,
		ERR_DROP,
		ERR_SERVERDISCONNECT,
		ERR_DISCONNECT,
		ERR_SCRIPT,
		ERR_SCRIPT_DROP,
		ERR_LOCALIZATION,
		ERR_MAPLOADERRORSUMMARY
	} errorParm_t;

	typedef enum DvarFlags_e
	{
		DVAR_NOFLAG					= 0,
		DVAR_ARCHIVE				= 1 << 0,
		DVAR_LATCH					= 1 << 1,
		DVAR_CHEAT					= 1 << 2,
		DVAR_CODINFO				= 1 << 3,
		DVAR_SCRIPTINFO				= 1 << 4,
		DVAR_TEMP					= 1 << 5,
		DVAR_SAVED					= 1 << 6,
		DVAR_INTERNAL				= 1 << 7,
		DVAR_EXTERNAL				= 1 << 8,
		DVAR_USERINFO				= 1 << 9,
		DVAR_SERVERINFO				= 1 << 10,
		DVAR_INIT					= 1 << 11,
		DVAR_SYSTEMINFO				= 1 << 12,
		DVAR_ROM					= 1 << 13,
		DVAR_CHANGEABLE_RESET		= 1 << 14,
		DVAR_AUTOEXEC				= 1 << 15
	} DvarFlags;

	typedef enum dvarType_e
	{
		DVAR_TYPE_BOOL = 0,
		DVAR_TYPE_FLOAT,
		DVAR_TYPE_FLOAT_2,
		DVAR_TYPE_FLOAT_3,
		DVAR_TYPE_FLOAT_4,
		DVAR_TYPE_INT,
		DVAR_TYPE_ENUM,
		DVAR_TYPE_STRING,
		DVAR_TYPE_COLOR,
		DVAR_TYPE_INT64,
		DVAR_TYPE_LINEAR_COLOR_RGB,
		DVAR_TYPE_COLOR_XYZ,
		DVAR_TYPE_COUNT
	} dvarType_t;

	typedef union DvarValue_u
	{
		bool enabled;
		int integer;
		unsigned int unsignedInt;
		int64_t integer64;
		uint64_t unsignedInt64;
		float value;
		float vector[4];
		const char* string;
		uint8_t color[4];
	} DvarValue;

	typedef struct dvar_enumeration_s
	{
		int stringCount;
		const char** strings;
	} dvar_enumeration_t;

	typedef struct dvar_integer_s
	{
		int min;
		int max;
	} dvar_integer_t;

	typedef struct dvar_integer64_s
	{
		int64_t min;
		int64_t max;
	} dvar_integer64_t;

	typedef struct dvar_value_s
	{
		float min;
		float max;
	} dvar_value_t;

	typedef struct dvar_vector_s
	{
		float min;
		float max;
	} dvar_vector_t;

	typedef struct dvar_s
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

		union
		{
			dvar_enumeration_t enumeration;
			dvar_integer_t integer;
			dvar_integer64_t integer64;
			dvar_value_t value;
			dvar_vector_t vector;
		} domain;

		struct dvar_s* hashNext;
	} dvar_t;

	typedef struct ScreenPlacement_s
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
	} ScreenPlacement;

	typedef struct CG_PerfData_s
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
	} CG_PerfData;

	typedef struct CG_PerfInfo_s
	{
		bool initialized;
		CG_PerfData frame;
		CG_PerfData script;
		CG_PerfData cscript;
		CG_PerfData server;
		CG_PerfData renderExec;
		CG_PerfData renderSwap;
	} CG_PerfInfo;

	typedef enum fsMode_e
	{
		FS_READ = 0,
		FS_WRITE,
		FS_APPEND,
		FS_APPEND_SYNC
	} fsMode_t;

	typedef enum FsThread_e
	{
		FS_THREAD_MAIN = 0,
		FS_THREAD_STREAM,
		FS_THREAD_DATABASE,
		FS_THREAD_BACKEND,
		FS_THREAD_SERVER,
		FS_THREAD_COUNT,
		FS_THREAD_INVALID
	} FsThread;
}
