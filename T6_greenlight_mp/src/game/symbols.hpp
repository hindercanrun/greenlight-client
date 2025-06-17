#pragma once

namespace game
{
	typedef void(*CG_DrawVersion_t)();
	extern CG_DrawVersion_t CG_DrawVersion;

	typedef void(*Cbuf_AddText_t)(int localClientNum, const char* text);
	extern Cbuf_AddText_t Cbuf_AddText;

	typedef void(*Cbuf_InsertText_t)(int localClientNum, const char* text);
	extern Cbuf_InsertText_t Cbuf_InsertText;

	typedef int(*Cmd_Argc_t)();
	extern Cmd_Argc_t Cmd_Argc;

	typedef const char*(*Cmd_Argv_t)(int value);
	extern Cmd_Argv_t Cmd_Argv;

	typedef void(*Cmd_AddCommandInternal_t)(const char* cmdName, void (__cdecl* function)(), cmd_function_s* allocedCmd);
	extern Cmd_AddCommandInternal_t Cmd_AddCommandInternal;

	typedef void(*Cmd_ExecuteSingleCommand_t)(int localClientNum, int controllerIndex, const char* text);
	extern Cmd_ExecuteSingleCommand_t Cmd_ExecuteSingleCommand;

	typedef cmd_function_s*(*Cmd_FindCommand_t)(const char* cmdName);
	extern Cmd_FindCommand_t Cmd_FindCommand;

	typedef void(*Cmd_SetAutoComplete_t)(const char* cmdName, const char* dir, const char* ext);
	extern Cmd_SetAutoComplete_t Cmd_SetAutoComplete;

	typedef void(*Com_BeginParseSession_t)(const char* filename);
	extern Com_BeginParseSession_t Com_BeginParseSession;

	typedef void(*Com_EndParseSession_t)();
	extern Com_EndParseSession_t Com_EndParseSession;

	typedef char*(*Com_Parse_t)(const char** data_p);
	extern Com_Parse_t Com_Parse;

	typedef void(*Com_Error_t)(errorParm_t code, const char* fmt, ...);
	extern Com_Error_t Com_Error;

	typedef void(*Com_Printf_t)(int channel, const char* fmt, ...);
	extern Com_Printf_t Com_Printf;

	typedef void(*Com_PrintWarning_t)(int channel, const char* fmt, ...);
	extern Com_PrintWarning_t Com_PrintWarning;

	typedef void(*Com_PrintError_t)(int channel, const char* fmt, ...);
	extern Com_PrintError_t Com_PrintError;

	typedef const char*(*va_t)(const char* fmt, ...);
	extern va_t va;

	typedef int(*Com_sprintf_t)(char* dest, unsigned int size, const char* fmt, ...);
	extern Com_sprintf_t Com_sprintf;

	typedef int(*Com_IsAddonMap_t)(const char* mapName, const char** pBaseMapName);
	extern Com_IsAddonMap_t Com_IsAddonMap;

	typedef void(*Com_GetBspFilename_t)(char* filename, int size, const char* mapname);
	extern Com_GetBspFilename_t Com_GetBspFilename;

	typedef const char*(*Com_GetExtensionSubString_t)(const char *filename);
	extern Com_GetExtensionSubString_t Com_GetExtensionSubString;

	typedef unsigned int(*Com_DecompressWithZLib_t)(unsigned __int8* from, unsigned int inSizeBytes, unsigned __int8* to, unsigned int outSizeBytes);
	extern Com_DecompressWithZLib_t Com_DecompressWithZLib;

	typedef unsigned int(*Hunk_AllocateTempMemoryHigh_t)(int size, const char *name);
	extern Hunk_AllocateTempMemoryHigh_t Hunk_AllocateTempMemoryHigh;

	typedef BOOL(*Con_IsActive_t)(int localClientNum);
	extern Con_IsActive_t Con_IsActive;

	typedef BOOL(*Con_IsDvarCommand_t)(const char* cmd);
	extern Con_IsDvarCommand_t Con_IsDvarCommand;

	typedef XAssetHeader(*DB_FindXAssetHeader_t)(XAssetType type, const char* name, bool errorIfMissing, int waitTime);
	extern DB_FindXAssetHeader_t DB_FindXAssetHeader;

	typedef void(*DB_LoadXAssets_t)(XZoneInfo* zoneInfo, unsigned int zoneCount, int sync);
	extern DB_LoadXAssets_t DB_LoadXAssets;

	typedef BOOL(*DevGui_IsActive_t)();
	extern DevGui_IsActive_t DevGui_IsActive;

	typedef dvar_s*(*Dvar_RegisterBool_t)(const char* dvar, bool value, unsigned __int16 flags, const char* description);
	extern Dvar_RegisterBool_t Dvar_RegisterBool;

	typedef dvar_s*(*Dvar_RegisterInt_t)(const char* dvar, int value, int min, int max, unsigned __int16 flags, const char* description);
	extern Dvar_RegisterInt_t Dvar_RegisterInt;

	typedef dvar_s*(*Dvar_RegisterInt64_t)(const char* dvar, __int64 value, __int64 min, __int64 max, unsigned __int16 flags, const char* description);
	extern Dvar_RegisterInt64_t Dvar_RegisterInt64;

	typedef dvar_s*(*Dvar_RegisterFloat_t)(const char* dvar, float value, float min, float max, unsigned __int16 flags, const char* description);
	extern Dvar_RegisterFloat_t Dvar_RegisterFloat;

	typedef dvar_s*(*Dvar_RegisterVec2_t)(const char* dvar, int x, int y,
										  float min, float max, unsigned __int16 flags, const char* description);
	extern Dvar_RegisterVec2_t Dvar_RegisterVec2;

	typedef dvar_s*(*Dvar_RegisterVec3_t)(const char* dvar, int x, int y, int z, float min,
										  float max, unsigned __int16 flags, const char* description);
	extern Dvar_RegisterVec3_t Dvar_RegisterVec3;

	typedef dvar_s*(*Dvar_RegisterVec4_t)(const char* dvar, int x, int y, int z, int w, float min,
										  float max, unsigned __int16 flags, const char* description);
	extern Dvar_RegisterVec4_t Dvar_RegisterVec4;

	typedef dvar_s*(*Dvar_RegisterString_t)(const char* dvar, const char* value, unsigned __int16 flags, const char* description);
	extern Dvar_RegisterString_t Dvar_RegisterString;

	typedef dvar_s*(*Dvar_RegisterEnum_t)(const char* dvar, const char** value, int defaultIndex, unsigned __int16 flags, const char* description);
	extern Dvar_RegisterEnum_t Dvar_RegisterEnum;

	typedef dvar_s*(*Dvar_RegisterColor_t)(const char* dvar, float r, float g, float b,
										   float a, unsigned __int16 flags, const char* description);
	extern Dvar_RegisterColor_t Dvar_RegisterColor;

	typedef dvar_s*(*Dvar_RegisterLinearRGB_t)(const char* dvar, int x, int y, int z, float min,
											  float max, unsigned __int16 flags, const char* description);
	extern Dvar_RegisterLinearRGB_t Dvar_RegisterLinearRGB;

	typedef dvar_s*(*Dvar_RegisterColorXYZ_t)(const char* dvar, int x, int y, int z, float min,
											  float max, unsigned __int16 flags, const char* description);
	extern Dvar_RegisterColorXYZ_t Dvar_RegisterColorXYZ;

	typedef int(*Dvar_Command_t)(/*PC T5 has bool restricted here*/);
	extern Dvar_Command_t Dvar_Command;

	typedef int(*SV_GameCommand_t)();
	extern SV_GameCommand_t SV_GameCommand;

	typedef dvar_s*(*Dvar_FindVar_t)(const char* dvar);
	extern Dvar_FindVar_t Dvar_FindVar;

	typedef unsigned int(*FS_Write_t)(char* buffer, unsigned int len, int h);
	extern FS_Write_t FS_Write;

	typedef unsigned int(*FS_Read_t)(const char* buffer, unsigned int len, int h);
	extern FS_Read_t FS_Read;

	typedef int(*FS_ReadFile_t)(const char* qpath, void** buffer);
	extern FS_ReadFile_t FS_ReadFile;

	typedef int(*FS_FOpenFileRead_t)(const char* filename, int* file);
	extern FS_FOpenFileRead_t FS_FOpenFileRead;

	typedef int(*FS_FOpenFileByMode_t)(const char *qpath, int *f, fsMode_t mode);
	extern FS_FOpenFileByMode_t FS_FOpenFileByMode;

	typedef int(*FS_FOpenTextFileWrite_t)(const char* filename);
	extern FS_FOpenTextFileWrite_t FS_FOpenTextFileWrite;

	typedef int(*FS_FOpenFileAppend_t)(const char* filename);
	extern FS_FOpenFileAppend_t FS_FOpenFileAppend;

	typedef void(*FS_FCloseFile_t)(int h);
	extern FS_FCloseFile_t FS_FCloseFile;

	typedef const char*(*FS_GetMapBaseName_t)(const char* mapname);
	extern FS_GetMapBaseName_t FS_GetMapBaseName;

	typedef bool(*Key_IsCatcherActive_t)(int localClientNum, int mask);
	extern Key_IsCatcherActive_t Key_IsCatcherActive;

	typedef void(*Key_RemoveCatcher_t)(int localClientNum, int andMask);
	extern Key_RemoveCatcher_t Key_RemoveCatcher;

	typedef int(*LiveContracts_GetVersion_t)();
	extern LiveContracts_GetVersion_t LiveContracts_GetVersion;

	typedef int(*LiveStorage_GetFFOTDVersion_t)();
	extern LiveStorage_GetFFOTDVersion_t LiveStorage_GetFFOTDVersion;

	typedef int(*Playlist_GetVersionNumber_t)();
	extern Playlist_GetVersionNumber_t Playlist_GetVersionNumber;

	typedef void(*LUI_CoD_Init_t)(bool frontend);
	extern LUI_CoD_Init_t LUI_CoD_Init;

	typedef void(*LUI_CoD_Shutdown_t)();
	extern LUI_CoD_Shutdown_t LUI_CoD_Shutdown;

	typedef Material *(*Material_RegisterHandle_t)(const char *name, int imageTrack);
	extern Material_RegisterHandle_t Material_RegisterHandle;

	typedef const char*(*Sys_DefaultInstallPath_t)();
	extern Sys_DefaultInstallPath_t Sys_DefaultInstallPath;

	typedef const char*(*Scr_AddSourceBuffer_t)(scriptInstance_t inst, const char* extFilename,
												const char* codePos, char *sourceBuf, int len, bool doEolFixup, bool archive);
	extern Scr_AddSourceBuffer_t Scr_AddSourceBuffer;

	typedef void(*R_AddCmdDrawStretchPic_t)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, const float* color, Material* material);
	extern R_AddCmdDrawStretchPic_t R_AddCmdDrawStretchPic;

	typedef void(*R_AddCmdDrawText_t)(const char* text, int maxChars, Font_s* font, float x, float y, float xScale, float yScale, float rotation, const float* color, int style);
	extern R_AddCmdDrawText_t R_AddCmdDrawText;

	typedef int(*R_TextWidth_t)(const char *text, int maxChars, Font_s *font);
	extern R_TextWidth_t R_TextWidth;

	typedef unsigned int(*R_GetFrameCount_t)();
	extern R_GetFrameCount_t R_GetFrameCount;

	typedef char*(*I_strlwr_t)(char* s);
	extern I_strlwr_t I_strlwr;

	typedef void(*I_strncpyz_t)(char* dest, const char* src, int destsize);
	extern I_strncpyz_t I_strncpyz;

	typedef char*(*I_strlwr_t)(char* s);
	extern I_strlwr_t I_strlwr;

	typedef int(*I_stricmp_t)(const char* s0, const char* s1);
	extern I_stricmp_t I_stricmp;

	typedef void(*I_strncat_t)(char* dest, int size, const char* src);
	extern I_strncat_t I_strncat;

	extern ScreenPlacement* scrPlaceFullUnsafe;

	extern CG_PerfInfo* cg_perfInfo;

	extern XNADDR_* g_ourXnaddr;

	extern bool byte_84A99C93;
	extern bool byte_84AB7CA2;

	extern bool byte_84C579C8;

	extern bool s_haveFFOTD;
	extern bool s_UTCSynced;
}
