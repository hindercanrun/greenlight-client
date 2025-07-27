#pragma once

namespace Symbols
{
	// Functions
	typedef void (*Cbuf_AddText_t)(int localClientNum, const char* text);
	extern Cbuf_AddText_t Cbuf_AddText;

	typedef void (*Cbuf_InsertText_t)(int localClientNum, const char* text);
	extern Cbuf_InsertText_t Cbuf_InsertText;

	typedef int (*Cmd_Argc_t)();
	extern Cmd_Argc_t Cmd_Argc;

	typedef const char* (*Cmd_Argv_t)(int value);
	extern Cmd_Argv_t Cmd_Argv;

	typedef void (*Cmd_AddCommand_t)(const char* cmdName, void (__cdecl* function)(), Structs::cmd_function_s* allocedCmd);
	extern Cmd_AddCommand_t Cmd_AddCommand;

	typedef void (*Cmd_ExecuteSingleCommand_t)(int localClientNum, int controllerIndex, const char* text);
	extern Cmd_ExecuteSingleCommand_t Cmd_ExecuteSingleCommand;

	typedef Structs::cmd_function_s* (*Cmd_FindCommand_t)(const char* cmdName);
	extern Cmd_FindCommand_t Cmd_FindCommand;

	typedef void (*Com_Error_t)(Structs::errorParm_t code, const char* fmt, ...);
	extern Com_Error_t Com_Error;

	typedef void (*Com_Printf_t)(int channel, const char* fmt, ...);
	extern Com_Printf_t Com_Printf;

	typedef void (*Com_PrintWarning_t)(int channel, const char* fmt, ...);
	extern Com_PrintWarning_t Com_PrintWarning;

	typedef void (*Com_PrintError_t)(int channel, const char* fmt, ...);
	extern Com_PrintError_t Com_PrintError;

	typedef int (*Com_sprintf_t)(char* dest, unsigned int size, const char* fmt, ...);
	extern Com_sprintf_t Com_sprintf;

	typedef Structs::XAssetHeader (*DB_FindXAssetHeader_t)(Structs::XAssetType type, const char* name, bool errorIfMissing, int waitTime);
	extern DB_FindXAssetHeader_t DB_FindXAssetHeader;

	typedef const Structs::dvar_t* (*Dvar_FindVar_t)(const char* dvar);
	extern Dvar_FindVar_t Dvar_FindVar;

	typedef const Structs::dvar_t* (*Dvar_RegisterBool_t)(const char* dvar, bool value, unsigned __int16 flags, const char* description);
	extern Dvar_RegisterBool_t Dvar_RegisterBool;

	typedef const Structs::dvar_t* (*Dvar_RegisterInt_t)(const char* dvar, int value, int min, int max, unsigned __int16 flags, const char* description);
	extern Dvar_RegisterInt_t Dvar_RegisterInt;

	typedef const Structs::dvar_t* (*Dvar_RegisterInt64_t)(const char* dvar, __int64 value, __int64 min, __int64 max, unsigned __int16 flags, const char* description);
	extern Dvar_RegisterInt64_t Dvar_RegisterInt64;

	typedef const Structs::dvar_t* (*Dvar_RegisterFloat_t)(const char* dvar, float value, float min, float max, unsigned __int16 flags, const char* description);
	extern Dvar_RegisterFloat_t Dvar_RegisterFloat;

	typedef const Structs::dvar_t* (*Dvar_RegisterVec2_t)(const char* dvar, int x, int y, float min, float max, unsigned __int16 flags, const char* description);
	extern Dvar_RegisterVec2_t Dvar_RegisterVec2;

	typedef const Structs::dvar_t* (*Dvar_RegisterVec3_t)(const char* dvar, int x, int y, int z, float min, float max, unsigned __int16 flags, const char* description);
	extern Dvar_RegisterVec3_t Dvar_RegisterVec3;

	typedef const Structs::dvar_t* (*Dvar_RegisterVec4_t)(const char* dvar, int x, int y, int z, int w, float min, float max, unsigned __int16 flags, const char* description);
	extern Dvar_RegisterVec4_t Dvar_RegisterVec4;

	typedef const Structs::dvar_t* (*Dvar_RegisterString_t)(const char* dvar, const char* value, unsigned __int16 flags, const char* description);
	extern Dvar_RegisterString_t Dvar_RegisterString;

	typedef const Structs::dvar_t* (*Dvar_RegisterEnum_t)(const char* dvar, const char** value, int defaultIndex, unsigned __int16 flags, const char* description);
	extern Dvar_RegisterEnum_t Dvar_RegisterEnum;

	typedef const Structs::dvar_t* (*Dvar_RegisterColor_t)(const char* dvar, float r, float g, float b, float a, unsigned __int16 flags, const char* description);
	extern Dvar_RegisterColor_t Dvar_RegisterColor;

	typedef const Structs::dvar_t* (*Dvar_RegisterLinearRGB_t)(const char* dvar, int x, int y, int z, float min, float max, unsigned __int16 flags, const char* description);
	extern Dvar_RegisterLinearRGB_t Dvar_RegisterLinearRGB;

	typedef const Structs::dvar_t* (*Dvar_RegisterColorXYZ_t)(const char* dvar, int x, int y, int z, float min, float max, unsigned __int16 flags, const char* description);
	extern Dvar_RegisterColorXYZ_t Dvar_RegisterColorXYZ;

	typedef unsigned int (*FS_Write_t)(char* buffer, unsigned int len, int h);
	extern FS_Write_t FS_Write;

	typedef unsigned int (*FS_Read_t)(const char* buffer, unsigned int len, int h);
	extern FS_Read_t FS_Read;

	typedef int (*FS_ReadFile_t)(const char* qpath, void** buffer);
	extern FS_ReadFile_t FS_ReadFile;

	typedef int (*FS_FOpenFileRead_t)(const char* filename, int* file);
	extern FS_FOpenFileRead_t FS_FOpenFileRead;

	typedef int (*FS_FOpenFileByMode_t)(const char* qpath, int* f, Structs::fsMode_t mode);
	extern FS_FOpenFileByMode_t FS_FOpenFileByMode;

	typedef int (*FS_FOpenTextFileWrite_t)(const char* filename);
	extern FS_FOpenTextFileWrite_t FS_FOpenTextFileWrite;

	typedef int (*FS_FOpenFileAppend_t)(const char* filename);
	extern FS_FOpenFileAppend_t FS_FOpenFileAppend;

	typedef void (*FS_FCloseFile_t)(int h);
	extern FS_FCloseFile_t FS_FCloseFile;

	typedef void (*LUI_CoD_Init_t)(bool frontend);
	extern LUI_CoD_Init_t LUI_CoD_Init;

	typedef void (*LUI_CoD_Shutdown_t)();
	extern LUI_CoD_Shutdown_t LUI_CoD_Shutdown;

	typedef DWORD (*Sys_Milliseconds_t)();
	extern Sys_Milliseconds_t Sys_Milliseconds;

	typedef void (*R_AddCmdDrawStretchPic_t)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, const float* colour, Structs::Material* material);
	extern R_AddCmdDrawStretchPic_t R_AddCmdDrawStretchPic;

	typedef void (*R_AddCmdDrawText_t)(const char* text, int maxChars, Structs::Font_s* font, float x, float y, float xScale, float yScale, float rotation, const float* colour, int style);
	extern R_AddCmdDrawText_t R_AddCmdDrawText;

	typedef int (*R_TextWidth_t)(const char* text, int maxChars, Structs::Font_s* font);
	extern R_TextWidth_t R_TextWidth;

	typedef char* (*I_strlwr_t)(char* s);
	extern I_strlwr_t I_strlwr;

	typedef void (*I_strncpyz_t)(char* dest, const char* src, int destsize);
	extern I_strncpyz_t I_strncpyz;

	typedef char* (*I_strlwr_t)(char* s);
	extern I_strlwr_t I_strlwr;

	typedef int (*I_stricmp_t)(const char* s0, const char* s1);
	extern I_stricmp_t I_stricmp;

	typedef void (*I_strncat_t)(char* dest, int size, const char* src);
	extern I_strncat_t I_strncat;

	// Variables
	extern Structs::CG_PerfInfo* cg_perfInfo;

	extern Structs::ScreenPlacement* scrPlaceFullUnsafe;
}