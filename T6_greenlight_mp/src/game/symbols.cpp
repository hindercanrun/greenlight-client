#include "../std_include.hpp"
#include "symbols.hpp"
#include "structs.hpp"

namespace game
{
	// functions
	CG_DrawVersion_t CG_DrawVersion = CG_DrawVersion_t(0x82249CC8);

	Cbuf_AddText_t Cbuf_AddText = Cbuf_AddText_t(0x824ABCC8);
	Cbuf_InsertText_t Cbuf_InsertText = Cbuf_InsertText_t(0x824ABDB8);
	Cmd_Argc_t Cmd_Argc = Cmd_Argc_t(0x82245B80);
	Cmd_Argv_t Cmd_Argv = Cmd_Argv_t(0x82245C10);
	Cmd_AddCommandInternal_t Cmd_AddCommandInternal = Cmd_AddCommandInternal_t(0x824AD1A8);
	Cmd_ExecuteSingleCommand_t Cmd_ExecuteSingleCommand = Cmd_ExecuteSingleCommand_t(0x824ADEF8);
	Cmd_FindCommand_t Cmd_FindCommand = Cmd_FindCommand_t(0x824AD110);
	Cmd_SetAutoComplete_t Cmd_SetAutoComplete = Cmd_SetAutoComplete_t(0x824AD330);

	Com_BeginParseSession_t Com_BeginParseSession = Com_BeginParseSession_t(0x825ADB68);
	Com_EndParseSession_t Com_EndParseSession = Com_EndParseSession_t(0x825ADC68);
	Com_Parse_t Com_Parse = Com_Parse_t(0x825AEAC0);
	Com_Error_t Com_Error = Com_Error_t(0x824BB040);
	Com_Printf_t Com_Printf = Com_Printf_t(0x824BA068);
	Com_PrintWarning_t Com_PrintWarning = Com_PrintWarning_t(0x824BA5D0);
	Com_PrintError_t Com_PrintError = Com_PrintError_t(0x824BA4D0);
	va_t va = va_t(0x825B0B38);
	Com_sprintf_t Com_sprintf = Com_sprintf_t(0x825B0818);
	Com_IsAddonMap_t Com_IsAddonMap = Com_IsAddonMap_t(0x82589218);
	Com_GetBspFilename_t Com_GetBspFilename = Com_GetBspFilename_t(0x825899A8);
	Com_GetExtensionSubString_t Com_GetExtensionSubString = Com_GetExtensionSubString_t(0x825AEF38);
	Com_DecompressWithZLib_t Com_DecompressWithZLib = Com_DecompressWithZLib_t(0x824B6DA8);

	Hunk_AllocateTempMemoryHigh_t Hunk_AllocateTempMemoryHigh = Hunk_AllocateTempMemoryHigh_t(0x82592BA8);

	Con_IsActive_t Con_IsActive = Con_IsActive_t(0x82304128);
	Con_IsDvarCommand_t Con_IsDvarCommand = Con_IsDvarCommand_t(0x82301BA8);

	DB_FindXAssetHeader_t DB_FindXAssetHeader = DB_FindXAssetHeader_t(0x82382240);
	DB_LoadXAssets_t DB_LoadXAssets = DB_LoadXAssets_t(0x82381DA0);

	DevGui_IsActive_t DevGui_IsActive = DevGui_IsActive_t(0x82389FC0);

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

	Dvar_Command_t Dvar_Command = Dvar_Command_t(0x824BFEF8);
	SV_GameCommand_t SV_GameCommand = SV_GameCommand_t(0x824F7EC8);

	Dvar_FindVar_t Dvar_FindVar = Dvar_FindVar_t(0x8259D7C8);

	FS_Write_t FS_Write = FS_Write_t(0x82588718);
	FS_Read_t FS_Read = FS_Read_t(0x825886C0);
	FS_ReadFile_t FS_ReadFile = FS_ReadFile_t(0x82588CC0);
	FS_FOpenFileRead_t FS_FOpenFileRead = FS_FOpenFileRead_t(0x825885A0);
	FS_FOpenFileByMode_t FS_FOpenFileByMode = FS_FOpenFileByMode_t(0x825890A0);
	FS_FOpenTextFileWrite_t FS_FOpenTextFileWrite = FS_FOpenTextFileWrite_t(0x825898D8);
	FS_FOpenFileAppend_t FS_FOpenFileAppend = FS_FOpenFileAppend_t(0x82588408);
	FS_FCloseFile_t FS_FCloseFile = FS_FCloseFile_t(0x82588340);
	FS_GetMapBaseName_t FS_GetMapBaseName = FS_GetMapBaseName_t(0x824C22F0);

	Key_IsCatcherActive_t Key_IsCatcherActive = Key_IsCatcherActive_t(0x82312038);
	Key_RemoveCatcher_t Key_RemoveCatcher = Key_RemoveCatcher_t(0x82312100);

	LiveContracts_GetVersion_t LiveContracts_GetVersion = LiveContracts_GetVersion_t(0x82713CE8);
	LiveStorage_GetFFOTDVersion_t LiveStorage_GetFFOTDVersion = LiveStorage_GetFFOTDVersion_t(0x8275F3F8);
	Playlist_GetVersionNumber_t Playlist_GetVersionNumber = Playlist_GetVersionNumber_t(0x82543B10);

	LUI_CoD_Init_t LUI_CoD_Init = LUI_CoD_Init_t(0x828BDA08);
	LUI_CoD_Shutdown_t LUI_CoD_Shutdown = LUI_CoD_Shutdown_t(0x828BBCF8);

	Material_RegisterHandle_t Material_RegisterHandle = Material_RegisterHandle_t(0x829E2BB8);

	Sys_DefaultInstallPath_t Sys_DefaultInstallPath = Sys_DefaultInstallPath_t(0x825B43F0);

	Scr_AddSourceBuffer_t Scr_AddSourceBuffer = Scr_AddSourceBuffer_t(0x8265B030);

	R_AddCmdDrawStretchPic_t R_AddCmdDrawStretchPic = R_AddCmdDrawStretchPic_t(0x829E66F0);
	R_AddCmdDrawText_t R_AddCmdDrawText = R_AddCmdDrawText_t(0x829E6C38);
	R_TextWidth_t R_TextWidth = R_TextWidth_t(0x829E1C68);
	R_GetFrameCount_t R_GetFrameCount = R_GetFrameCount_t(0x829E63D0);

	I_strncpyz_t I_strncpyz = I_strncpyz_t(0x825AFC88);
	I_strlwr_t I_strlwr = I_strlwr_t(0x825B0420);
	I_stricmp_t I_stricmp = I_stricmp_t(0x825B0160);
	I_strncat_t I_strncat = I_strncat_t(0x825B05B8);

	// variables
	ScreenPlacement* scrPlaceFullUnsafe = reinterpret_cast<ScreenPlacement*>(0x82EA7478);

	CG_PerfInfo* cg_perfInfo = reinterpret_cast<CG_PerfInfo*>(0x82DC3A40);

	XNADDR_* g_ourXnaddr = reinterpret_cast<XNADDR_*>(0x8446EC10);

	bool byte_84A99C93 = reinterpret_cast<bool*>(0x84A99C93);
	bool byte_84AB7CA2 = reinterpret_cast<bool*>(0x84AB7CA2);

	bool byte_84C579C8 = reinterpret_cast<bool*>(0x84C579C8);

	bool s_haveFFOTD = reinterpret_cast<bool*>(0x84C577A8);
	bool s_UTCSynced = reinterpret_cast<bool*>(0x84BF007C);
}