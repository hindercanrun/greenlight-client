#pragma once

namespace Symbols
{
	// Functions
	typedef void (*Cbuf_AddText_t)(Structs::LocalClientNum localClientNum, const char* text);
	extern Cbuf_AddText_t Cbuf_AddText;

	typedef void (*Cbuf_InsertText_t)(Structs::LocalClientNum localClientNum, const char* text);
	extern Cbuf_InsertText_t Cbuf_InsertText;

	typedef int (*Cmd_Argc_t)();
	extern Cmd_Argc_t Cmd_Argc;

	typedef const char* (*Cmd_Argv_t)(int value);
	extern Cmd_Argv_t Cmd_Argv;

	typedef void (*Cmd_AddCommand_t)(const char* cmdName, void (__cdecl* function)(), Structs::cmd_function_s* allocedCmd);
	extern Cmd_AddCommand_t Cmd_AddCommand;

	typedef void (*Cmd_ExecuteSingleCommand_t)(Structs::LocalClientNum localClientNum, Structs::ControllerIndex controllerIndex, const char* text);
	extern Cmd_ExecuteSingleCommand_t Cmd_ExecuteSingleCommand;

	typedef void (*Com_Printf_t)(int channel, const char* fmt, ...);
	extern Com_Printf_t Com_Printf;

	typedef void (*Com_PrintWarning_t)(int channel, const char* fmt, ...);
	extern Com_PrintWarning_t Com_PrintWarning;

	typedef void (*Com_PrintError_t)(int channel, const char* fmt, ...);
	extern Com_PrintError_t Com_PrintError;

	typedef int (*Com_sprintf_t)(char* dest, unsigned int size, const char* fmt, ...);
	extern Com_sprintf_t Com_sprintf;

	typedef int (*Com_HashKey_t)(const char* string, int maxlen);
	extern Com_HashKey_t Com_HashKey;

	typedef Structs::XAssetHeader (*DB_FindXAssetHeader_t)(Structs::XAssetType type, const char* name, bool errorIfMissing, int waitTime);
	extern DB_FindXAssetHeader_t DB_FindXAssetHeader;

	typedef int (*DB_GetAllXAssetOfType_FastFile_t)(Structs::XAssetType type, Structs::XAssetHeader* assets, int maxCount);
	extern DB_GetAllXAssetOfType_FastFile_t DB_GetAllXAssetOfType_FastFile;

	typedef const Structs::dvar_t* (*Dvar_FindVar_t)(const char* dvar);
	extern Dvar_FindVar_t Dvar_FindVar;

	typedef int (*FS_FOpenTextFileWrite_t)(const char* filename);
	extern FS_FOpenTextFileWrite_t FS_FOpenTextFileWrite;

	typedef const char* (*SL_ConvertToString_t)(unsigned int stringValue);
	extern SL_ConvertToString_t SL_ConvertToString;

	typedef DWORD (*Sys_Milliseconds_t)();
	extern Sys_Milliseconds_t Sys_Milliseconds;

	typedef void (*R_AddCmdDrawStretchPic_t)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, const float* colour, Structs::Material* material);
	extern R_AddCmdDrawStretchPic_t R_AddCmdDrawStretchPic;

	typedef void (*R_AddCmdDrawText_t)(const char* text, int maxChars, Structs::Font_s* font, float x, float y, float xScale, float yScale, float rotation, const float* colour, int style);
	extern R_AddCmdDrawText_t R_AddCmdDrawText;

	// Variables
	extern Structs::CG_PerfInfo* cg_perfInfo;

	extern int* logfile;
}