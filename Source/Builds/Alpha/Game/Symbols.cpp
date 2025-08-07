#include "../Std_Include.h"
#include "Symbols.h"

namespace Symbols
{
	// Functions
	Cbuf_AddText_t Cbuf_AddText = Cbuf_AddText_t(0x8250F1C8);
	Cbuf_InsertText_t Cbuf_InsertText = Cbuf_InsertText_t(0x8250F2B0);

	Cmd_Argc_t Cmd_Argc = Cmd_Argc_t(0x8225E8B0);
	Cmd_Argv_t Cmd_Argv = Cmd_Argv_t(0x8225E930);
	Cmd_AddCommand_t Cmd_AddCommand = Cmd_AddCommand_t(0x825105B8);
	Cmd_ExecuteSingleCommand_t Cmd_ExecuteSingleCommand = Cmd_ExecuteSingleCommand_t(0x82511280);

	Com_Printf_t Com_Printf = Com_Printf_t(0x82519E38);
	Com_PrintWarning_t Com_PrintWarning = Com_PrintWarning_t(0x8251E798);
	Com_PrintError_t Com_PrintError = Com_PrintError_t(0x8251E6A8);
	Com_sprintf_t Com_sprintf = Com_sprintf_t(0x825FE558);
	Com_HashKey_t Com_HashKey = Com_HashKey_t(0x825D50C8);

	DB_FindXAssetHeader_t DB_FindXAssetHeader = DB_FindXAssetHeader_t(0x823A4F70);
	DB_GetAllXAssetOfType_FastFile_t DB_GetAllXAssetOfType_FastFile = DB_GetAllXAssetOfType_FastFile_t(0x8239DE00);

	Dvar_FindVar_t Dvar_FindVar = Dvar_FindVar_t(0x825DEEA8);

	FS_FOpenTextFileWrite_t FS_FOpenTextFileWrite = FS_FOpenTextFileWrite_t(0x825CBB08);

	SL_ConvertToString_t SL_ConvertToString = SL_ConvertToString_t(0x826BB148);

	Sys_Milliseconds_t Sys_Milliseconds = Sys_Milliseconds_t(0x826414C0);

	R_AddCmdDrawText_t R_AddCmdDrawText = R_AddCmdDrawText_t(0x82AA4210);

	// Variables
	Structs::CG_PerfInfo* cg_perfInfo = reinterpret_cast<Structs::CG_PerfInfo*>(0x82E564F0);

	int* logfile = reinterpret_cast<int*>(0x83C2263C);
}