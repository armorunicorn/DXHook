// HookDll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

#include <easyhook.h>
#include <string>
#include <iostream>
#include <Windows.h>
#include <d3dkmthk.h>
#include "include/log4cpp/Category.hh"
#include "include/log4cpp/PropertyConfigurator.hh"

//using namespace std;

#pragma comment(lib, "log4cppD.lib")
#pragma comment(lib, "Ws2_32.lib")

using DDIFakeFunType = BOOL(WINAPI*)(void* arg);

#define HOOK_FUN_DEF(OrgFunName, ArgName) \
BOOL WINAPI OrgFunName##Fake(##ArgName *Arg1)\
{\
	log4cpp::Category& root = log4cpp::Category::getRoot();\
	Save##ArgName(Arg1, root, in);\
	BOOL ret = OrgFunName##(Arg1);\
	Save##ArgName(Arg1, root, out);\
	return ret;\
}

#define DDI_HOOK_FUN_DEF(OrgFunName, ArgName) \
DDIFakeFunType OrgFunName##Org = (DDIFakeFunType)GetProcAddress(GetModuleHandle(TEXT("win32u")), #OrgFunName);\
BOOL WINAPI OrgFunName##Fake(##ArgName *Arg1)\
{\
	log4cpp::Category& root = log4cpp::Category::getRoot();\
	Save##ArgName(Arg1, root, in);\
	BOOL ret = OrgFunName##Org((void*)Arg1);\
	Save##ArgName(Arg1, root, out);\
	return ret;\
}

enum LogType
{
	in,
	out
};

void SaveD3DKMT_CREATEALLOCATION(D3DKMT_CREATEALLOCATION *Arg1, UINT tpye, LogType inout);
void SaveD3DKMT_CREATEDEVICE(D3DKMT_CREATEDEVICE  *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_CREATECONTEXT(D3DKMT_CREATECONTEXT  *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_RENDER(D3DKMT_RENDER  *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_CREATECONTEXTVIRTUAL(D3DKMT_CREATECONTEXTVIRTUAL   *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_ESCAPE(D3DKMT_ESCAPE    *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_SUBMITCOMMAND(D3DKMT_SUBMITCOMMAND     *Arg1, log4cpp::Category& log, LogType inout);

//NtGdiDdDDI
HOOK_FUN_DEF(D3DKMTCreateDevice, D3DKMT_CREATEDEVICE)
DDI_HOOK_FUN_DEF(NtGdiDdDDICreateDevice, D3DKMT_CREATEDEVICE)

//HOOK_FUN_DEF(D3DKMTCreateAllocation, D3DKMT_CREATEALLOCATION)
//DDI_HOOK_FUN_DEF(NtGdiDdDDICreateAllocation, D3DKMT_CREATEALLOCATION)
BOOL WINAPI D3DKMTCreateAllocationFake(D3DKMT_CREATEALLOCATION *Arg1)
{
	//log4cpp::Category& root = log4cpp::Category::getRoot();
	int type = 2;
	__try
	{
		SaveD3DKMT_CREATEALLOCATION(Arg1, 2, in);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		type = 1;
		SaveD3DKMT_CREATEALLOCATION(Arg1, 1, in);
	}
	
	BOOL ret = D3DKMTCreateAllocation(Arg1);
	SaveD3DKMT_CREATEALLOCATION(Arg1, type, out);
	return ret;
}

DDIFakeFunType NtGdiDdDDICreateAllocationOrg = (DDIFakeFunType)GetProcAddress(GetModuleHandle(TEXT("win32u")), "NtGdiDdDDICreateAllocation");
BOOL WINAPI NtGdiDdDDICreateAllocationFake(D3DKMT_CREATEALLOCATION *Arg1)
{
	//log4cpp::Category& root = log4cpp::Category::getRoot();
	int type = 2;
	__try
	{
		SaveD3DKMT_CREATEALLOCATION(Arg1, 2, in);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		SaveD3DKMT_CREATEALLOCATION(Arg1, 1, in);
	}
	BOOL ret = NtGdiDdDDICreateAllocationOrg((void*)Arg1);
	SaveD3DKMT_CREATEALLOCATION(Arg1, type, out);
	return ret;
}

HOOK_FUN_DEF(D3DKMTCreateContext, D3DKMT_CREATECONTEXT)
DDI_HOOK_FUN_DEF(NtGdiDdDDICreateContext, D3DKMT_CREATECONTEXT)

HOOK_FUN_DEF(D3DKMTRender, D3DKMT_RENDER)
DDI_HOOK_FUN_DEF(NtGdiDdDDIRender, D3DKMT_RENDER)

HOOK_FUN_DEF(D3DKMTCreateContextVirtual, D3DKMT_CREATECONTEXTVIRTUAL)
DDI_HOOK_FUN_DEF(NtGdiDdDDICreateContextVirtual, D3DKMT_CREATECONTEXTVIRTUAL)

HOOK_FUN_DEF(D3DKMTEscape, D3DKMT_ESCAPE)
DDI_HOOK_FUN_DEF(NtGdiDdDDIEscape, D3DKMT_ESCAPE)

HOOK_FUN_DEF(D3DKMTSubmitCommand, D3DKMT_SUBMITCOMMAND)
DDI_HOOK_FUN_DEF(NtGdiDdDDISubmitCommand, D3DKMT_SUBMITCOMMAND)



extern "C" void __declspec(dllexport) __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO* inRemoteInfo);

#define HOOK_FUN(OrgFunName, HookFun, DllName, LOG, ret) \
HOOK_TRACE_INFO h##HookFun##Hook = { NULL };\
##ret = LhInstallHook( \
GetProcAddress(GetModuleHandle(TEXT(#DllName)), #OrgFunName), \
HookFun, \
NULL, \
&h##HookFun##Hook); \
if (FAILED(##ret)) \
{\
	##LOG.error("Hook ##OrgFunName Failed\n");\
}\
LhSetExclusiveACL(ACLEntries, 1, &h##HookFun##Hook);

void __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO* inRemoteInfo)
{
	std::string initFileName = "C:\\Users\\Saber\\Documents\\project\\DXHook\\DXHook\\log4cpp.properties";
	log4cpp::PropertyConfigurator::configure(initFileName);
	log4cpp::Category& root = log4cpp::Category::getRoot();
	root.info("\n\n------------------Hook start-------------------------\n\n");

	root.info( "Injected by process Id: %d", inRemoteInfo->HostPID);

	// Perform hooking
	NTSTATUS result = 0;
	ULONG ACLEntries[1] = { 0 };
	HOOK_FUN(D3DKMTCreateAllocation, D3DKMTCreateAllocationFake, gdi32, root, result);
	HOOK_FUN(NtGdiDdDDICreateAllocation, NtGdiDdDDICreateAllocationFake, win32u, root, result);

	HOOK_FUN(D3DKMTCreateDevice, D3DKMTCreateDeviceFake, gdi32, root, result);
	HOOK_FUN(NtGdiDdDDICreateDevice, NtGdiDdDDICreateDeviceFake, win32u, root, result);

	HOOK_FUN(D3DKMTCreateContext, D3DKMTCreateContextFake, gdi32, root, result);
	HOOK_FUN(NtGdiDdDDICreateContext, NtGdiDdDDICreateContextFake, win32u, root, result);

	HOOK_FUN(D3DKMTCreateContextVirtual, D3DKMTCreateContextVirtualFake, gdi32, root, result);
	HOOK_FUN(NtGdiDdDDICreateContextVirtual, NtGdiDdDDICreateContextVirtualFake, win32u, root, result);

	HOOK_FUN(D3DKMTRender, D3DKMTRenderFake, gdi32, root, result);
	HOOK_FUN(NtGdiDdDDIRender, NtGdiDdDDIRenderFake, win32u, root, result);

	HOOK_FUN(D3DKMTEscape, D3DKMTEscapeFake, gdi32, root, result);
	HOOK_FUN(NtGdiDdDDIEscape, NtGdiDdDDIEscapeFake, win32u, root, result);

	HOOK_FUN(D3DKMTSubmitCommand, D3DKMTSubmitCommandFake, gdi32, root, result);
	HOOK_FUN(NtGdiDdDDISubmitCommand, NtGdiDdDDISubmitCommandFake, win32u, root, result);
	

	// If the threadId in the ACL is set to 0,
	// then internally EasyHook uses GetCurrentThreadId()
	//ULONG ACLEntries[1] = { 0 };

	// Disable the hook for the provided threadIds, enable for all others
	
	RhWakeUpProcess();
	return;
}

std::string GetLogStr(LogType inout)
{
	const char* str;
	switch (inout)
	{
	case in:
		str = "ArgIn";
		break;
	case out:
		str = "ArgOut";
		break;
	default:
		str = nullptr;
		break;
	}
	return str;
}

std::string FormatDWORD(DWORD value)
{
	char strBuf[10] = { 0 };
	sprintf_s(strBuf, "%x", value);
	std::string ret = std::string(strBuf);
	return ret;
}

std::string FormatULONG64(ULONG64 value)
{
	char strBuf[20] = { 0 };
	sprintf_s(strBuf, "%llx", value);
	std::string ret = std::string(strBuf);
	return ret;
}

std::string FormatPVOID(char* pBuf, UINT bufSize, UINT step, const char* name)
{
	char stepBuf[6] = { 0 };
	char tmpBuf[12] = { 0 };
	if (step > 5)
		return nullptr;
	for (UINT i = 0; i < step; i++)
	{
		stepBuf[i] = '\t';
	}
	std::string pvoidStr = stepBuf;
	pvoidStr += "start VOID ";
	pvoidStr += name;
	pvoidStr += "\n";
	stepBuf[step] = '\t';
	if (bufSize == 0)
	{
		pvoidStr += stepBuf;
		pvoidStr += "buffer size is 0\n";
	}
	else
	{
		pvoidStr += stepBuf;
		UINT i = 0;
		for (; i <= bufSize - 4; i += 4)
		{
			if (i != 0 && i % 4 == 0)
			{
				pvoidStr += " ";
			}
			if (i != 0 && i % 48 == 0)
			{
				pvoidStr += "\n";
				pvoidStr += stepBuf;
			}
			sprintf_s(tmpBuf, "%08x", *((UINT*)(pBuf + i)));
			pvoidStr += tmpBuf;
		}
		if (i > bufSize - 4 && i < bufSize)
		{
			for (UINT j = i; j < bufSize; j++)
			{
				sprintf_s(tmpBuf, "%02x", pBuf[j]);
				pvoidStr += tmpBuf;
			}
		}
		pvoidStr += "\n";
	}

	stepBuf[step] = 0;
	pvoidStr += stepBuf;
	pvoidStr += "end VOID ";
	pvoidStr += name;
	pvoidStr += "\n";
	return pvoidStr;
}

void SaveD3DKMT_CREATEALLOCATION(D3DKMT_CREATEALLOCATION *Arg1, UINT type, LogType inout)
{
	log4cpp::Category& root = log4cpp::Category::getRoot();
	char tmpBuf[512] = { 0 };
	std::string logstr = "\nstart struct D3DKMT_CREATEALLOCATION ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hDevice " + FormatDWORD(Arg1->hDevice) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hResource " + FormatDWORD(Arg1->hResource) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hGlobalShare " + FormatDWORD(Arg1->hGlobalShare) + "\n";
	logstr += FormatPVOID((char*)Arg1->pPrivateRuntimeData, Arg1->PrivateRuntimeDataSize, 1, "pPrivateRuntimeData");
	logstr += "\tmember UINT PrivateRuntimeDataSize " + FormatDWORD(Arg1->PrivateRuntimeDataSize) + "\n";

	logstr += "\tstart union\n";
	if (Arg1->PrivateDriverDataSize > sizeof(D3DKMT_CREATESTANDARDALLOCATION))
	{
		logstr += "\t\tstart struct D3DKMT_CREATESTANDARDALLOCATION pStandardAllocation\n";
		logstr += "\t\t\tmember D3DKMT_STANDARDALLOCATIONTYPE Type " + FormatDWORD(Arg1->pStandardAllocation->Type) + "\n";
		logstr += "\t\t\tstart struct D3DKMT_STANDARDALLOCATION_EXISTINGHEAP ExistingHeapData\n";
		logstr += "\t\t\t\tmember SIZE_T size " + FormatDWORD(Arg1->pStandardAllocation->ExistingHeapData.Size) + "\n";
		logstr += "\t\t\tend struct D3DKMT_STANDARDALLOCATION_EXISTINGHEAP ExistingHeapData\n";
		logstr += "\t\t\tmember D3DKMT_CREATESTANDARDALLOCATIONFLAGS Flags " + FormatDWORD((DWORD)Arg1->pStandardAllocation->Flags.Value) + "\n";
		logstr += "\t\tend struct D3DKMT_CREATESTANDARDALLOCATION pStandardAllocation\n";
	}

	logstr += FormatPVOID((char*)Arg1->pPrivateDriverData, Arg1->PrivateDriverDataSize, 2, "pPrivateDriverData");
	logstr += "\tend union\n";

	logstr += "\tmember UINT PrivateDriverDataSize " + FormatDWORD(Arg1->PrivateDriverDataSize) + "\n";
	logstr += "\tmember UINT NumAllocations " + FormatDWORD(Arg1->NumAllocations) + "\n";
	std::string tmpstr = "";

	//try D3DDDI_ALLOCATIONINFO2 
	
	if (type == 2)
	{
		tmpstr += "\tstart list\n";
		for (UINT i = 0; i < Arg1->NumAllocations; i++)
		{
			D3DDDI_ALLOCATIONINFO2 *cur_ptr = Arg1->pAllocationInfo2 + i;
			tmpstr += "\t\tstart struct D3DDDI_ALLOCATIONINFO2\n";
			tmpstr += "\t\t\tmember D3DKMT_HANDLE hAllocation " + FormatDWORD(cur_ptr->hAllocation) + "\n";

			tmpstr += "\t\t\tstart union\n";
			tmpstr += "\t\t\t\tmember HANDLE hSection " + FormatDWORD((DWORD)cur_ptr->hSection) + "\n";
			tmpstr += "\t\t\t\tmember PVOID pSystemMem " + FormatDWORD((DWORD)cur_ptr->pSystemMem) + "\n";
			tmpstr += "\t\t\tend union\n";

			tmpstr += FormatPVOID((char*)cur_ptr->pPrivateDriverData, cur_ptr->PrivateDriverDataSize, 3, "pPrivateDriverData");
			tmpstr += "\t\t\tmember UINT PrivateDriverDataSize " + FormatDWORD(cur_ptr->PrivateDriverDataSize) + "\n";
			tmpstr += "\t\t\tmember D3DDDI_VIDEO_PRESENT_SOURCE_ID VidPnSourceId " + FormatDWORD(cur_ptr->VidPnSourceId) + "\n";

			tmpstr += "\t\t\tstart union Flags\n";
			tmpstr += "\t\t\t\tmember UINT Primary " + FormatDWORD((DWORD)cur_ptr->Flags.Primary) + "\n";
			tmpstr += "\t\t\t\tmember UINT Stereo " + FormatDWORD((DWORD)cur_ptr->Flags.Stereo) + "\n";
			tmpstr += "\t\t\t\tmember UINT OverridePriority " + FormatDWORD((DWORD)cur_ptr->Flags.OverridePriority) + "\n";
			tmpstr += "\t\t\t\tmember UINT Value " + FormatDWORD((DWORD)cur_ptr->Flags.Value) + "\n";
			tmpstr += "\t\t\tend union\n";

			tmpstr += "\t\t\t\tmember D3DGPU_VIRTUAL_ADDRESS GpuVirtualAddress " + FormatDWORD((DWORD)cur_ptr->GpuVirtualAddress) + "\n";

			tmpstr += "\t\t\tstart union\n";
			tmpstr += "\t\t\t\tmember UINT Priority " + FormatDWORD((DWORD)cur_ptr->Priority) + "\n";
			tmpstr += "\t\t\t\tmember UINT Unused " + FormatDWORD((DWORD)cur_ptr->Unused) + "\n";
			tmpstr += "\t\t\tend union\n";

			tmpstr += FormatPVOID((char*)cur_ptr->Reserved, 5 * sizeof(ULONG), 3, "Reserved");
			tmpstr += "\t\tend struct D3DDDI_ALLOCATIONINFO2\n";
		}
		tmpstr += "\tend list\n";
	}
	else
	{
		//is D3DDDI_ALLOCATIONINFO
		tmpstr += "\tstart list\n";
		for (UINT i = 0; i < Arg1->NumAllocations; i++)
		{
			D3DDDI_ALLOCATIONINFO *cur_ptr = Arg1->pAllocationInfo + i;
			tmpstr += "\t\tstart struct D3DDDI_ALLOCATIONINFO\n";
			tmpstr += "\t\t\tmember D3DKMT_HANDLE hAllocation " + FormatDWORD(cur_ptr->hAllocation) + "\n";
			tmpstr += "\t\t\tmember PVOID pSystemMem " + FormatDWORD((DWORD)cur_ptr->pSystemMem) + "\n";
			tmpstr += FormatPVOID((char*)cur_ptr->pPrivateDriverData, cur_ptr->PrivateDriverDataSize, 3, "pPrivateDriverData");
			tmpstr += "\t\t\tmember UINT PrivateDriverDataSize " + FormatDWORD(cur_ptr->PrivateDriverDataSize) + "\n";
			tmpstr += "\t\t\tmember D3DDDI_VIDEO_PRESENT_SOURCE_ID VidPnSourceId " + FormatDWORD(cur_ptr->VidPnSourceId) + "\n";

			tmpstr += "\t\t\tstart union Flags\n";
			tmpstr += "\t\t\t\tmember UINT Primary " + FormatDWORD((DWORD)cur_ptr->Flags.Primary) + "\n";
			tmpstr += "\t\t\t\tmember UINT Stereo " + FormatDWORD((DWORD)cur_ptr->Flags.Stereo) + "\n";
			tmpstr += "\t\t\t\tmember UINT Value " + FormatDWORD((DWORD)cur_ptr->Flags.Value) + "\n";
			tmpstr += "\t\t\tend union\n";
			tmpstr += "\t\tend struct D3DDDI_ALLOCATIONINFO\n";
		}
		tmpstr += "\tend list\n";
	}
	logstr += tmpstr;

	logstr += "\tstart struct D3DKMT_CREATEALLOCATIONFLAGS Flags\n";
	logstr += "\t\tmember " + FormatDWORD(*((DWORD*)&Arg1->Flags)) + "\n";
	logstr += "\tend struct D3DKMT_CREATEALLOCATIONFLAGS Flags\n";

	logstr += "\t\tmember HANDLE hPrivateRuntimeResourceHandle " + FormatDWORD((DWORD)Arg1->hPrivateRuntimeResourceHandle) + "\n";
	logstr += "end struct D3DKMT_CREATEALLOCATION";
	root.info(logstr);
}

void SaveD3DKMT_CREATEDEVICE(D3DKMT_CREATEDEVICE  *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = "\nstart struct D3DKMT_CREATEDEVICE ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hAdapter " + FormatDWORD(Arg1->hAdapter) + "\n";
	logstr += "\tmember " + FormatDWORD(*((DWORD*)&Arg1->Flags)) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hDevice " + FormatDWORD(Arg1->hDevice) + "\n";

	logstr += FormatPVOID((char*)Arg1->pCommandBuffer, Arg1->CommandBufferSize, 1, "pCommandBuffer");
	logstr += "\tmember UINT CommandBufferSize " + FormatDWORD(Arg1->CommandBufferSize) + "\n";

	logstr += "\tstart list\n";
	D3DDDI_ALLOCATIONLIST *cur_allocation;
	for (UINT i = 0; i < Arg1->AllocationListSize; i++)
	{
		logstr += "\t\tstart struct D3DDDI_ALLOCATIONLIST\n";
		cur_allocation = Arg1->pAllocationList + i;
		logstr += "\t\t\tmember D3DKMT_HANDLE hAllocation " + FormatDWORD(cur_allocation->hAllocation) + "\n";
		logstr += "\t\t\tmember UINT Value " + FormatDWORD(cur_allocation->Value) + "\n";
		logstr += "\t\tend struct D3DDDI_ALLOCATIONLIST\n";
	}
	logstr += "\tend list\n";
	logstr += "\tmember UINT AllocationListSize " + FormatDWORD(Arg1->AllocationListSize) + "\n";

	logstr += "\tstart list\n";
	D3DDDI_PATCHLOCATIONLIST *cur_patchlocaton;
	for (UINT i = 0; i < Arg1->PatchLocationListSize; i++)
	{
		logstr += "\t\tstart struct D3DDDI_PATCHLOCATIONLIST\n";
		cur_patchlocaton = Arg1->pPatchLocationList + i;
		logstr += "\t\t\tmember UINT AllocationIndex " + FormatDWORD(cur_patchlocaton->AllocationIndex) + "\n";
		logstr += "\t\t\tmember UINT Value " + FormatDWORD(cur_patchlocaton->Value) + "\n";
		logstr += "\t\t\tmember UINT DriverId " + FormatDWORD(cur_patchlocaton->DriverId) + "\n";
		logstr += "\t\t\tmember UINT AllocationOffset " + FormatDWORD(cur_patchlocaton->AllocationOffset) + "\n";
		logstr += "\t\t\tmember UINT PatchOffset " + FormatDWORD(cur_patchlocaton->PatchOffset) + "\n";
		logstr += "\t\t\tmember UINT SplitOffset " + FormatDWORD(cur_patchlocaton->SplitOffset) + "\n";
		logstr += "\t\tend struct D3DDDI_PATCHLOCATIONLIST\n";
	}
	logstr += "\tend list\n";
	logstr += "\tmember UINT PatchLocationListSize " + FormatDWORD(Arg1->PatchLocationListSize) + "\n";

	logstr += "end struct D3DKMT_CREATEDEVICE";
	log.info(logstr);
}

void SaveD3DKMT_CREATECONTEXT(D3DKMT_CREATECONTEXT  *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = "\nstart struct D3DKMT_CREATECONTEXT ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hDevice " + FormatDWORD(Arg1->hDevice) + "\n";
	logstr += "\tmember UINT NodeOrdinal " + FormatDWORD(Arg1->NodeOrdinal) + "\n";
	logstr += "\tmember UINT EngineAffinity " + FormatDWORD(Arg1->EngineAffinity) + "\n";
	logstr += "\tmember Flags" + FormatDWORD(*((DWORD*)&Arg1->Flags)) + "\n";

	logstr += FormatPVOID((char*)Arg1->pPrivateDriverData, Arg1->PrivateDriverDataSize, 1, "pPrivateDriverData");
	logstr += "\tmember UINT PrivateDriverDataSize " + FormatDWORD(Arg1->PrivateDriverDataSize) + "\n";

	logstr += "\tmember D3DKMT_CLIENTHINT ClientHint ";
	switch (Arg1->ClientHint)
	{
	case D3DKMT_CLIENTHINT_UNKNOWN:
		logstr += "D3DKMT_CLIENTHINT_UNKNOWN\n";
		break;
	case D3DKMT_CLIENTHINT_OPENGL:
		logstr += "D3DKMT_CLIENTHINT_OPENGL\n";
		break;
	case D3DKMT_CLIENTHINT_CDD:
		logstr += "D3DKMT_CLIENTHINT_CDD\n";
		break;
	case D3DKMT_CLIENTHINT_DX7:
		logstr += "D3DKMT_CLIENTHINT_DX7\n";
		break;
	case D3DKMT_CLIENTHINT_DX8:
		logstr += "D3DKMT_CLIENTHINT_DX8\n";
		break;
	case D3DKMT_CLIENTHINT_DX9:
		logstr += "D3DKMT_CLIENTHINT_DX9\n";
		break;
	case D3DKMT_CLIENTHINT_DX10:
		logstr += "D3DKMT_CLIENTHINT_DX10\n";
		break;
	default:
		logstr += FormatDWORD(Arg1->ClientHint) + "\n";
		break;
	}

	logstr += "\tmember D3DKMT_HANDLE hContext " + FormatDWORD(Arg1->hContext) + "\n";
	logstr += FormatPVOID((char*)Arg1->pCommandBuffer, Arg1->CommandBufferSize, 1, "pCommandBuffer");
	logstr += "\tmember UINT CommandBufferSize " + FormatDWORD(Arg1->CommandBufferSize) + "\n";

	logstr += "\tstart list\n";
	D3DDDI_ALLOCATIONLIST *cur_allocation;
	for (UINT i = 0; i < Arg1->AllocationListSize; i++)
	{
		logstr += "\t\tstart struct D3DDDI_ALLOCATIONLIST\n";
		cur_allocation = Arg1->pAllocationList + i;
		logstr += "\t\t\tmember D3DKMT_HANDLE hAllocation " + FormatDWORD(cur_allocation->hAllocation) + "\n";
		logstr += "\t\t\tmember UINT Value " + FormatDWORD(cur_allocation->Value) + "\n";
		logstr += "\t\tend struct D3DDDI_ALLOCATIONLIST\n";
	}
	logstr += "\tend list\n";
	logstr += "\tmember UINT AllocationListSize " + FormatDWORD(Arg1->AllocationListSize) + "\n";

	logstr += "\tstart list\n";
	D3DDDI_PATCHLOCATIONLIST *cur_patchlocaton;
	for (UINT i = 0; i < Arg1->PatchLocationListSize; i++)
	{
		logstr += "\t\tstart struct D3DDDI_PATCHLOCATIONLIST\n";
		cur_patchlocaton = Arg1->pPatchLocationList + i;
		logstr += "\t\t\tmember UINT AllocationIndex " + FormatDWORD(cur_patchlocaton->AllocationIndex) + "\n";
		logstr += "\t\t\tmember UINT Value " + FormatDWORD(cur_patchlocaton->Value) + "\n";
		logstr += "\t\t\tmember UINT DriverId " + FormatDWORD(cur_patchlocaton->DriverId) + "\n";
		logstr += "\t\t\tmember UINT AllocationOffset " + FormatDWORD(cur_patchlocaton->AllocationOffset) + "\n";
		logstr += "\t\t\tmember UINT PatchOffset " + FormatDWORD(cur_patchlocaton->PatchOffset) + "\n";
		logstr += "\t\t\tmember UINT SplitOffset " + FormatDWORD(cur_patchlocaton->SplitOffset) + "\n";
		logstr += "\t\tend struct D3DDDI_PATCHLOCATIONLIST\n";
	}
	logstr += "\tend list\n";
	logstr += "\tmember UINT PatchLocationListSize " + FormatDWORD(Arg1->PatchLocationListSize) + "\n";

	logstr += "\tmember D3DGPU_VIRTUAL_ADDRESS CommandBuffer " + FormatULONG64(Arg1->CommandBuffer) + "\n";
	logstr += "end struct D3DKMT_CREATECONTEXT";
	log.info(logstr);
}

void SaveD3DKMT_RENDER(D3DKMT_RENDER  *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = "\nstart struct D3DKMT_RENDER ";
	logstr += GetLogStr(inout) + "\n";

	logstr += "\tmember D3DKMT_HANDLE hDevice " + FormatDWORD(Arg1->hDevice) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hContext " + FormatDWORD(Arg1->hContext) + "\n";
	logstr += "\tmember UINT CommandOffset " + FormatDWORD(Arg1->CommandOffset) + "\n";
	logstr += "\tmember UINT CommandLength " + FormatDWORD(Arg1->CommandLength) + "\n";
	logstr += "\tmember UINT AllocationCount " + FormatDWORD(Arg1->AllocationCount) + "\n";
	logstr += "\tmember UINT PatchLocationCount " + FormatDWORD(Arg1->PatchLocationCount) + "\n";

	logstr += FormatPVOID((char*)Arg1->pNewCommandBuffer, Arg1->NewCommandBufferSize, 1, "pNewCommandBuffer");
	logstr += "\tmember UINT NewCommandBufferSize " + FormatDWORD(Arg1->NewCommandBufferSize) + "\n";

	logstr += "\tstart list\n";
	D3DDDI_ALLOCATIONLIST *cur_allocation;
	for (UINT i = 0; i < Arg1->NewAllocationListSize; i++)
	{
		logstr += "\t\tstart struct D3DDDI_ALLOCATIONLIST\n";
		cur_allocation = Arg1->pNewAllocationList + i;
		logstr += "\t\t\tmember D3DKMT_HANDLE hAllocation " + FormatDWORD(cur_allocation->hAllocation) + "\n";
		logstr += "\t\t\tmember UINT Value " + FormatDWORD(cur_allocation->Value) + "\n";
		logstr += "\t\tend struct D3DDDI_ALLOCATIONLIST\n";
	}
	logstr += "\tend list\n";

	logstr += "\tstart list\n";
	D3DDDI_PATCHLOCATIONLIST *cur_patchlocaton;
	for (UINT i = 0; i < Arg1->NewPatchLocationListSize; i++)
	{
		logstr += "\t\tstart struct D3DDDI_PATCHLOCATIONLIST\n";
		cur_patchlocaton = Arg1->pNewPatchLocationList + i;
		logstr += "\t\t\tmember UINT AllocationIndex " + FormatDWORD(cur_patchlocaton->AllocationIndex) + "\n";
		logstr += "\t\t\tmember UINT Value " + FormatDWORD(cur_patchlocaton->Value) + "\n";
		logstr += "\t\t\tmember UINT DriverId " + FormatDWORD(cur_patchlocaton->DriverId) + "\n";
		logstr += "\t\t\tmember UINT AllocationOffset " + FormatDWORD(cur_patchlocaton->AllocationOffset) + "\n";
		logstr += "\t\t\tmember UINT PatchOffset " + FormatDWORD(cur_patchlocaton->PatchOffset) + "\n";
		logstr += "\t\t\tmember UINT SplitOffset " + FormatDWORD(cur_patchlocaton->SplitOffset) + "\n";
		logstr += "\t\tend struct D3DDDI_PATCHLOCATIONLIST\n";
	}
	logstr += "\tend list\n";

	logstr += "\tmember " + FormatDWORD(*((DWORD*)&Arg1->Flags)) + "\n";
	logstr += "\tmember ULONGLONG PresentHistoryToken " + FormatULONG64(Arg1->PresentHistoryToken) + "\n";
	logstr += "\tmember ULONG BroadcastContextCount " + FormatDWORD(Arg1->BroadcastContextCount) + "\n";
	logstr += "\tstart list\n";
	for (ULONG i = 0; i < Arg1->BroadcastContextCount; i++)
	{
		logstr += "\t\tmember D3DKMT_HANDLE " + FormatDWORD(Arg1->BroadcastContext[i]) + "\n";
	}
	logstr += "\tend list\n";
	logstr += "\tmember ULONG QueuedBufferCount " + FormatDWORD(Arg1->QueuedBufferCount) + "\n";
	logstr += "\tmember D3DGPU_VIRTUAL_ADDRESS NewCommandBuffer " + FormatULONG64(Arg1->NewCommandBuffer) + "\n";
	logstr += FormatPVOID((char*)Arg1->pPrivateDriverData, Arg1->PrivateDriverDataSize, 1, "pPrivateDriverData");
	logstr += "\tmember UINT PrivateDriverDataSize " + FormatDWORD(Arg1->PrivateDriverDataSize) + "\n";
	logstr += "end struct D3DKMT_RENDER";
	log.info(logstr);
}

void SaveD3DKMT_CREATECONTEXTVIRTUAL(D3DKMT_CREATECONTEXTVIRTUAL   *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = "\nstart struct D3DKMT_CREATECONTEXTVIRTUAL ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hDevice " + FormatDWORD(Arg1->hDevice) + "\n";
	logstr += "\tmember UINT NodeOrdinal " + FormatDWORD(Arg1->NodeOrdinal) + "\n";
	logstr += "\tmember UINT EngineAffinity " + FormatDWORD(Arg1->EngineAffinity) + "\n";
	logstr += "\tmember Flags " + FormatDWORD(*((DWORD*)&Arg1->Flags)) + "\n";

	logstr += FormatPVOID((char*)Arg1->pPrivateDriverData, Arg1->PrivateDriverDataSize, 1, "pPrivateDriverData");
	logstr += "\tmember UINT PrivateDriverDataSize " + FormatDWORD(Arg1->PrivateDriverDataSize) + "\n";

	logstr += "\tmember D3DKMT_CLIENTHINT ClientHint ";
	switch (Arg1->ClientHint)
	{
	case D3DKMT_CLIENTHINT_UNKNOWN:
		logstr += "D3DKMT_CLIENTHINT_UNKNOWN\n";
		break;
	case D3DKMT_CLIENTHINT_OPENGL:
		logstr += "D3DKMT_CLIENTHINT_OPENGL\n";
		break;
	case D3DKMT_CLIENTHINT_CDD:
		logstr += "D3DKMT_CLIENTHINT_CDD\n";
		break;
	case D3DKMT_CLIENTHINT_DX7:
		logstr += "D3DKMT_CLIENTHINT_DX7\n";
		break;
	case D3DKMT_CLIENTHINT_DX8:
		logstr += "D3DKMT_CLIENTHINT_DX8\n";
		break;
	case D3DKMT_CLIENTHINT_DX9:
		logstr += "D3DKMT_CLIENTHINT_DX9\n";
		break;
	case D3DKMT_CLIENTHINT_DX10:
		logstr += "D3DKMT_CLIENTHINT_DX10\n";
		break;
	default:
		logstr += FormatDWORD(Arg1->ClientHint) + "\n";
		break;
	}

	logstr += "\tmember D3DKMT_HANDLE hContext " + FormatDWORD(Arg1->hContext) + "\n";
	logstr += "end struct D3DKMT_CREATECONTEXTVIRTUAL";
	log.info(logstr);
}

void SaveD3DKMT_ESCAPE(D3DKMT_ESCAPE    *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = "\nstart struct D3DKMT_ESCAPE ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hAdapter " + FormatDWORD(Arg1->hAdapter) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hDevice " + FormatDWORD(Arg1->hDevice) + "\n";
	logstr += "\tmember D3DKMT_ESCAPETYPE Type " + FormatDWORD(Arg1->Type) + "\n";
	logstr += "\tmember Flags " + FormatDWORD(*((DWORD*)&Arg1->Flags)) + "\n";

	logstr += FormatPVOID((char*)Arg1->pPrivateDriverData, Arg1->PrivateDriverDataSize, 1, "pPrivateDriverData");
	logstr += "\tmember UINT PrivateDriverDataSize " + FormatDWORD(Arg1->PrivateDriverDataSize) + "\n";

	logstr += "\tmember D3DKMT_HANDLE hContext " + FormatDWORD(Arg1->hContext) + "\n";
	logstr += "end struct D3DKMT_ESCAPE";
	log.info(logstr);
}

void SaveD3DKMT_SUBMITCOMMAND(D3DKMT_SUBMITCOMMAND     *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = "\nstart struct D3DKMT_SUBMITCOMMAND ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DGPU_VIRTUAL_ADDRESS Commands " + FormatULONG64(Arg1->Commands) + "\n";
	logstr += "\tmember UINT CommandLength " + FormatDWORD(Arg1->CommandLength) + "\n";
	logstr += "\tmember Flags " + FormatDWORD(*((DWORD*)&Arg1->Flags)) + "\n";
	logstr += "\tmember ULONGLONG PresentHistoryToken " + FormatULONG64(Arg1->PresentHistoryToken) + "\n";
	logstr += "\tmember UINT BroadcastContextCount " + FormatDWORD(Arg1->BroadcastContextCount) + "\n";
	logstr += "\tstart list\n";
	for (ULONG i = 0; i < Arg1->BroadcastContextCount; i++)
	{
		logstr += "\t\tmember D3DKMT_HANDLE " + FormatDWORD(Arg1->BroadcastContext[i]) + "\n";
	}
	logstr += "\tend list\n";

	logstr += FormatPVOID((char*)Arg1->pPrivateDriverData, Arg1->PrivateDriverDataSize, 1, "pPrivateDriverData");
	logstr += "\tmember UINT PrivateDriverDataSize " + FormatDWORD(Arg1->PrivateDriverDataSize) + "\n";

	logstr += "\tmember UINT NumPrimaries " + FormatDWORD(Arg1->NumPrimaries) + "\n";
	logstr += "\tstart list\n";
	for (ULONG i = 0; i < Arg1->NumPrimaries; i++)
	{
		logstr += "\t\tmember D3DKMT_HANDLE " + FormatDWORD(Arg1->WrittenPrimaries[i]) + "\n";
	}
	logstr += "\tend list\n";

	logstr += "\tmember UINT NumHistoryBuffers " + FormatDWORD(Arg1->NumHistoryBuffers) + "\n";
	logstr += "\tstart list\n";
	for (ULONG i = 0; i < Arg1->NumHistoryBuffers; i++)
	{
		logstr += "\t\tmember D3DKMT_HANDLE " + FormatDWORD(Arg1->HistoryBufferArray[i]) + "\n";
	}
	logstr += "\tend list\n";

	logstr += "end struct D3DKMT_SUBMITCOMMAND";
	log.info(logstr);
}