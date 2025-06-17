#pragma once

#define TITLE_ID 0x415608C3 // Black Ops II

#define	MAX_QPATH 64 // max length of a path name

// this makes 'std::numeric_limits<int>::max()' work
#define NOMINMAX

// KeGetCurrentProcessType()
#define IDLE_PROC 0
#define USER_PROC 1
#define SYSTEM_PROC 2

#define NTSTATUS long
// #define ULONG ACCESS_MASK;

// #define NT_EXTRACT_ST(Status) ((((ULONG)(Status)) >> 30)& 0x3)
// #define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
// #define NT_INFORMATION(Status) (NT_EXTRACT_ST(Status) == 1)
// #define NT_WARNING(Status) (NT_EXTRACT_ST(Status) == 2)
// #define NT_ERROR(Status) (NT_EXTRACT_ST(Status) == 3)

#define __isync() __emit(0x4C00012C)
#define __icbi(rA, rB) __emit(0x7C80006C);

#include <string>
#include <vector>
#include <cstdint>
#include <cwctype>
#include <algorithm>
#include <iostream>
#include <ctime>
#include <cstdio>
#include <unordered_map>
#include <fstream>

#include <xtl.h>
#include <stdio.h> // for sprintf_s
#include <stdint.h>
#include <ppcintrinsics.h>
#include <direct.h>

#include <xtl.h>
#include <xboxmath.h>

#include "game\game.hpp"
#include "game\structs.hpp"
#include "game\symbols.hpp"

// xbox kernel stuff
extern "C"
{
	NTSYSAPI NTSTATUS NTAPI XamGetExecutionId(IN OUT PXEX_EXECUTION_ID* xid);
	NTSYSAPI DWORD NTAPI XamGetCurrentTitleId(VOID);

	NTSYSAPI BOOL NTAPI XexCheckExecutablePrivilege(IN DWORD priviledge);

	// example: XexGetModuleHandle("xam.xex", &hand), returns 0 on success
	NTSYSAPI NTSTATUS NTAPI XexGetModuleHandle(IN PSZ moduleName, IN OUT PHANDLE hand); 

	// example: XexGetProcedureAddress(hand, 0x50, &addr) returns 0 on success
	NTSYSAPI DWORD NTAPI XexGetProcedureAddress(IN HANDLE hand, IN DWORD dwOrdinal, IN PVOID Address);

	NTSYSAPI DWORD NTAPI XexLoadExecutable(IN PCHAR xexName, IN OUT PHANDLE handle, IN DWORD typeInfo, IN DWORD ver);
	NTSYSAPI DWORD NTAPI XexLoadImage(IN LPCSTR xexName, IN DWORD typeInfo, IN DWORD ver, IN OUT PHANDLE modHandle);

	NTSYSAPI VOID NTAPI DbgPrint(const char* s, ...);
}
