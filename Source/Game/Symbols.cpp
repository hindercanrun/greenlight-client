#include "../Std_Include.h"
#include "Symbols.h"

namespace Symbols
{
	// functions
	Cbuf_AddText_t Cbuf_AddText = Cbuf_AddText_t(0x824ABCC8);
	Cbuf_InsertText_t Cbuf_InsertText = Cbuf_InsertText_t(0x824ABDB8);
	Cmd_Argc_t Cmd_Argc = Cmd_Argc_t(0x82245B80);
	Cmd_Argv_t Cmd_Argv = Cmd_Argv_t(0x82245C10);
	Cmd_AddCommand_t Cmd_AddCommand = Cmd_AddCommand_t(0x824AD1A8);
	Cmd_ExecuteSingleCommand_t Cmd_ExecuteSingleCommand = Cmd_ExecuteSingleCommand_t(0x824ADEF8);
	Cmd_FindCommand_t Cmd_FindCommand = Cmd_FindCommand_t(0x824AD110);

	Com_Error_t Com_Error = Com_Error_t(0x824BB040);
	Com_Printf_t Com_Printf = Com_Printf_t(0x824BA068);
	Com_PrintWarning_t Com_PrintWarning = Com_PrintWarning_t(0x824BA5D0);
	Com_PrintError_t Com_PrintError = Com_PrintError_t(0x824BA4D0);
	Com_sprintf_t Com_sprintf = Com_sprintf_t(0x825B0818);

	DB_FindXAssetHeader_t DB_FindXAssetHeader = DB_FindXAssetHeader_t(0x82382240);
	DB_LoadXAssets_t DB_LoadXAssets = DB_LoadXAssets_t(0x82381DA0);

	DevGui_IsActive_t DevGui_IsActive = DevGui_IsActive_t(0x82389FC0);

	Dvar_Command_t Dvar_Command = Dvar_Command_t(0x824BFEF8);
	SV_GameCommand_t SV_GameCommand = SV_GameCommand_t(0x824F7EC8);

	Dvar_FindVar_t Dvar_FindVar = Dvar_FindVar_t(0x8259D7C8);

	Dvar_RegisterBool_t Dvar_RegisterBool = Dvar_RegisterBool_t(0x825A1EF8);
	Dvar_RegisterInt_t Dvar_RegisterInt = Dvar_RegisterInt_t(0x825A1F70);
	Dvar_RegisterInt64_t Dvar_RegisterInt64 = Dvar_RegisterInt64_t(0x825A1F70);
	Dvar_RegisterFloat_t Dvar_RegisterFloat = Dvar_RegisterFloat_t(0x825A2040);
	Dvar_RegisterVec2_t Dvar_RegisterVec2 = Dvar_RegisterVec2_t(0x825A20C8);
	Dvar_RegisterVec3_t Dvar_RegisterVec3 = Dvar_RegisterVec3_t(0x825A2150);
	Dvar_RegisterVec4_t Dvar_RegisterVec4 = Dvar_RegisterVec4_t(0x825A21E0);
	Dvar_RegisterString_t Dvar_RegisterString = Dvar_RegisterString_t(0x825A2288);
	Dvar_RegisterEnum_t Dvar_RegisterEnum = Dvar_RegisterEnum_t(0x825A23C0);
	Dvar_RegisterColor_t Dvar_RegisterColor = Dvar_RegisterColor_t(0x825A2518);
	Dvar_RegisterLinearRGB_t Dvar_RegisterLinearRGB = Dvar_RegisterLinearRGB_t(0x825A2670);
	Dvar_RegisterColorXYZ_t Dvar_RegisterColorXYZ = Dvar_RegisterColorXYZ_t(0x825A2700);

	FS_Write_t FS_Write = FS_Write_t(0x82588718);
	FS_Read_t FS_Read = FS_Read_t(0x825886C0);
	FS_ReadFile_t FS_ReadFile = FS_ReadFile_t(0x82588CC0);
	FS_FOpenFileRead_t FS_FOpenFileRead = FS_FOpenFileRead_t(0x825885A0);
	FS_FOpenFileByMode_t FS_FOpenFileByMode = FS_FOpenFileByMode_t(0x825890A0);
	FS_FOpenTextFileWrite_t FS_FOpenTextFileWrite = FS_FOpenTextFileWrite_t(0x825898D8);
	FS_FOpenFileAppend_t FS_FOpenFileAppend = FS_FOpenFileAppend_t(0x82588408);
	FS_FCloseFile_t FS_FCloseFile = FS_FCloseFile_t(0x82588340);

	Key_IsCatcherActive_t Key_IsCatcherActive = Key_IsCatcherActive_t(0x82312038);
	Key_RemoveCatcher_t Key_RemoveCatcher = Key_RemoveCatcher_t(0x82312100);

	LiveContracts_GetVersion_t LiveContracts_GetVersion = LiveContracts_GetVersion_t(0x82713CE8);
	LiveStorage_GetFFOTDVersion_t LiveStorage_GetFFOTDVersion = LiveStorage_GetFFOTDVersion_t(0x8275F3F8);
	Playlist_GetVersionNumber_t Playlist_GetVersionNumber = Playlist_GetVersionNumber_t(0x82543B10);

	LUI_CoD_Init_t LUI_CoD_Init = LUI_CoD_Init_t(0x828BDA08);
	LUI_CoD_Shutdown_t LUI_CoD_Shutdown = LUI_CoD_Shutdown_t(0x828BBCF8);

	Material_RegisterHandle_t Material_RegisterHandle = Material_RegisterHandle_t(0x829E2BB8);

	Sys_Milliseconds_t Sys_Milliseconds = Sys_Milliseconds_t(0x825DC628);

	R_AddCmdDrawStretchPic_t R_AddCmdDrawStretchPic = R_AddCmdDrawStretchPic_t(0x829E66F0);
	R_AddCmdDrawText_t R_AddCmdDrawText = R_AddCmdDrawText_t(0x829E6C38);
	R_TextWidth_t R_TextWidth = R_TextWidth_t(0x829E1C68);

	I_strncpyz_t I_strncpyz = I_strncpyz_t(0x825AFC88);
	I_strlwr_t I_strlwr = I_strlwr_t(0x825B0420);
	I_stricmp_t I_stricmp = I_stricmp_t(0x825B0160);
	I_strncat_t I_strncat = I_strncat_t(0x825B05B8);

	// variables
	Structs::CG_PerfInfo* cg_perfInfo = reinterpret_cast<Structs::CG_PerfInfo*>(0x82DC3A40);

	Structs::ScreenPlacement* scrPlaceFullUnsafe = reinterpret_cast<Structs::ScreenPlacement*>(0x82EA7478);
}