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
	Save##ArgName(Arg1, root);\
	BOOL ret = OrgFunName##(Arg1);\
	return ret;\
}

#define DDI_HOOK_FUN_DEF(OrgFunName, ArgName) \
DDIFakeFunType OrgFunName##Org = (DDIFakeFunType)GetProcAddress(GetModuleHandle(TEXT("win32u")), #OrgFunName);\
BOOL WINAPI OrgFunName##Fake(##ArgName *Arg1)\
{\
	log4cpp::Category& root = log4cpp::Category::getRoot();\
	Save##ArgName(Arg1, root);\
	BOOL ret = OrgFunName##Org((void*)Arg1);\
	return ret;\
}

void SaveD3DKMT_CREATEALLOCATION(D3DKMT_CREATEALLOCATION *Arg1, log4cpp::Category& log);
void SaveD3DKMT_CREATEDEVICE(D3DKMT_CREATEDEVICE  *Arg1, log4cpp::Category& log);
void SaveD3DKMT_CREATECONTEXT(D3DKMT_CREATECONTEXT  *Arg1, log4cpp::Category& log);
void SaveD3DKMT_RENDER(D3DKMT_RENDER  *Arg1, log4cpp::Category& log);
void SaveD3DKMT_CREATECONTEXTVIRTUAL(D3DKMT_CREATECONTEXTVIRTUAL   *Arg1, log4cpp::Category& log);
void SaveD3DKMT_ESCAPE(D3DKMT_ESCAPE    *Arg1, log4cpp::Category& log);
void SaveD3DKMT_SUBMITCOMMAND(D3DKMT_SUBMITCOMMAND     *Arg1, log4cpp::Category& log);

//NtGdiDdDDI
HOOK_FUN_DEF(D3DKMTCreateDevice, D3DKMT_CREATEDEVICE)
DDI_HOOK_FUN_DEF(NtGdiDdDDICreateDevice, D3DKMT_CREATEDEVICE)

HOOK_FUN_DEF(D3DKMTCreateAllocation, D3DKMT_CREATEALLOCATION)
DDI_HOOK_FUN_DEF(NtGdiDdDDICreateAllocation, D3DKMT_CREATEALLOCATION)

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

void SaveD3DKMT_CREATEALLOCATION(D3DKMT_CREATEALLOCATION *Arg1, log4cpp::Category& log)
{
	log.info("SaveD3DKMT_CREATEALLOCATION");
}

void SaveD3DKMT_CREATEDEVICE(D3DKMT_CREATEDEVICE  *Arg1, log4cpp::Category& log)
{
	log.info("SaveD3DKMT_CREATEDEVICE");
}

void SaveD3DKMT_CREATECONTEXT(D3DKMT_CREATECONTEXT  *Arg1, log4cpp::Category& log)
{
	log.info("SaveD3DKMT_CREATECONTEXT");
}

void SaveD3DKMT_RENDER(D3DKMT_RENDER  *Arg1, log4cpp::Category& log)
{
	log.info("SaveD3DKMT_RENDER");
}

void SaveD3DKMT_CREATECONTEXTVIRTUAL(D3DKMT_CREATECONTEXTVIRTUAL   *Arg1, log4cpp::Category& log)
{
	log.info("SaveD3DKMT_CREATECONTEXTVIRTUAL");
}

void SaveD3DKMT_ESCAPE(D3DKMT_ESCAPE    *Arg1, log4cpp::Category& log)
{
	log.info("SaveD3DKMT_ESCAPE");
}

void SaveD3DKMT_SUBMITCOMMAND(D3DKMT_SUBMITCOMMAND     *Arg1, log4cpp::Category& log)
{

}