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
using DDIShareObjectsFunType = BOOL(WINAPI*)(
	UINT                cObjects,
	const D3DKMT_HANDLE *hObjects,
	POBJECT_ATTRIBUTES  pObjectAttributes,
	DWORD               dwDesiredAccess,
	HANDLE              *phSharedNtHandle);
using DDINOArgFakeFunType = BOOL(WINAPI*)();

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

#define TEST_DDI_HOOK_FUN_DEF(OrgFunName) \
DDIFakeFunType OrgFunName##Org = (DDIFakeFunType)GetProcAddress(GetModuleHandle(TEXT("win32u")), #OrgFunName);\
BOOL WINAPI OrgFunName##Fake(void *Arg1)\
{\
	log4cpp::Category& root = log4cpp::Category::getRoot();\
	root.info("Test " #OrgFunName);\
	BOOL ret = OrgFunName##Org((void*)Arg1);\
	return ret;\
}

#define TEST_NOARG_DDI_HOOK_FUN_DEF(OrgFunName) \
DDINOArgFakeFunType OrgFunName##Org = (DDINOArgFakeFunType)GetProcAddress(GetModuleHandle(TEXT("win32u")), #OrgFunName);\
BOOL WINAPI OrgFunName##Fake()\
{\
	log4cpp::Category& root = log4cpp::Category::getRoot();\
	root.info(#OrgFunName);\
	BOOL ret = OrgFunName##Org();\
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
void SaveD3DKMT_ENUMADAPTERS(D3DKMT_ENUMADAPTERS     *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_QUERYADAPTERINFO(D3DKMT_QUERYADAPTERINFO *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_GETDISPLAYMODELIST(D3DKMT_GETDISPLAYMODELIST *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_ENUMADAPTERS2(D3DKMT_ENUMADAPTERS2 *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_CLOSEADAPTER(D3DKMT_CLOSEADAPTER  *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_GET_MULTIPLANE_OVERLAY_CAPS(D3DKMT_GET_MULTIPLANE_OVERLAY_CAPS   *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_CREATEPAGINGQUEUE(D3DKMT_CREATEPAGINGQUEUE *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_CREATESYNCHRONIZATIONOBJECT(D3DKMT_CREATESYNCHRONIZATIONOBJECT *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_LOCK2(D3DKMT_LOCK2 *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_UNLOCK2(D3DKMT_UNLOCK2 *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DDDI_MAPGPUVIRTUALADDRESS(D3DDDI_MAPGPUVIRTUALADDRESS *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DDDI_MAKERESIDENT(D3DDDI_MAKERESIDENT *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_SIGNALSYNCHRONIZATIONOBJECT(D3DKMT_SIGNALSYNCHRONIZATIONOBJECT *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_GETDEVICESTATE(D3DKMT_GETDEVICESTATE *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_CHECKMONITORPOWERSTATE(D3DKMT_CHECKMONITORPOWERSTATE *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_QUERYRESOURCEINFO(D3DKMT_QUERYRESOURCEINFO *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_OPENRESOURCE(D3DKMT_OPENRESOURCE *Arg1, int type, LogType inout);
void SaveD3DKMT_OFFERALLOCATIONS(D3DKMT_OFFERALLOCATIONS *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_PRESENT(D3DKMT_PRESENT *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_RECLAIMALLOCATIONS2(D3DKMT_RECLAIMALLOCATIONS2 *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_DESTROYALLOCATION2(D3DKMT_DESTROYALLOCATION2 *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_DESTROYSYNCHRONIZATIONOBJECT(D3DKMT_DESTROYSYNCHRONIZATIONOBJECT *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_DESTROYCONTEXT(D3DKMT_DESTROYCONTEXT *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DDDI_DESTROYPAGINGQUEUE(D3DDDI_DESTROYPAGINGQUEUE *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_DESTROYDEVICE(D3DKMT_DESTROYDEVICE *Arg1, log4cpp::Category& log, LogType inout);
void SaveShareObjectsOrg(
	UINT                cObjects,
	const D3DKMT_HANDLE *hObjects,
	POBJECT_ATTRIBUTES  pObjectAttributes,
	DWORD               dwDesiredAccess,
	HANDLE              *phSharedNtHandle,
	log4cpp::Category& log,
	LogType inout);
void SaveD3DKMT_SETQUEUEDLIMIT(D3DKMT_SETQUEUEDLIMIT *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_GET_POST_COMPOSITION_CAPS(D3DKMT_GET_POST_COMPOSITION_CAPS *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_SIGNALSYNCHRONIZATIONOBJECTFROMGPU2(D3DKMT_SIGNALSYNCHRONIZATIONOBJECTFROMGPU2 *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_WAITFORSYNCHRONIZATIONOBJECTFROMGPU(D3DKMT_WAITFORSYNCHRONIZATIONOBJECTFROMGPU *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_WAITFORSYNCHRONIZATIONOBJECTFROMCPU(D3DKMT_WAITFORSYNCHRONIZATIONOBJECTFROMCPU *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_FREEGPUVIRTUALADDRESS(D3DKMT_FREEGPUVIRTUALADDRESS *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_QUERYRESOURCEINFOFROMNTHANDLE(D3DKMT_QUERYRESOURCEINFOFROMNTHANDLE *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_OPENRESOURCEFROMNTHANDLE(D3DKMT_OPENRESOURCEFROMNTHANDLE *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_OPENSYNCOBJECTFROMNTHANDLE2(D3DKMT_OPENSYNCOBJECTFROMNTHANDLE2 *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_LOCK(D3DKMT_LOCK *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_UNLOCK(D3DKMT_UNLOCK *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_SETCONTEXTINPROCESSSCHEDULINGPRIORITY(D3DKMT_SETCONTEXTINPROCESSSCHEDULINGPRIORITY *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_SIGNALSYNCHRONIZATIONOBJECTFROMCPU(D3DKMT_SIGNALSYNCHRONIZATIONOBJECTFROMCPU *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_EVICT(D3DKMT_EVICT *Arg1, log4cpp::Category& log, LogType inout);
//NtGdiDdDDI

DDI_HOOK_FUN_DEF(NtGdiDdDDICreateDevice, D3DKMT_CREATEDEVICE)
//DDI_HOOK_FUN_DEF(NtGdiDdDDICreateAllocation, D3DKMT_CREATEALLOCATION)
#define SET_DWORD(p, i, v) *((DWORD*)p + i) = v
BOOL WINAPI D3DKMTCreateAllocationFake(D3DKMT_CREATEALLOCATION *Arg1)
{
	NTSTATUS ret = 0;

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
	ret = D3DKMTCreateAllocation(Arg1);
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
		type = 1;
		SaveD3DKMT_CREATEALLOCATION(Arg1, 1, in);
	}
	BOOL ret = NtGdiDdDDICreateAllocationOrg((void*)Arg1);
	SaveD3DKMT_CREATEALLOCATION(Arg1, type, out);
	return ret;
}

DDI_HOOK_FUN_DEF(NtGdiDdDDICreateContext, D3DKMT_CREATECONTEXT)
DDI_HOOK_FUN_DEF(NtGdiDdDDIRender, D3DKMT_RENDER)
DDI_HOOK_FUN_DEF(NtGdiDdDDICreateContextVirtual, D3DKMT_CREATECONTEXTVIRTUAL)
DDI_HOOK_FUN_DEF(NtGdiDdDDIEscape, D3DKMT_ESCAPE)
DDI_HOOK_FUN_DEF(NtGdiDdDDISubmitCommand, D3DKMT_SUBMITCOMMAND)
DDI_HOOK_FUN_DEF(NtGdiDdDDIEnumAdapters, D3DKMT_ENUMADAPTERS)
DDI_HOOK_FUN_DEF(NtGdiDdDDIQueryAdapterInfo, D3DKMT_QUERYADAPTERINFO)
DDI_HOOK_FUN_DEF(NtGdiDdDDIGetDisplayModeList, D3DKMT_GETDISPLAYMODELIST)
DDI_HOOK_FUN_DEF(NtGdiDdDDIEnumAdapters2, D3DKMT_ENUMADAPTERS2)
DDI_HOOK_FUN_DEF(NtGdiDdDDICloseAdapter, D3DKMT_CLOSEADAPTER)
DDI_HOOK_FUN_DEF(NtGdiDdDDIGetMultiPlaneOverlayCaps, D3DKMT_GET_MULTIPLANE_OVERLAY_CAPS)
DDI_HOOK_FUN_DEF(NtGdiDdDDICreatePagingQueue, D3DKMT_CREATEPAGINGQUEUE)
DDI_HOOK_FUN_DEF(NtGdiDdDDICreateSynchronizationObject, D3DKMT_CREATESYNCHRONIZATIONOBJECT)
DDI_HOOK_FUN_DEF(NtGdiDdDDILock2, D3DKMT_LOCK2)
DDI_HOOK_FUN_DEF(NtGdiDdDDIMapGpuVirtualAddress, D3DDDI_MAPGPUVIRTUALADDRESS)
DDI_HOOK_FUN_DEF(NtGdiDdDDIMakeResident, D3DDDI_MAKERESIDENT)
DDI_HOOK_FUN_DEF(NtGdiDdDDIUnlock2, D3DKMT_UNLOCK2)
DDI_HOOK_FUN_DEF(NtGdiDdDDISignalSynchronizationObject, D3DKMT_SIGNALSYNCHRONIZATIONOBJECT)
DDI_HOOK_FUN_DEF(NtGdiDdDDIGetDeviceState, D3DKMT_GETDEVICESTATE)
DDI_HOOK_FUN_DEF(NtGdiDdDDICheckMonitorPowerState, D3DKMT_CHECKMONITORPOWERSTATE)
DDI_HOOK_FUN_DEF(NtGdiDdDDIQueryResourceInfo, D3DKMT_QUERYRESOURCEINFO)
//DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenResource, D3DKMT_OPENRESOURCE)
DDIFakeFunType NtGdiDdDDIOpenResourceOrg = (DDIFakeFunType)GetProcAddress(GetModuleHandle(TEXT("win32u")), "NtGdiDdDDIOpenResource");
BOOL WINAPI NtGdiDdDDIOpenResourceFake(D3DKMT_OPENRESOURCE *Arg1)
{
	//log4cpp::Category& root = log4cpp::Category::getRoot();
	int type = 2;
	__try
	{
		SaveD3DKMT_OPENRESOURCE(Arg1, 2, in);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		type = 1;
		SaveD3DKMT_OPENRESOURCE(Arg1, 1, in);
	}
	BOOL ret = NtGdiDdDDIOpenResourceOrg((void*)Arg1);
	SaveD3DKMT_OPENRESOURCE(Arg1, type, out);
	return ret;
}
DDIShareObjectsFunType NtGdiDdDDIShareObjectsOrg = (DDIShareObjectsFunType)GetProcAddress(GetModuleHandle(TEXT("win32u")), "NtGdiDdDDIShareObjects");
BOOL WINAPI NtGdiDdDDIShareObjectsFake(
	UINT                cObjects,
	const D3DKMT_HANDLE *hObjects,
	POBJECT_ATTRIBUTES  pObjectAttributes,
	DWORD               dwDesiredAccess,
	HANDLE              *phSharedNtHandle)
{
	log4cpp::Category& root = log4cpp::Category::getRoot();
	SaveShareObjectsOrg(cObjects, hObjects, pObjectAttributes, dwDesiredAccess, phSharedNtHandle, root, in);
	BOOL ret = NtGdiDdDDIShareObjectsOrg(cObjects, hObjects, pObjectAttributes, dwDesiredAccess, phSharedNtHandle);
	SaveShareObjectsOrg(cObjects, hObjects, pObjectAttributes, dwDesiredAccess, phSharedNtHandle, root, out);
	return ret;
}

DDI_HOOK_FUN_DEF(NtGdiDdDDIOfferAllocations, D3DKMT_OFFERALLOCATIONS)
DDI_HOOK_FUN_DEF(NtGdiDdDDIPresent, D3DKMT_PRESENT)
DDI_HOOK_FUN_DEF(NtGdiDdDDIReclaimAllocations2, D3DKMT_RECLAIMALLOCATIONS2)
DDI_HOOK_FUN_DEF(NtGdiDdDDIDestroyAllocation2, D3DKMT_DESTROYALLOCATION2)
DDI_HOOK_FUN_DEF(NtGdiDdDDIDestroySynchronizationObject, D3DKMT_DESTROYSYNCHRONIZATIONOBJECT)
DDI_HOOK_FUN_DEF(NtGdiDdDDIDestroyContext, D3DKMT_DESTROYCONTEXT)
DDI_HOOK_FUN_DEF(NtGdiDdDDIDestroyPagingQueue, D3DDDI_DESTROYPAGINGQUEUE)
DDI_HOOK_FUN_DEF(NtGdiDdDDIDestroyDevice, D3DKMT_DESTROYDEVICE)
DDI_HOOK_FUN_DEF(NtGdiDdDDISetQueuedLimit, D3DKMT_SETQUEUEDLIMIT)
DDI_HOOK_FUN_DEF(NtGdiDdDDIGetPostCompositionCaps, D3DKMT_GET_POST_COMPOSITION_CAPS)
DDI_HOOK_FUN_DEF(NtGdiDdDDISignalSynchronizationObjectFromGpu2, D3DKMT_SIGNALSYNCHRONIZATIONOBJECTFROMGPU2)
DDI_HOOK_FUN_DEF(NtGdiDdDDIWaitForSynchronizationObjectFromGpu, D3DKMT_WAITFORSYNCHRONIZATIONOBJECTFROMGPU)
DDI_HOOK_FUN_DEF(NtGdiDdDDIWaitForSynchronizationObjectFromCpu, D3DKMT_WAITFORSYNCHRONIZATIONOBJECTFROMCPU)
DDI_HOOK_FUN_DEF(NtGdiDdDDIFreeGpuVirtualAddress, D3DKMT_FREEGPUVIRTUALADDRESS)
DDI_HOOK_FUN_DEF(NtGdiDdDDIQueryResourceInfoFromNtHandle, D3DKMT_QUERYRESOURCEINFOFROMNTHANDLE)
DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenResourceFromNtHandle, D3DKMT_OPENRESOURCEFROMNTHANDLE)
DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenSyncObjectFromNtHandle2, D3DKMT_OPENSYNCOBJECTFROMNTHANDLE2)
DDI_HOOK_FUN_DEF(NtGdiDdDDILock, D3DKMT_LOCK)
DDI_HOOK_FUN_DEF(NtGdiDdDDIUnlock, D3DKMT_UNLOCK)
DDI_HOOK_FUN_DEF(NtGdiDdDDISetContextInProcessSchedulingPriority, D3DKMT_SETCONTEXTINPROCESSSCHEDULINGPRIORITY)
DDI_HOOK_FUN_DEF(NtGdiDdDDISignalSynchronizationObjectFromCpu, D3DKMT_SIGNALSYNCHRONIZATIONOBJECTFROMCPU)
DDI_HOOK_FUN_DEF(NtGdiDdDDIEvict, D3DKMT_EVICT)

TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIAbandonSwapChain)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIAcquireKeyedMutex)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIAcquireKeyedMutex2)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIAcquireSwapChain)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIAddSurfaceToSwapChain)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIAdjustFullscreenGamma)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICacheHybridQueryValue)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIChangeVideoMemoryReservation)
TEST_NOARG_DDI_HOOK_FUN_DEF(NtGdiDdDDICheckExclusiveOwnership)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICheckMultiPlaneOverlaySupport)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICheckMultiPlaneOverlaySupport2)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICheckMultiPlaneOverlaySupport3)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICheckOcclusion)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICheckSharedResourceAccess)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICheckVidPnExclusiveOwnership)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIConfigureSharedResource)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICreateBundleObject)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICreateDCFromMemory)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICreateHwContext)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICreateHwQueue)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICreateKeyedMutex)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICreateKeyedMutex2)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICreateOutputDupl)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICreateOverlay)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICreateProtectedSession)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICreateSwapChain)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIDDisplayEnum)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIDestroyAllocation)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIDestroyHwContext)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIDestroyHwQueue)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIDestroyKeyedMutex)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIDestroyOutputDupl)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIDestroyOverlay)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIDestroyProtectedSession)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIDispMgrCreate)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIDispMgrSourceOperation)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIDispMgrTargetOperation)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIExtractBundleObject)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIFlipOverlay)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIFlushHeapTransitions)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetAllocationPriority)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetCachedHybridQueryValue)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetContextInProcessSchedulingPriority)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetContextSchedulingPriority)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetDWMVerticalBlankEvent)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetMemoryBudgetTarget)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetMultisampleMethodList)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetOverlayState)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetPresentHistory)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetPresentQueueEvent)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetProcessDeviceRemovalSupport)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetProcessSchedulingPriorityBand)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetProcessSchedulingPriorityClass)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetResourcePresentPrivateDriverData)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetRuntimeData)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetScanLine)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetSetSwapChainMetadata)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetSharedPrimaryHandle)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetSharedResourceAdapterLuid)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetSharedResourceAdapterLuidFlipManager)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetYieldPercentage)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIInvalidateActiveVidPn)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIInvalidateCache)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIMarkDeviceAsError)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDINetDispGetNextChunkInfo)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDINetDispQueryMiracastDisplayDeviceStatus)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDINetDispQueryMiracastDisplayDeviceSupport)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDINetDispStartMiracastDisplayDevice)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDINetDispStopMiracastDisplayDevice)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDINetDispStopSessions)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenAdapterFromDeviceName)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenAdapterFromHdc)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenAdapterFromLuid)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenBundleObjectNtHandleFromName)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenKeyedMutex)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenKeyedMutex2)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenKeyedMutexFromNtHandle)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenNtHandleFromName)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenProtectedSessionFromNtHandle)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenSwapChain)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenSyncObjectFromNtHandle)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenSyncObjectNtHandleFromName)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenSynchronizationObject)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOutputDuplGetFrameInfo)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOutputDuplGetMetaData)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOutputDuplGetPointerShapeData)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOutputDuplPresent)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOutputDuplReleaseFrame)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIPinDirectFlipResources)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIPollDisplayChildren)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIPresentMultiPlaneOverlay)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIPresentMultiPlaneOverlay2)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIPresentMultiPlaneOverlay3)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIPresentRedirected)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIQueryAllocationResidency)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIQueryClockCalibration)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIQueryFSEBlock)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIQueryProcessOfferInfo)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIQueryProtectedSessionInfoFromNtHandle)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIQueryProtectedSessionStatus)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIQueryRemoteVidPnSourceFromGdiDisplayName)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIQueryStatistics)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIQueryVidPnExclusiveOwnership)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIQueryVideoMemoryInfo)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIReclaimAllocations)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIReleaseKeyedMutex)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIReleaseKeyedMutex2)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIReleaseProcessVidPnSourceOwners)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIReleaseSwapChain)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIRemoveSurfaceFromSwapChain)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIReserveGpuVirtualAddress)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISetAllocationPriority)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISetContextSchedulingPriority)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISetDisplayMode)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISetDisplayPrivateDriverFormat)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISetDodIndirectSwapchain)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISetFSEBlock)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISetGammaRamp)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISetHwProtectionTeardownRecovery)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISetMemoryBudgetTarget)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISetMonitorColorSpaceTransform)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISetProcessDeviceRemovalSupport)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISetProcessSchedulingPriorityBand)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISetProcessSchedulingPriorityClass)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISetStablePowerState)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISetStereoEnabled)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISetSyncRefreshCountWaitTarget)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISetVidPnSourceHwProtection)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISetVidPnSourceOwner)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISetYieldPercentage)
//TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIShareObjects)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISharedPrimaryLockNotification)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISharedPrimaryUnLockNotification)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISignalSynchronizationObjectFromGpu)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISubmitCommandToHwQueue)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISubmitSignalSyncObjectsToHwQueue)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISubmitWaitForSyncObjectsToHwQueue)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDITrimProcessCommitment)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIUnOrderedPresentSwapChain)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIUnpinDirectFlipResources)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIUpdateAllocationProperty)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIUpdateGpuVirtualAddress)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIUpdateOverlay)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIWaitForIdle)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIWaitForSynchronizationObject)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIWaitForVerticalBlankEvent)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIWaitForVerticalBlankEvent2)




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
	//std::string initFileName = "D:\\DXTest\\log4cpp.properties";
	log4cpp::PropertyConfigurator::configure(initFileName);
	log4cpp::Category& root = log4cpp::Category::getRoot();
	root.info("------------------Hook start-------------------------\n\n");

	root.info( "Injected by process Id: %d", inRemoteInfo->HostPID);

	// Perform hooking
	NTSTATUS result = 0;
	ULONG ACLEntries[1] = { 0 };
	
	
	HOOK_FUN(NtGdiDdDDIAbandonSwapChain, NtGdiDdDDIAbandonSwapChainFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIAcquireKeyedMutex, NtGdiDdDDIAcquireKeyedMutexFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIAcquireKeyedMutex2, NtGdiDdDDIAcquireKeyedMutex2Fake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIAcquireSwapChain, NtGdiDdDDIAcquireSwapChainFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIAddSurfaceToSwapChain, NtGdiDdDDIAddSurfaceToSwapChainFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIAdjustFullscreenGamma, NtGdiDdDDIAdjustFullscreenGammaFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDICacheHybridQueryValue, NtGdiDdDDICacheHybridQueryValueFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIChangeVideoMemoryReservation, NtGdiDdDDIChangeVideoMemoryReservationFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDICheckExclusiveOwnership, NtGdiDdDDICheckExclusiveOwnershipFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDICheckMonitorPowerState, NtGdiDdDDICheckMonitorPowerStateFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDICheckMultiPlaneOverlaySupport, NtGdiDdDDICheckMultiPlaneOverlaySupportFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDICheckMultiPlaneOverlaySupport2, NtGdiDdDDICheckMultiPlaneOverlaySupport2Fake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDICheckMultiPlaneOverlaySupport3, NtGdiDdDDICheckMultiPlaneOverlaySupport3Fake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDICheckOcclusion, NtGdiDdDDICheckOcclusionFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDICheckSharedResourceAccess, NtGdiDdDDICheckSharedResourceAccessFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDICheckVidPnExclusiveOwnership, NtGdiDdDDICheckVidPnExclusiveOwnershipFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDICloseAdapter, NtGdiDdDDICloseAdapterFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIConfigureSharedResource, NtGdiDdDDIConfigureSharedResourceFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDICreateAllocation, NtGdiDdDDICreateAllocationFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDICreateBundleObject, NtGdiDdDDICreateBundleObjectFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDICreateContext, NtGdiDdDDICreateContextFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDICreateContextVirtual, NtGdiDdDDICreateContextVirtualFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDICreateDCFromMemory, NtGdiDdDDICreateDCFromMemoryFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDICreateDevice, NtGdiDdDDICreateDeviceFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDICreateHwContext, NtGdiDdDDICreateHwContextFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDICreateHwQueue, NtGdiDdDDICreateHwQueueFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDICreateKeyedMutex, NtGdiDdDDICreateKeyedMutexFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDICreateKeyedMutex2, NtGdiDdDDICreateKeyedMutex2Fake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDICreateOutputDupl, NtGdiDdDDICreateOutputDuplFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDICreateOverlay, NtGdiDdDDICreateOverlayFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDICreatePagingQueue, NtGdiDdDDICreatePagingQueueFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDICreateProtectedSession, NtGdiDdDDICreateProtectedSessionFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDICreateSwapChain, NtGdiDdDDICreateSwapChainFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDICreateSynchronizationObject, NtGdiDdDDICreateSynchronizationObjectFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIDDisplayEnum, NtGdiDdDDIDDisplayEnumFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIDestroyAllocation, NtGdiDdDDIDestroyAllocationFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIDestroyAllocation2, NtGdiDdDDIDestroyAllocation2Fake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIDestroyContext, NtGdiDdDDIDestroyContextFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIDestroyDevice, NtGdiDdDDIDestroyDeviceFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIDestroyHwContext, NtGdiDdDDIDestroyHwContextFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIDestroyHwQueue, NtGdiDdDDIDestroyHwQueueFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIDestroyKeyedMutex, NtGdiDdDDIDestroyKeyedMutexFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIDestroyOutputDupl, NtGdiDdDDIDestroyOutputDuplFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIDestroyOverlay, NtGdiDdDDIDestroyOverlayFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIDestroyPagingQueue, NtGdiDdDDIDestroyPagingQueueFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIDestroyProtectedSession, NtGdiDdDDIDestroyProtectedSessionFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIDestroySynchronizationObject, NtGdiDdDDIDestroySynchronizationObjectFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIDispMgrCreate, NtGdiDdDDIDispMgrCreateFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIDispMgrSourceOperation, NtGdiDdDDIDispMgrSourceOperationFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIDispMgrTargetOperation, NtGdiDdDDIDispMgrTargetOperationFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIEnumAdapters, NtGdiDdDDIEnumAdaptersFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIEnumAdapters2, NtGdiDdDDIEnumAdapters2Fake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIEscape, NtGdiDdDDIEscapeFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIEvict, NtGdiDdDDIEvictFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIExtractBundleObject, NtGdiDdDDIExtractBundleObjectFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIFlipOverlay, NtGdiDdDDIFlipOverlayFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIFlushHeapTransitions, NtGdiDdDDIFlushHeapTransitionsFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIFreeGpuVirtualAddress, NtGdiDdDDIFreeGpuVirtualAddressFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIGetAllocationPriority, NtGdiDdDDIGetAllocationPriorityFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIGetCachedHybridQueryValue, NtGdiDdDDIGetCachedHybridQueryValueFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIGetContextInProcessSchedulingPriority, NtGdiDdDDIGetContextInProcessSchedulingPriorityFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIGetContextSchedulingPriority, NtGdiDdDDIGetContextSchedulingPriorityFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIGetDWMVerticalBlankEvent, NtGdiDdDDIGetDWMVerticalBlankEventFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIGetDeviceState, NtGdiDdDDIGetDeviceStateFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIGetDisplayModeList, NtGdiDdDDIGetDisplayModeListFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIGetMemoryBudgetTarget, NtGdiDdDDIGetMemoryBudgetTargetFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIGetMultiPlaneOverlayCaps, NtGdiDdDDIGetMultiPlaneOverlayCapsFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIGetMultisampleMethodList, NtGdiDdDDIGetMultisampleMethodListFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIGetOverlayState, NtGdiDdDDIGetOverlayStateFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIGetPostCompositionCaps, NtGdiDdDDIGetPostCompositionCapsFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIGetPresentHistory, NtGdiDdDDIGetPresentHistoryFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIGetPresentQueueEvent, NtGdiDdDDIGetPresentQueueEventFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIGetProcessDeviceRemovalSupport, NtGdiDdDDIGetProcessDeviceRemovalSupportFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIGetProcessSchedulingPriorityBand, NtGdiDdDDIGetProcessSchedulingPriorityBandFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIGetProcessSchedulingPriorityClass, NtGdiDdDDIGetProcessSchedulingPriorityClassFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIGetResourcePresentPrivateDriverData, NtGdiDdDDIGetResourcePresentPrivateDriverDataFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIGetRuntimeData, NtGdiDdDDIGetRuntimeDataFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIGetScanLine, NtGdiDdDDIGetScanLineFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIGetSetSwapChainMetadata, NtGdiDdDDIGetSetSwapChainMetadataFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIGetSharedPrimaryHandle, NtGdiDdDDIGetSharedPrimaryHandleFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIGetSharedResourceAdapterLuid, NtGdiDdDDIGetSharedResourceAdapterLuidFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIGetSharedResourceAdapterLuidFlipManager, NtGdiDdDDIGetSharedResourceAdapterLuidFlipManagerFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIGetYieldPercentage, NtGdiDdDDIGetYieldPercentageFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIInvalidateActiveVidPn, NtGdiDdDDIInvalidateActiveVidPnFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIInvalidateCache, NtGdiDdDDIInvalidateCacheFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDILock, NtGdiDdDDILockFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDILock2, NtGdiDdDDILock2Fake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIMakeResident, NtGdiDdDDIMakeResidentFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIMapGpuVirtualAddress, NtGdiDdDDIMapGpuVirtualAddressFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIMarkDeviceAsError, NtGdiDdDDIMarkDeviceAsErrorFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDINetDispGetNextChunkInfo, NtGdiDdDDINetDispGetNextChunkInfoFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDINetDispQueryMiracastDisplayDeviceStatus, NtGdiDdDDINetDispQueryMiracastDisplayDeviceStatusFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDINetDispQueryMiracastDisplayDeviceSupport, NtGdiDdDDINetDispQueryMiracastDisplayDeviceSupportFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDINetDispStartMiracastDisplayDevice, NtGdiDdDDINetDispStartMiracastDisplayDeviceFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDINetDispStopMiracastDisplayDevice, NtGdiDdDDINetDispStopMiracastDisplayDeviceFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDINetDispStopSessions, NtGdiDdDDINetDispStopSessionsFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIOfferAllocations, NtGdiDdDDIOfferAllocationsFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIOpenAdapterFromDeviceName, NtGdiDdDDIOpenAdapterFromDeviceNameFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIOpenAdapterFromHdc, NtGdiDdDDIOpenAdapterFromHdcFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIOpenAdapterFromLuid, NtGdiDdDDIOpenAdapterFromLuidFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIOpenBundleObjectNtHandleFromName, NtGdiDdDDIOpenBundleObjectNtHandleFromNameFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIOpenKeyedMutex, NtGdiDdDDIOpenKeyedMutexFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIOpenKeyedMutex2, NtGdiDdDDIOpenKeyedMutex2Fake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIOpenKeyedMutexFromNtHandle, NtGdiDdDDIOpenKeyedMutexFromNtHandleFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIOpenNtHandleFromName, NtGdiDdDDIOpenNtHandleFromNameFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIOpenProtectedSessionFromNtHandle, NtGdiDdDDIOpenProtectedSessionFromNtHandleFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIOpenResource, NtGdiDdDDIOpenResourceFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIOpenResourceFromNtHandle, NtGdiDdDDIOpenResourceFromNtHandleFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIOpenSwapChain, NtGdiDdDDIOpenSwapChainFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIOpenSyncObjectFromNtHandle, NtGdiDdDDIOpenSyncObjectFromNtHandleFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIOpenSyncObjectFromNtHandle2, NtGdiDdDDIOpenSyncObjectFromNtHandle2Fake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIOpenSyncObjectNtHandleFromName, NtGdiDdDDIOpenSyncObjectNtHandleFromNameFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIOpenSynchronizationObject, NtGdiDdDDIOpenSynchronizationObjectFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIOutputDuplGetFrameInfo, NtGdiDdDDIOutputDuplGetFrameInfoFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIOutputDuplGetMetaData, NtGdiDdDDIOutputDuplGetMetaDataFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIOutputDuplGetPointerShapeData, NtGdiDdDDIOutputDuplGetPointerShapeDataFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIOutputDuplPresent, NtGdiDdDDIOutputDuplPresentFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIOutputDuplReleaseFrame, NtGdiDdDDIOutputDuplReleaseFrameFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIPinDirectFlipResources, NtGdiDdDDIPinDirectFlipResourcesFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIPollDisplayChildren, NtGdiDdDDIPollDisplayChildrenFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIPresent, NtGdiDdDDIPresentFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIPresentMultiPlaneOverlay, NtGdiDdDDIPresentMultiPlaneOverlayFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIPresentMultiPlaneOverlay2, NtGdiDdDDIPresentMultiPlaneOverlay2Fake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIPresentMultiPlaneOverlay3, NtGdiDdDDIPresentMultiPlaneOverlay3Fake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIPresentRedirected, NtGdiDdDDIPresentRedirectedFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIQueryAdapterInfo, NtGdiDdDDIQueryAdapterInfoFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIQueryAllocationResidency, NtGdiDdDDIQueryAllocationResidencyFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIQueryClockCalibration, NtGdiDdDDIQueryClockCalibrationFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIQueryFSEBlock, NtGdiDdDDIQueryFSEBlockFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIQueryProcessOfferInfo, NtGdiDdDDIQueryProcessOfferInfoFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIQueryProtectedSessionInfoFromNtHandle, NtGdiDdDDIQueryProtectedSessionInfoFromNtHandleFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIQueryProtectedSessionStatus, NtGdiDdDDIQueryProtectedSessionStatusFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIQueryRemoteVidPnSourceFromGdiDisplayName, NtGdiDdDDIQueryRemoteVidPnSourceFromGdiDisplayNameFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIQueryResourceInfo, NtGdiDdDDIQueryResourceInfoFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIQueryResourceInfoFromNtHandle, NtGdiDdDDIQueryResourceInfoFromNtHandleFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIQueryStatistics, NtGdiDdDDIQueryStatisticsFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIQueryVidPnExclusiveOwnership, NtGdiDdDDIQueryVidPnExclusiveOwnershipFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIQueryVideoMemoryInfo, NtGdiDdDDIQueryVideoMemoryInfoFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIReclaimAllocations, NtGdiDdDDIReclaimAllocationsFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIReclaimAllocations2, NtGdiDdDDIReclaimAllocations2Fake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIReleaseKeyedMutex, NtGdiDdDDIReleaseKeyedMutexFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIReleaseKeyedMutex2, NtGdiDdDDIReleaseKeyedMutex2Fake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIReleaseProcessVidPnSourceOwners, NtGdiDdDDIReleaseProcessVidPnSourceOwnersFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIReleaseSwapChain, NtGdiDdDDIReleaseSwapChainFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIRemoveSurfaceFromSwapChain, NtGdiDdDDIRemoveSurfaceFromSwapChainFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIRender, NtGdiDdDDIRenderFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIReserveGpuVirtualAddress, NtGdiDdDDIReserveGpuVirtualAddressFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISetAllocationPriority, NtGdiDdDDISetAllocationPriorityFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISetContextInProcessSchedulingPriority, NtGdiDdDDISetContextInProcessSchedulingPriorityFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISetContextSchedulingPriority, NtGdiDdDDISetContextSchedulingPriorityFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISetDisplayMode, NtGdiDdDDISetDisplayModeFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISetDisplayPrivateDriverFormat, NtGdiDdDDISetDisplayPrivateDriverFormatFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISetDodIndirectSwapchain, NtGdiDdDDISetDodIndirectSwapchainFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISetFSEBlock, NtGdiDdDDISetFSEBlockFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISetGammaRamp, NtGdiDdDDISetGammaRampFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISetHwProtectionTeardownRecovery, NtGdiDdDDISetHwProtectionTeardownRecoveryFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISetMemoryBudgetTarget, NtGdiDdDDISetMemoryBudgetTargetFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISetMonitorColorSpaceTransform, NtGdiDdDDISetMonitorColorSpaceTransformFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISetProcessDeviceRemovalSupport, NtGdiDdDDISetProcessDeviceRemovalSupportFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISetProcessSchedulingPriorityBand, NtGdiDdDDISetProcessSchedulingPriorityBandFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISetProcessSchedulingPriorityClass, NtGdiDdDDISetProcessSchedulingPriorityClassFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISetQueuedLimit, NtGdiDdDDISetQueuedLimitFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISetStablePowerState, NtGdiDdDDISetStablePowerStateFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISetStereoEnabled, NtGdiDdDDISetStereoEnabledFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISetSyncRefreshCountWaitTarget, NtGdiDdDDISetSyncRefreshCountWaitTargetFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISetVidPnSourceHwProtection, NtGdiDdDDISetVidPnSourceHwProtectionFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISetVidPnSourceOwner, NtGdiDdDDISetVidPnSourceOwnerFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISetYieldPercentage, NtGdiDdDDISetYieldPercentageFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIShareObjects, NtGdiDdDDIShareObjectsFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISharedPrimaryLockNotification, NtGdiDdDDISharedPrimaryLockNotificationFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISharedPrimaryUnLockNotification, NtGdiDdDDISharedPrimaryUnLockNotificationFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISignalSynchronizationObject, NtGdiDdDDISignalSynchronizationObjectFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISignalSynchronizationObjectFromCpu, NtGdiDdDDISignalSynchronizationObjectFromCpuFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISignalSynchronizationObjectFromGpu, NtGdiDdDDISignalSynchronizationObjectFromGpuFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISignalSynchronizationObjectFromGpu2, NtGdiDdDDISignalSynchronizationObjectFromGpu2Fake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISubmitCommand, NtGdiDdDDISubmitCommandFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISubmitCommandToHwQueue, NtGdiDdDDISubmitCommandToHwQueueFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISubmitSignalSyncObjectsToHwQueue, NtGdiDdDDISubmitSignalSyncObjectsToHwQueueFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDISubmitWaitForSyncObjectsToHwQueue, NtGdiDdDDISubmitWaitForSyncObjectsToHwQueueFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDITrimProcessCommitment, NtGdiDdDDITrimProcessCommitmentFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIUnOrderedPresentSwapChain, NtGdiDdDDIUnOrderedPresentSwapChainFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIUnlock, NtGdiDdDDIUnlockFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIUnlock2, NtGdiDdDDIUnlock2Fake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIUnpinDirectFlipResources, NtGdiDdDDIUnpinDirectFlipResourcesFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIUpdateAllocationProperty, NtGdiDdDDIUpdateAllocationPropertyFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIUpdateGpuVirtualAddress, NtGdiDdDDIUpdateGpuVirtualAddressFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIUpdateOverlay, NtGdiDdDDIUpdateOverlayFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIWaitForIdle, NtGdiDdDDIWaitForIdleFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIWaitForSynchronizationObject, NtGdiDdDDIWaitForSynchronizationObjectFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIWaitForSynchronizationObjectFromCpu, NtGdiDdDDIWaitForSynchronizationObjectFromCpuFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIWaitForSynchronizationObjectFromGpu, NtGdiDdDDIWaitForSynchronizationObjectFromGpuFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIWaitForVerticalBlankEvent, NtGdiDdDDIWaitForVerticalBlankEventFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIWaitForVerticalBlankEvent2, NtGdiDdDDIWaitForVerticalBlankEvent2Fake, win32u, root, result);


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

std::string GetTID()
{
	char tmp[20] = { 0 };
	DWORD tid = GetCurrentThreadId();
	sprintf_s(tmp, "%x", tid);
	return tmp;
}

std::string FormatDWORD(DWORD value)
{
	char strBuf[10] = { 0 };
	sprintf_s(strBuf, "%x", value);
	std::string ret = std::string(strBuf);
	return ret;
}

std::string FormatINT(int value)
{
	char strBuf[10] = { 0 };
	sprintf_s(strBuf, "%d", value);
	std::string ret = std::string(strBuf);
	return ret;
}

std::string FormatFloat(float value)
{
	char strBuf[30] = { 0 };
	sprintf_s(strBuf, "%f", value);
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
	if (bufSize == 0 || pBuf == 0)
	{
		pvoidStr += stepBuf;
		pvoidStr += "buffer size or pBuf is 0\n";
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
	std::string logstr = GetTID() + "\nstart struct D3DKMT_CREATEALLOCATION ";

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
	logstr += "\t\tmember Flags " + FormatDWORD(*((DWORD*)&Arg1->Flags)) + "\n";
	logstr += "\tend struct D3DKMT_CREATEALLOCATIONFLAGS Flags\n";

	logstr += "\t\tmember HANDLE hPrivateRuntimeResourceHandle " + FormatDWORD((DWORD)Arg1->hPrivateRuntimeResourceHandle) + "\n";
	logstr += "end struct D3DKMT_CREATEALLOCATION";
	root.info(logstr);
}

void SaveD3DKMT_CREATEDEVICE(D3DKMT_CREATEDEVICE  *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_CREATEDEVICE ";
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
	std::string logstr = GetTID() + "\nstart struct D3DKMT_CREATECONTEXT ";
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
	if (Arg1->pAllocationList != 0)
	{
		for (UINT i = 0; i < Arg1->AllocationListSize; i++)
		{
			logstr += "\t\tstart struct D3DDDI_ALLOCATIONLIST\n";
			cur_allocation = Arg1->pAllocationList + i;
			logstr += "\t\t\tmember D3DKMT_HANDLE hAllocation " + FormatDWORD(cur_allocation->hAllocation) + "\n";
			logstr += "\t\t\tmember UINT Value " + FormatDWORD(cur_allocation->Value) + "\n";
			logstr += "\t\tend struct D3DDDI_ALLOCATIONLIST\n";
		}
	}
	else
	{
		logstr += "\t\t member D3DDDI_ALLOCATIONLIST* pAllocationList 0\n";
	}
	logstr += "\tend list\n";
	logstr += "\tmember UINT AllocationListSize " + FormatDWORD(Arg1->AllocationListSize) + "\n";

	logstr += "\tstart list\n";
	if (Arg1->pPatchLocationList != 0)
	{
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
	}
	else
	{
		logstr += "\t\t member D3DDDI_PATCHLOCATIONLIST* pPatchLocationList 0\n";
	}
	logstr += "\tend list\n";
	logstr += "\tmember UINT PatchLocationListSize " + FormatDWORD(Arg1->PatchLocationListSize) + "\n";

	logstr += "\tmember D3DGPU_VIRTUAL_ADDRESS CommandBuffer " + FormatULONG64(Arg1->CommandBuffer) + "\n";
	logstr += "end struct D3DKMT_CREATECONTEXT";
	log.info(logstr);
}

void SaveD3DKMT_RENDER(D3DKMT_RENDER  *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_RENDER ";
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
	std::string logstr = GetTID() + "\nstart struct D3DKMT_CREATECONTEXTVIRTUAL ";
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
	std::string logstr = GetTID() + "\nstart struct D3DKMT_ESCAPE ";
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
	std::string logstr = GetTID() + "\nstart struct D3DKMT_SUBMITCOMMAND ";
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

void SaveD3DKMT_ENUMADAPTERS(D3DKMT_ENUMADAPTERS     *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_ENUMADAPTERS ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember ULONG Commands " + FormatDWORD(Arg1->NumAdapters) + "\n";
	logstr += "\tstart list\n";
	for (ULONG i = 0; i < Arg1->NumAdapters; i++)
	{
		logstr += "\t\tmember D3DKMT_HANDLE " + FormatDWORD(Arg1->Adapters[i].hAdapter) + "\n";
	}
	logstr += "\tend list\n";

	logstr += "end struct D3DKMT_ENUMADAPTERS";
	log.info(logstr);
}

void SaveD3DKMT_QUERYADAPTERINFO(D3DKMT_QUERYADAPTERINFO *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_QUERYADAPTERINFO ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hAdapter " + FormatDWORD(Arg1->hAdapter) + "\n";
	logstr += "\tmember KMTQUERYADAPTERINFOTYPE Type " + FormatDWORD(Arg1->Type) + "\n";
	logstr += FormatPVOID((char*)Arg1->pPrivateDriverData, Arg1->PrivateDriverDataSize, 1, "pPrivateDriverData");
	logstr += "\tmember UINT PrivateDriverDataSize " + FormatDWORD(Arg1->PrivateDriverDataSize) + "\n";
	logstr += "end struct D3DKMT_QUERYADAPTERINFO";
	log.info(logstr);
}

void SaveD3DKMT_GETDISPLAYMODELIST(D3DKMT_GETDISPLAYMODELIST *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_GETDISPLAYMODELIST ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hAdapter " + FormatDWORD(Arg1->hAdapter) + "\n";
	logstr += "\tmember D3DDDI_VIDEO_PRESENT_SOURCE_ID VidPnSourceId " + FormatDWORD(Arg1->VidPnSourceId) + "\n";
	
	logstr += "\tstart list\n";
	if (Arg1->pModeList != 0)
	{
		for (ULONG i = 0; i < Arg1->ModeCount; i++)
		{
			logstr += "\t\tstart struct D3DKMT_DISPLAYMODE\n";
			logstr += "\t\t\tmember UINT Width " + FormatDWORD(Arg1->pModeList[i].Width) + "\n";
			logstr += "\t\t\tmember UINT Height " + FormatDWORD(Arg1->pModeList[i].Height) + "\n";
			logstr += "\t\t\tmember D3DDDIFORMAT Format " + FormatDWORD(Arg1->pModeList[i].Format) + "\n";
			logstr += "\t\t\tmember UINT IntegerRefreshRate " + FormatDWORD(Arg1->pModeList[i].IntegerRefreshRate) + "\n";
			logstr += "\t\t\tstart struct D3DDDI_RATIONAL RefreshRate\n";
			logstr += "\t\t\t\tmember UINT Numerator " + FormatDWORD(Arg1->pModeList[i].RefreshRate.Numerator) + "\n";
			logstr += "\t\t\t\tmember UINT Denominator " + FormatDWORD(Arg1->pModeList[i].RefreshRate.Denominator) + "\n";
			logstr += "\t\t\tend struct D3DDDI_RATIONAL RefreshRate\n";
			logstr += "\t\t\tmember D3DDDI_VIDEO_SIGNAL_SCANLINE_ORDERING ScanLineOrdering " + FormatDWORD(Arg1->pModeList[i].ScanLineOrdering) + "\n";
			logstr += "\t\t\tmember D3DDDI_ROTATION DisplayOrientation " + FormatDWORD(Arg1->pModeList[i].DisplayOrientation) + "\n";
			logstr += "\t\t\tmember UINT DisplayFixedOutput " + FormatDWORD(Arg1->pModeList[i].DisplayFixedOutput) + "\n";
			logstr += "\t\t\tmember UINT Flags " + FormatDWORD(*(DWORD*)&Arg1->pModeList[i].Flags) + "\n";
			logstr += "\t\tend struct D3DKMT_DISPLAYMODE\n";
		}
	}
	logstr += "\tend list\n";

	logstr += "\tmember UINT ModeCount " + FormatDWORD(Arg1->ModeCount) + "\n";
	logstr += "end struct D3DKMT_GETDISPLAYMODELIST";
	log.info(logstr);
}


void SaveD3DKMT_ENUMADAPTERS2(D3DKMT_ENUMADAPTERS2 *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_ENUMADAPTERS2 ";
	logstr += GetLogStr(inout) + "\n";

	logstr += "\tmember UINT NumAdapters " + FormatDWORD(Arg1->NumAdapters) + "\n";

	if (Arg1->pAdapters != 0)
	{
		logstr += "\tstart list\n";
		for (ULONG i = 0; i < Arg1->NumAdapters; i++)
		{
			logstr += "\t\tstart struct D3DKMT_ADAPTERINFO\n";
			logstr += "\t\t\tmember D3DKMT_HANDLE hAdapter " + FormatDWORD(Arg1->pAdapters[i].hAdapter) + "\n";
			logstr += "\t\tend struct D3DKMT_ADAPTERINFO\n";
		}
		logstr += "\tend list\n";
	}
	else
	{
		logstr += "\tmember PVOID pAdapters 0\n";
	}

	logstr += "end struct D3DKMT_ENUMADAPTERS2";
	log.info(logstr);
}

void SaveD3DKMT_CLOSEADAPTER(D3DKMT_CLOSEADAPTER  *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_CLOSEADAPTER ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hAdapter " + FormatDWORD(Arg1->hAdapter) + "\n";
	logstr += "end struct D3DKMT_CLOSEADAPTER";
	log.info(logstr);
}

void SaveD3DKMT_GET_MULTIPLANE_OVERLAY_CAPS(D3DKMT_GET_MULTIPLANE_OVERLAY_CAPS   *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_GET_MULTIPLANE_OVERLAY_CAPS ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hAdapter " + FormatDWORD(Arg1->hAdapter) + "\n";
	logstr += "\tmember D3DDDI_VIDEO_PRESENT_SOURCE_ID VidPnSourceId " + FormatDWORD(Arg1->VidPnSourceId) + "\n";
	logstr += "\tmember UINT MaxPlanes " + FormatDWORD(Arg1->MaxPlanes) + "\n";
	logstr += "\tmember UINT MaxRGBPlanes " + FormatDWORD(Arg1->MaxRGBPlanes) + "\n";
	logstr += "\tmember UINT MaxYUVPlanes " + FormatDWORD(Arg1->MaxYUVPlanes) + "\n";
	logstr += "\tmember D3DKMT_MULTIPLANE_OVERLAY_CAPS OverlayCaps " + FormatDWORD(*(DWORD*)&Arg1->OverlayCaps) + "\n";
	logstr += "\tmember float MaxStretchFactor " + FormatFloat(Arg1->MaxStretchFactor) + "\n";
	logstr += "\tmember float MaxShrinkFactor " + FormatFloat(Arg1->MaxShrinkFactor) + "\n";
	logstr += "end struct D3DKMT_GET_MULTIPLANE_OVERLAY_CAPS";

	log.info(logstr);
}

void SaveD3DKMT_CREATEPAGINGQUEUE(D3DKMT_CREATEPAGINGQUEUE    *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_CREATEPAGINGQUEUE ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hDevice " + FormatDWORD(Arg1->hDevice) + "\n";
	logstr += "\tmember D3DDDI_PAGINGQUEUE_PRIORITY Priority " + FormatDWORD(Arg1->Priority) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hPagingQueue " + FormatDWORD(Arg1->hPagingQueue) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hSyncObject " + FormatDWORD(Arg1->hSyncObject) + "\n";
	logstr += "\tmember VOID* MaxYUVPlanes " + FormatDWORD((DWORD)Arg1->FenceValueCPUVirtualAddress) + "\n";
	logstr += "\tmember UINT PhysicalAdapterIndex " + FormatDWORD(Arg1->PhysicalAdapterIndex) + "\n";
	logstr += "end struct D3DKMT_CREATEPAGINGQUEUE";

	log.info(logstr);
}

void SaveD3DKMT_CREATESYNCHRONIZATIONOBJECT(D3DKMT_CREATESYNCHRONIZATIONOBJECT    *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_CREATESYNCHRONIZATIONOBJECT ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hDevice " + FormatDWORD(Arg1->hDevice) + "\n";

	logstr += "\tstart struct D3DDDI_SYNCHRONIZATIONOBJECTINFO Info\n";
	logstr += "\t\tmember D3DDDI_SYNCHRONIZATIONOBJECT_TYPE Type " + FormatDWORD(*((DWORD*)&Arg1->Info.Type)) + "\n";
	logstr += "\t\tstart union\n";
	logstr += "\t\t\tmember BOOL InitialState " + FormatDWORD((DWORD)Arg1->Info.SynchronizationMutex.InitialState) + "\n";
	logstr += "\t\t\tmember UINT MaxCount " + FormatDWORD(Arg1->Info.Semaphore.MaxCount) + "\n";
	logstr += "\t\t\tmember UINT InitialCount " + FormatDWORD(Arg1->Info.Semaphore.InitialCount) + "\n";
	logstr += FormatPVOID((char*)&Arg1->Info.Reserved, 16 * sizeof(UINT), 3, "Reserved");
	logstr += "\t\tend union\n";
	logstr += "\tend struct D3DDDI_SYNCHRONIZATIONOBJECTINFO Info\n";

	logstr += "\tmember D3DKMT_HANDLE hSyncObject " + FormatDWORD(Arg1->hSyncObject) + "\n";
	logstr += "end struct D3DKMT_CREATESYNCHRONIZATIONOBJECT";

	log.info(logstr);
}

void SaveD3DKMT_LOCK2(D3DKMT_LOCK2 *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_LOCK2 ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hDevice " + FormatDWORD(Arg1->hDevice) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hAllocation " + FormatDWORD(Arg1->hAllocation) + "\n";
	logstr += "\tmember D3DDDICB_LOCK2FLAGS Flags " + FormatDWORD(*(DWORD*)&Arg1->Flags) + "\n";
	logstr += "\tmember PVOID pData " + FormatDWORD((DWORD)Arg1->pData) + "\n";
	logstr += "end struct D3DKMT_LOCK2";

	log.info(logstr);
}

void SaveD3DDDI_MAPGPUVIRTUALADDRESS(D3DDDI_MAPGPUVIRTUALADDRESS *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DDDI_MAPGPUVIRTUALADDRESS ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hPagingQueue " + FormatDWORD(Arg1->hPagingQueue) + "\n";
	logstr += "\tmember D3DGPU_VIRTUAL_ADDRESS BaseAddress " + FormatULONG64(Arg1->BaseAddress) + "\n";
	logstr += "\tmember D3DGPU_VIRTUAL_ADDRESS MinimumAddress " + FormatULONG64(Arg1->MinimumAddress) + "\n";
	logstr += "\tmember D3DGPU_VIRTUAL_ADDRESS MaximumAddress " + FormatULONG64(Arg1->MaximumAddress) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hAllocation " + FormatDWORD(Arg1->hAllocation) + "\n";
	logstr += "\tmember D3DGPU_SIZE_T OffsetInPages " + FormatULONG64(Arg1->OffsetInPages) + "\n";
	logstr += "\tmember D3DGPU_SIZE_T SizeInPages " + FormatULONG64(Arg1->SizeInPages) + "\n";
	logstr += "\tmember D3DDDIGPUVIRTUALADDRESS_PROTECTION_TYPE Protection " + FormatDWORD(*(DWORD*)&Arg1->Protection) + "\n";
	logstr += "\tmember UINT64 DriverProtection " + FormatULONG64(Arg1->DriverProtection) + "\n";
	logstr += "\tmember UINT Reserved0 " + FormatDWORD(Arg1->Reserved0) + "\n";
	logstr += "\tmember UINT64 Reserved1 " + FormatULONG64(Arg1->Reserved1) + "\n";
	logstr += "\tmember D3DGPU_VIRTUAL_ADDRESS VirtualAddress " + FormatULONG64(Arg1->VirtualAddress) + "\n";
	logstr += "\tmember UINT64 PagingFenceValue " + FormatULONG64(Arg1->PagingFenceValue) + "\n";
	logstr += "end struct D3DDDI_MAPGPUVIRTUALADDRESS";

	log.info(logstr);
}

void SaveD3DDDI_MAKERESIDENT(D3DDDI_MAKERESIDENT *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DDDI_MAKERESIDENT ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hPagingQueue " + FormatDWORD(Arg1->hPagingQueue) + "\n";
	logstr += "\tmember UINT NumAllocations " + FormatDWORD(Arg1->NumAllocations) + "\n";
	logstr += "\tstart list AllocationList\n";
	for (ULONG i = 0; i < Arg1->NumAllocations; i++)
	{
		logstr += "\t\tmember D3DKMT_HANDLE " + FormatDWORD(Arg1->AllocationList[i]) + "\n";
	}
	logstr += "\tend list AllocationList\n";
	logstr += "\tstart list PriorityList\n";
	if (Arg1->PriorityList != NULL)
	{
		for (ULONG i = 0; i < Arg1->NumAllocations; i++)
		{
			logstr += "\t\tmember D3DKMT_HANDLE " + FormatDWORD(Arg1->PriorityList[i]) + "\n";
		}
	}
	logstr += "\tend list PriorityList\n";

	logstr += "\tmember D3DDDI_MAKERESIDENT_FLAGS Flags " + FormatDWORD(*(DWORD*)&Arg1->Flags) + "\n";
	logstr += "\tmember UINT64 PagingFenceValue " + FormatULONG64(Arg1->PagingFenceValue) + "\n";
	logstr += "\tmember UINT64 NumBytesToTrim " + FormatULONG64(Arg1->NumBytesToTrim) + "\n";
	logstr += "end struct D3DDDI_MAKERESIDENT";

	log.info(logstr);
}

void SaveD3DKMT_UNLOCK2(D3DKMT_UNLOCK2 *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_UNLOCK2 ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hDevice " + FormatDWORD(Arg1->hDevice) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hAllocation " + FormatDWORD(Arg1->hAllocation) + "\n";
	logstr += "end struct D3DKMT_UNLOCK2";

	log.info(logstr);
}

void SaveD3DKMT_SIGNALSYNCHRONIZATIONOBJECT(D3DKMT_SIGNALSYNCHRONIZATIONOBJECT *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_SIGNALSYNCHRONIZATIONOBJECT ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hContext " + FormatDWORD(Arg1->hContext) + "\n";
	logstr += "\tmember UINT ObjectCount " + FormatDWORD(Arg1->ObjectCount) + "\n";
	logstr += "\tstart list ObjectHandleArray\n";
	for (ULONG i = 0; i < Arg1->ObjectCount; i++)
	{
		logstr += "\t\tmember D3DKMT_HANDLE " + FormatDWORD(Arg1->ObjectHandleArray[i]) + "\n";
	}
	logstr += "\tend list ObjectHandleArray\n";
	
	logstr += "\tmember D3DDDICB_SIGNALFLAGS Flags " + FormatDWORD(*(DWORD*)&Arg1->Flags) + "\n";
	logstr += "end struct D3DKMT_SIGNALSYNCHRONIZATIONOBJECT";

	log.info(logstr);
}

void SaveD3DKMT_GETDEVICESTATE(D3DKMT_GETDEVICESTATE *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_GETDEVICESTATE ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hDevice " + FormatDWORD(Arg1->hDevice) + "\n";
	logstr += "\tmember D3DKMT_DEVICESTATE_TYPE StateType " + FormatDWORD(*(DWORD*)&Arg1->StateType) + "\n";


	logstr += "\tstart union\n";
	logstr += "\t\tmember D3DKMT_DEVICEEXECUTION_STATE ExecutionState " + FormatDWORD(*(DWORD*)&Arg1->ExecutionState) + "\n";
	logstr += "\t\tmember D3DKMT_DEVICEPRESENT_STATE PresentState " + FormatDWORD(*(DWORD*)&Arg1->PresentState) + "\n";
	logstr += "\t\tmember D3DKMT_DEVICERESET_STATE ResetState " + FormatDWORD(*(DWORD*)&Arg1->ResetState) + "\n";

	logstr += "\t\tmember start struct D3DKMT_DEVICEPRESENT_STATE_DWM PresentStateDWM\n";
	logstr += "\t\t\tmember D3DDDI_VIDEO_PRESENT_SOURCE_ID VidPnSourceId " + FormatDWORD(Arg1->PresentStateDWM.VidPnSourceId) + "\n";
	logstr += "\t\t\tmember start struct D3DKMT_PRESENT_STATS_DWM PresentStatsDWM\n";
	logstr += "\t\t\t\tmember UINT PresentCount " + FormatDWORD(Arg1->PresentStateDWM.PresentStatsDWM.PresentCount) + "\n";
	logstr += "\t\t\t\tmember UINT PresentRefreshCount " + FormatDWORD(Arg1->PresentStateDWM.PresentStatsDWM.PresentRefreshCount) + "\n";
	logstr += "\t\t\t\tmember LARGE_INTEGER PresentQPCTime " + FormatULONG64(Arg1->PresentStateDWM.PresentStatsDWM.PresentQPCTime.QuadPart) + "\n";
	logstr += "\t\t\t\tmember UINT SyncRefreshCount " + FormatDWORD(Arg1->PresentStateDWM.PresentStatsDWM.SyncRefreshCount) + "\n";
	logstr += "\t\t\t\tmember LARGE_INTEGER SyncQPCTime " + FormatULONG64(Arg1->PresentStateDWM.PresentStatsDWM.SyncQPCTime.QuadPart) + "\n";
	logstr += "\t\t\t\tmember UINT CustomPresentDuration " + FormatDWORD(Arg1->PresentStateDWM.PresentStatsDWM.CustomPresentDuration) + "\n";
	logstr += "\t\t\tmember end struct D3DKMT_PRESENT_STATS_DWM PresentStatsDWM\n";
	logstr += "\t\tmember end struct D3DKMT_DEVICEPRESENT_STATE_DWM PresentStateDWM\n";

	logstr += "\t\tmember start struct D3DKMT_DEVICEPAGEFAULT_STATE PageFaultState\n";
	logstr += "\t\t\tmember UINT64 FaultedPrimitiveAPISequenceNumber " + FormatULONG64(Arg1->PageFaultState.FaultedPrimitiveAPISequenceNumber) + "\n";
	logstr += "\t\t\tmember DXGK_RENDER_PIPELINE_STAGE FaultedPipelineStage " + FormatDWORD(*(DWORD*)&Arg1->PageFaultState.FaultedPipelineStage) + "\n";
	logstr += "\t\t\tmember UINT FaultedBindTableEntry " + FormatDWORD(Arg1->PageFaultState.FaultedBindTableEntry) + "\n";
	logstr += "\t\t\tmember DXGK_PAGE_FAULT_FLAGS PageFaultFlags " + FormatDWORD(*(DWORD*)&Arg1->PageFaultState.PageFaultFlags) + "\n";
	logstr += "\t\t\tmember DXGK_FAULT_ERROR_CODE FaultErrorCode " + FormatDWORD(*(DWORD*)&Arg1->PageFaultState.FaultErrorCode) + "\n";
	logstr += "\t\t\tmember D3DGPU_VIRTUAL_ADDRESS FaultedVirtualAddress " + FormatULONG64(Arg1->PageFaultState.FaultedVirtualAddress) + "\n";
	logstr += "\t\tmember end struct D3DKMT_DEVICEPAGEFAULT_STATE PageFaultState\n";

	logstr += "\t\tmember start struct D3DKMT_DEVICEPRESENT_QUEUE_STATE PresentQueueState\n";
	logstr += "\t\t\tmember D3DDDI_VIDEO_PRESENT_SOURCE_ID VidPnSourceId " + FormatDWORD(Arg1->PresentQueueState.VidPnSourceId) + "\n";
	logstr += "\t\t\tmember BOOLEAN bQueuedPresentLimitReached " + FormatDWORD(Arg1->PresentQueueState.bQueuedPresentLimitReached) + "\n";
	logstr += "\t\tmember end struct D3DKMT_DEVICEPRESENT_QUEUE_STATE PresentQueueState\n";

	logstr += "\tend union\n";

	logstr += "end struct D3DKMT_GETDEVICESTATE";

	log.info(logstr);
}


void SaveD3DKMT_CHECKMONITORPOWERSTATE(D3DKMT_CHECKMONITORPOWERSTATE *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_CHECKMONITORPOWERSTATE ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hAdapter " + FormatDWORD(Arg1->hAdapter) + "\n";
	logstr += "\tmember D3DDDI_VIDEO_PRESENT_SOURCE_ID VidPnSourceId " + FormatDWORD(Arg1->VidPnSourceId) + "\n";
	logstr += "end struct D3DKMT_CHECKMONITORPOWERSTATE";

	log.info(logstr);
}

void SaveD3DKMT_QUERYRESOURCEINFO(D3DKMT_QUERYRESOURCEINFO *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = "\nstart struct D3DKMT_QUERYRESOURCEINFO ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hDevice " + FormatDWORD(Arg1->hDevice) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hGlobalShare " + FormatDWORD(Arg1->hGlobalShare) + "\n";
	logstr += FormatPVOID((char*)Arg1->pPrivateRuntimeData, Arg1->PrivateRuntimeDataSize, 1, "pPrivateRuntimeData");
	logstr += "\tmember UINT PrivateRuntimeDataSize " + FormatDWORD(Arg1->PrivateRuntimeDataSize) + "\n";
	logstr += "\tmember UINT TotalPrivateDriverDataSize " + FormatDWORD(Arg1->TotalPrivateDriverDataSize) + "\n";
	logstr += "\tmember UINT ResourcePrivateDriverDataSize " + FormatDWORD(Arg1->ResourcePrivateDriverDataSize) + "\n";
	logstr += "\tmember UINT NumAllocations " + FormatDWORD(Arg1->NumAllocations) + "\n";
	logstr += "end struct D3DKMT_QUERYRESOURCEINFO";

	log.info(logstr);
}


void SaveD3DKMT_OPENRESOURCE(D3DKMT_OPENRESOURCE *Arg1, int type, LogType inout)
{
	log4cpp::Category& root = log4cpp::Category::getRoot();
	//char tmpBuf[512] = { 0 };
	std::string logstr = "\nstart struct D3DKMT_OPENRESOURCE ";

	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hDevice " + FormatDWORD(Arg1->hDevice) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hGlobalShare " + FormatDWORD(Arg1->hGlobalShare) + "\n";
	
	logstr += "\tmember UINT NumAllocations " + FormatDWORD(Arg1->NumAllocations) + "\n";
	std::string tmpstr = "";

	//try D3DDDI_ALLOCATIONINFO2 

	if (type == 2)
	{
		tmpstr += "\tstart list\n";
		for (UINT i = 0; i < Arg1->NumAllocations; i++)
		{
			D3DDDI_OPENALLOCATIONINFO2 *cur_ptr = Arg1->pOpenAllocationInfo2 + i;
			tmpstr += "\t\tstart struct D3DDDI_OPENALLOCATIONINFO2\n";
			tmpstr += "\t\t\tmember D3DKMT_HANDLE hAllocation " + FormatDWORD(cur_ptr->hAllocation) + "\n";


			tmpstr += FormatPVOID((char*)cur_ptr->pPrivateDriverData, cur_ptr->PrivateDriverDataSize, 3, "pPrivateDriverData");
			tmpstr += "\t\t\tmember UINT PrivateDriverDataSize " + FormatDWORD(cur_ptr->PrivateDriverDataSize) + "\n";

			tmpstr += "\t\t\t\tmember D3DGPU_VIRTUAL_ADDRESS GpuVirtualAddress " + FormatDWORD((DWORD)cur_ptr->GpuVirtualAddress) + "\n";

			tmpstr += FormatPVOID((char*)cur_ptr->Reserved, 6 * sizeof(ULONG), 3, "Reserved");
			tmpstr += "\t\tend struct D3DDDI_OPENALLOCATIONINFO2\n";
		}
		tmpstr += "\tend list\n";
	}
	else
	{
		//is D3DDDI_ALLOCATIONINFO
		tmpstr += "\tstart list\n";
		for (UINT i = 0; i < Arg1->NumAllocations; i++)
		{
			D3DDDI_OPENALLOCATIONINFO  *cur_ptr = Arg1->pOpenAllocationInfo + i;
			tmpstr += "\t\tstart struct D3DDDI_OPENALLOCATIONINFO\n";
			tmpstr += "\t\t\tmember D3DKMT_HANDLE hAllocation " + FormatDWORD(cur_ptr->hAllocation) + "\n";


			tmpstr += FormatPVOID((char*)cur_ptr->pPrivateDriverData, cur_ptr->PrivateDriverDataSize, 3, "pPrivateDriverData");
			tmpstr += "\t\t\tmember UINT PrivateDriverDataSize " + FormatDWORD(cur_ptr->PrivateDriverDataSize) + "\n";
			tmpstr += "\t\tend struct D3DDDI_OPENALLOCATIONINFO\n";
		}
		tmpstr += "\tend list\n";
	}
	logstr += tmpstr;

	logstr += FormatPVOID((char*)Arg1->pPrivateRuntimeData, Arg1->PrivateRuntimeDataSize, 1, "pPrivateRuntimeData");
	logstr += "\tmember UINT PrivateRuntimeDataSize " + FormatDWORD(Arg1->PrivateRuntimeDataSize) + "\n";

	logstr += FormatPVOID((char*)Arg1->pResourcePrivateDriverData, Arg1->ResourcePrivateDriverDataSize, 1, "pResourcePrivateDriverData");
	logstr += "\tmember UINT ResourcePrivateDriverDataSize " + FormatDWORD(Arg1->ResourcePrivateDriverDataSize) + "\n";

	logstr += FormatPVOID((char*)Arg1->pTotalPrivateDriverDataBuffer, Arg1->TotalPrivateDriverDataBufferSize, 1, "pTotalPrivateDriverDataBuffer");
	logstr += "\tmember UINT TotalPrivateDriverDataBufferSize " + FormatDWORD(Arg1->TotalPrivateDriverDataBufferSize) + "\n";

	logstr += "\t\tmember D3DKMT_HANDLE hResource " + FormatDWORD((DWORD)Arg1->hResource) + "\n";
	logstr += "end struct D3DKMT_OPENRESOURCE";
	root.info(logstr);
}

void SaveD3DKMT_OFFERALLOCATIONS(D3DKMT_OFFERALLOCATIONS *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = "\nstart struct D3DKMT_OFFERALLOCATIONS ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hDevice " + FormatDWORD(Arg1->hDevice) + "\n";
	if (Arg1->pResources != 0)
	{
		logstr += "\tstart list pResources\n";
		for (ULONG i = 0; i < Arg1->NumAllocations; i++)
		{
			logstr += "\t\tmember D3DKMT_HANDLE " + FormatDWORD(Arg1->pResources[i]) + "\n";
		}
		logstr += "\tend list pResources\n";
	}
	else
	{
		logstr += "\tmember D3DKMT_HANDLE pResources 0\n";
	}

	if (Arg1->HandleList != 0)
	{
		logstr += "\tstart list HandleList\n";
		for (ULONG i = 0; i < Arg1->NumAllocations; i++)
		{
			logstr += "\t\tmember D3DKMT_HANDLE " + FormatDWORD(Arg1->HandleList[i]) + "\n";
		}
		logstr += "\tend list HandleList\n";
	}
	else
	{
		logstr += "\tmember D3DKMT_HANDLE HandleList 0\n";
	}

	logstr += "\tmember UINT NumAllocations " + FormatDWORD(Arg1->NumAllocations) + "\n";
	logstr += "\tmember D3DKMT_OFFER_PRIORITY Priority " + FormatDWORD(Arg1->Priority) + "\n";
	logstr += "\tmember Flags " + FormatDWORD(*((DWORD*)&Arg1->Flags)) + "\n";
	
	logstr += "end struct D3DKMT_OFFERALLOCATIONS";

	log.info(logstr);
}

void SaveD3DKMT_PRESENT(D3DKMT_PRESENT *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = "\nstart struct D3DKMT_PRESENT ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hDevice " + FormatDWORD(Arg1->hDevice) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hContext " + FormatDWORD(Arg1->hContext) + "\n";
	logstr += "\tmember HWND hWindow " + FormatDWORD((DWORD)Arg1->hWindow) + "\n";
	logstr += "\tmember D3DDDI_VIDEO_PRESENT_SOURCE_ID VidPnSourceId " + FormatDWORD(Arg1->VidPnSourceId) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hSource " + FormatDWORD(Arg1->hSource) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hDestination " + FormatDWORD(Arg1->hDestination) + "\n";
	logstr += "\tmember UINT Color " + FormatDWORD(Arg1->Color) + "\n";

	logstr += "\tstart struct RECT DstRect\n";
	logstr += "\t\tmember LONG left " + FormatINT(Arg1->DstRect.left) + "\n";
	logstr += "\t\tmember LONG top " + FormatINT(Arg1->DstRect.top) + "\n";
	logstr += "\t\tmember LONG right " + FormatINT(Arg1->DstRect.right) + "\n";
	logstr += "\t\tmember LONG bottom " + FormatINT(Arg1->DstRect.bottom) + "\n";
	logstr += "\tend struct RECT DstRect\n";

	logstr += "\tstart struct RECT SrcRect\n";
	logstr += "\t\tmember LONG left " + FormatINT(Arg1->SrcRect.left) + "\n";
	logstr += "\t\tmember LONG top " + FormatINT(Arg1->SrcRect.top) + "\n";
	logstr += "\t\tmember LONG right " + FormatINT(Arg1->SrcRect.right) + "\n";
	logstr += "\t\tmember LONG bottom " + FormatINT(Arg1->SrcRect.bottom) + "\n";
	logstr += "\tend struct RECT SrcRect\n";

	logstr += "\tmember UINT SubRectCnt " + FormatDWORD(Arg1->SubRectCnt) + "\n";
	if (Arg1->SubRectCnt != 0)
	{
		logstr += "\tstart list pSrcSubRects\n";
		for (ULONG i = 0; i < Arg1->SubRectCnt; i++)
		{
			logstr += "\t\tstart struct RECT SrcRect\n";
			logstr += "\t\t\tmember LONG left " + FormatINT(Arg1->pSrcSubRects[i].left) + "\n";
			logstr += "\t\t\tmember LONG top " + FormatINT(Arg1->pSrcSubRects[i].top) + "\n";
			logstr += "\t\t\tmember LONG right " + FormatINT(Arg1->pSrcSubRects[i].right) + "\n";
			logstr += "\t\t\tmember LONG bottom " + FormatINT(Arg1->pSrcSubRects[i].bottom) + "\n";
			logstr += "\t\tend struct RECT SrcRect\n";
		}
		logstr += "\tend list pSrcSubRects\n";
	}
	else
	{
		logstr += "\tmember RECT* pSrcSubRects 0\n";
	}

	logstr += "\tmember UINT PresentCount " + FormatDWORD(Arg1->PresentCount) + "\n";
	logstr += "\tmember D3DDDI_FLIPINTERVAL_TYPE FlipInterval " + FormatDWORD(Arg1->FlipInterval) + "\n";
	logstr += "\tmember D3DKMT_PRESENTFLAGS Flags " + FormatDWORD(*((DWORD*)&Arg1->Flags)) + "\n";
	logstr += "\tmember ULONG PresentCoBroadcastContextCountunt " + FormatDWORD(Arg1->BroadcastContextCount) + "\n";
	logstr += "\tstart list BroadcastContext\n";
	for (ULONG i = 0; i < Arg1->BroadcastContextCount; i++)
	{
		logstr += "\t\tmember D3DKMT_HANDLE " + FormatDWORD(Arg1->BroadcastContext[i]) + "\n";
	}
	logstr += "\tend list BroadcastContext\n";

	logstr += "\tmember HANDLE PresentLimitSemaphore " + FormatDWORD((DWORD)Arg1->PresentLimitSemaphore) + "\n";

	logstr += FormatPVOID((char*)&Arg1->PresentHistoryToken, sizeof(D3DKMT_PRESENTHISTORYTOKEN), 1, "PresentHistoryToken");
	
	if (Arg1->pPresentRegions != 0)
	{
		logstr += "\tstart struct D3DKMT_PRESENT_RGNS pPresentRegions\n";
		logstr += "\t\tmember UINT DirtyRectCount " + FormatDWORD(Arg1->pPresentRegions->DirtyRectCount) + "\n";
		logstr += "\t\tmember RECT* pDirtyRects " + FormatDWORD((DWORD)Arg1->pPresentRegions->pDirtyRects) + "\n";
		logstr += "\t\tmember UINT MoveRectCount " + FormatDWORD(Arg1->pPresentRegions->MoveRectCount) + "\n";
		logstr += "\t\tmember D3DKMT_MOVE_RECT* pMoveRects " + FormatDWORD((DWORD)Arg1->pPresentRegions->pMoveRects) + "\n";
		logstr += "\tend struct D3DKMT_PRESENT_RGNS pPresentRegions\n";
	}
	else
	{
		logstr += "\tmember D3DKMT_PRESENT_RGNS* pPresentRegions 0\n";
	}
	logstr += "\tmember D3DKMT_HANDLE hAdapter " + FormatDWORD(Arg1->hAdapter) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hIndirectContext " + FormatDWORD(Arg1->hIndirectContext) + "\n";
	logstr += "\tmember UINT Duration " + FormatDWORD(Arg1->Duration) + "\n";

	logstr += "\tstart list BroadcastSrcAllocation\n";
	for (ULONG i = 0; i < Arg1->BroadcastContextCount; i++)
	{
		logstr += "\t\tmember D3DKMT_HANDLE " + FormatDWORD(Arg1->BroadcastSrcAllocation[i]) + "\n";
	}
	logstr += "\tend list BroadcastSrcAllocation\n";

	logstr += "\tstart list BroadcastDstAllocation\n";
	for (ULONG i = 0; i < Arg1->BroadcastContextCount; i++)
	{
		logstr += "\t\tmember D3DKMT_HANDLE " + FormatDWORD(Arg1->BroadcastDstAllocation[i]) + "\n";
	}
	logstr += "\tend list BroadcastDstAllocation\n";

	logstr += "\tmember UINT PrivateDriverDataSize " + FormatDWORD(Arg1->PrivateDriverDataSize) + "\n";
	logstr += FormatPVOID((char*)Arg1->pPrivateDriverData, Arg1->PrivateDriverDataSize, 1, "pPrivateDriverData");

	logstr += "\tmember BOOLEAN bOptimizeForComposition " + FormatDWORD(Arg1->bOptimizeForComposition) + "\n";
	

	logstr += "end struct D3DKMT_PRESENT";

	log.info(logstr);
}

void SaveD3DKMT_RECLAIMALLOCATIONS2(D3DKMT_RECLAIMALLOCATIONS2 *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = "\nstart struct D3DKMT_RECLAIMALLOCATIONS2 ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hPagingQueue " + FormatDWORD(Arg1->hPagingQueue) + "\n";
	logstr += "\tmember UINT NumAllocations " + FormatDWORD(Arg1->NumAllocations) + "\n";

	if (Arg1->pResources != 0)
	{
		logstr += "\tstart list pResources\n";
		for (ULONG i = 0; i < Arg1->NumAllocations; i++)
		{
			logstr += "\t\tmember D3DKMT_HANDLE " + FormatDWORD(Arg1->pResources[i]) + "\n";
		}
		logstr += "\tend list pResources\n";
	}
	else
	{
		logstr += "\tmember D3DKMT_HANDLE pResources 0\n";
	}

	if (Arg1->HandleList != 0)
	{
		logstr += "\tstart list HandleList\n";
		for (ULONG i = 0; i < Arg1->NumAllocations; i++)
		{
			logstr += "\t\tmember D3DKMT_HANDLE " + FormatDWORD(Arg1->HandleList[i]) + "\n";
		}
		logstr += "\tend list HandleList\n";
	}
	else
	{
		logstr += "\tmember D3DKMT_HANDLE HandleList 0\n";
	}

	if (Arg1->pResults != 0)
	{
		logstr += "\tstart list pResults\n";
		for (ULONG i = 0; i < Arg1->NumAllocations; i++)
		{
			logstr += "\t\tmember D3DDDI_RECLAIM_RESULT " + FormatDWORD(Arg1->pResults[i]) + "\n";
		}
		logstr += "\tend list pResults\n";
	}
	else
	{
		logstr += "\tmember D3DDDI_RECLAIM_RESULT pResults 0\n";
	}

	logstr += "\tmember UINT64 PagingFenceValue " + FormatULONG64(Arg1->PagingFenceValue) + "\n";

	logstr += "end struct D3DKMT_RECLAIMALLOCATIONS2";

	log.info(logstr);
}

void SaveD3DKMT_DESTROYALLOCATION2(D3DKMT_DESTROYALLOCATION2 *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_DESTROYALLOCATION2 ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hDevice " + FormatDWORD(Arg1->hDevice) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hResource " + FormatDWORD(Arg1->hResource) + "\n";
	logstr += "\tstart list phAllocationList\n";
	for (ULONG i = 0; i < Arg1->AllocationCount; i++)
	{
		logstr += "\t\tmember D3DKMT_HANDLE " + FormatDWORD(Arg1->phAllocationList[i]) + "\n";
	}
	logstr += "\tend list phAllocationList\n";
	logstr += "\tmember UINT AllocationCount " + FormatDWORD(Arg1->AllocationCount) + "\n";
	logstr += "\tmember D3DDDICB_DESTROYALLOCATION2FLAGS Flags " + FormatDWORD(*((DWORD*)&Arg1->Flags)) + "\n";

	logstr += "end struct D3DKMT_DESTROYALLOCATION2";

	log.info(logstr);
}

void SaveD3DKMT_DESTROYSYNCHRONIZATIONOBJECT(D3DKMT_DESTROYSYNCHRONIZATIONOBJECT *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_DESTROYSYNCHRONIZATIONOBJECT ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hSyncObject " + FormatDWORD(Arg1->hSyncObject) + "\n";
	logstr += "end struct D3DKMT_DESTROYSYNCHRONIZATIONOBJECT";

	log.info(logstr);
}

void SaveD3DKMT_DESTROYCONTEXT(D3DKMT_DESTROYCONTEXT *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_DESTROYCONTEXT ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hContext " + FormatDWORD(Arg1->hContext) + "\n";
	logstr += "end struct D3DKMT_DESTROYCONTEXT";

	log.info(logstr);
}

void SaveD3DDDI_DESTROYPAGINGQUEUE(D3DDDI_DESTROYPAGINGQUEUE *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DDDI_DESTROYPAGINGQUEUE ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hPagingQueue " + FormatDWORD(Arg1->hPagingQueue) + "\n";
	logstr += "end struct D3DDDI_DESTROYPAGINGQUEUE";

	log.info(logstr);
}

void SaveD3DKMT_DESTROYDEVICE(D3DKMT_DESTROYDEVICE *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DDDI_DESTROYPAGINGQUEUE ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hDevice " + FormatDWORD(Arg1->hDevice) + "\n";
	logstr += "end struct D3DDDI_DESTROYPAGINGQUEUE";

	log.info(logstr);
}

void SaveShareObjectsOrg(
	UINT                cObjects,
	const D3DKMT_HANDLE *hObjects,
	POBJECT_ATTRIBUTES  pObjectAttributes,
	DWORD               dwDesiredAccess,
	HANDLE              *phSharedNtHandle,
	log4cpp::Category& log,
	LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct ShareObjectsOrg ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember UINT cObjects " + FormatDWORD(cObjects) + "\n";
	if (hObjects != NULL)
	{
		logstr += "\tmember UINT cObjects " + FormatDWORD(cObjects) + "\n";
		logstr += "\tstart list hObjects\n";
		for (UINT i = 0; i < cObjects; i++)
		{
			logstr += "\t\tmember D3DKMT_HANDLE " + FormatDWORD(*(hObjects + i)) + "\n";
		}
		logstr += "\tend list hObjects\n";
	}
	else
	{
		logstr += "\tmember D3DKMT_HANDLE hObjects 0";
	}
	

	logstr += "\tmember PVOID pObjectAttributes " + FormatDWORD((DWORD)pObjectAttributes) + "\n";
	logstr += "\tmember DWORD dwDesiredAccess " + FormatDWORD(dwDesiredAccess) + "\n";
	logstr += "\tmember HANDLE* phSharedNtHandle " + FormatDWORD((DWORD)phSharedNtHandle) + "\n";
	logstr += "end struct ShareObjectsOrg";

	log.info(logstr);
}


void SaveD3DKMT_SETQUEUEDLIMIT(D3DKMT_SETQUEUEDLIMIT *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_SETQUEUEDLIMIT ";
	logstr += GetLogStr(inout) + "\n";

	
	logstr += "\tmember D3DKMT_HANDLE hDevice " + FormatDWORD(Arg1->hDevice) + "\n";
	logstr += "\tmember D3DKMT_QUEUEDLIMIT_TYPE Type " + FormatDWORD(Arg1->Type) + "\n";
	logstr += "\tstart union\n";
	logstr += "\t\tmember UINT QueuedPresentLimit " + FormatDWORD(Arg1->QueuedPresentLimit) + "\n";
	logstr += "\t\tmember D3DDDI_VIDEO_PRESENT_SOURCE_ID VidPnSourceId " + FormatDWORD(Arg1->VidPnSourceId) + "\n";
	logstr += "\t\tmember UINT QueuedPendingFlipLimit " + FormatDWORD(Arg1->QueuedPendingFlipLimit) + "\n";
	logstr += "\tend union\n";
	logstr += "end struct D3DKMT_SETQUEUEDLIMIT";
	log.info(logstr);
}

void SaveD3DKMT_GET_POST_COMPOSITION_CAPS(D3DKMT_GET_POST_COMPOSITION_CAPS *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_GET_POST_COMPOSITION_CAPS ";
	logstr += GetLogStr(inout) + "\n";


	logstr += "\tmember D3DKMT_HANDLE hAdapter " + FormatDWORD(Arg1->hAdapter) + "\n";
	logstr += "\tmember D3DDDI_VIDEO_PRESENT_SOURCE_ID VidPnSourceId " + FormatDWORD(Arg1->VidPnSourceId) + "\n";
	logstr += "\tmember float MaxStretchFactor " + FormatFloat(Arg1->MaxStretchFactor) + "\n";
	logstr += "\tmember float MaxShrinkFactor " + FormatFloat(Arg1->MaxShrinkFactor) + "\n";
	logstr += "end struct D3DKMT_GET_POST_COMPOSITION_CAPS";
	log.info(logstr);
}

void SaveD3DKMT_SIGNALSYNCHRONIZATIONOBJECTFROMGPU2(D3DKMT_SIGNALSYNCHRONIZATIONOBJECTFROMGPU2 *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_SIGNALSYNCHRONIZATIONOBJECTFROMGPU2 ";
	logstr += GetLogStr(inout) + "\n";

	logstr += "\tmember UINT ObjectCount " + FormatDWORD(Arg1->ObjectCount) + "\n";
	logstr += "\tstart list ObjectHandleArray\n";
	if (Arg1->ObjectCount > 0 && Arg1->ObjectHandleArray != NULL)
	{
		for(UINT i = 0; i < Arg1->ObjectCount; i++)
			logstr += "\t\tmember D3DKMT_HANDLE " + FormatDWORD(*(Arg1->ObjectHandleArray + i)) + "\n";
	}
	else
	{
		logstr += "\t\tmember D3DKMT_HANDLE 0\n";
	}
	logstr += "\tend list ObjectHandleArray\n";


	logstr += "\tmember ULONG BroadcastContextCount " + FormatDWORD(Arg1->BroadcastContextCount) + "\n";
	logstr += "\tstart list BroadcastContextArray\n";
	if (Arg1->BroadcastContextCount > 0 && Arg1->BroadcastContextArray != NULL)
	{
		for (UINT i = 0; i < Arg1->BroadcastContextCount; i++)
			logstr += "\t\tmember D3DKMT_HANDLE " + FormatDWORD(*(Arg1->BroadcastContextArray + i)) + "\n";
	}
	else
	{
		logstr += "\t\tmember D3DKMT_HANDLE 0\n";
	}
	logstr += "\tend list BroadcastContextArray\n";

	logstr += "\tstart union\n";
	logstr += "\t\tmember UINT64 FenceValue " + FormatULONG64(Arg1->FenceValue) + "\n";
	logstr += "\t\tmember HANDLE CpuEventHandle " + FormatDWORD((DWORD)Arg1->CpuEventHandle) + "\n";
	logstr += "\t\tmember UINT64* MonitoredFenceValueArray " + FormatDWORD((DWORD)Arg1->MonitoredFenceValueArray) + "\n";
	logstr += FormatPVOID((char*)Arg1->Reserved, sizeof(UINT64) * 8, 1, "Reserved");
	logstr += "\tend union\n";
	logstr += "end struct D3DKMT_SIGNALSYNCHRONIZATIONOBJECTFROMGPU2";
	log.info(logstr);
}

void SaveD3DKMT_WAITFORSYNCHRONIZATIONOBJECTFROMGPU(D3DKMT_WAITFORSYNCHRONIZATIONOBJECTFROMGPU *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_WAITFORSYNCHRONIZATIONOBJECTFROMGPU ";
	logstr += GetLogStr(inout) + "\n";

	logstr += "\tmember UINT ObjectCount " + FormatDWORD(Arg1->ObjectCount) + "\n";
	logstr += "\tstart list ObjectHandleArray\n";
	if (Arg1->ObjectCount > 0 && Arg1->ObjectHandleArray != NULL)
	{
		for (UINT i = 0; i < Arg1->ObjectCount; i++)
			logstr += "\t\tmember D3DKMT_HANDLE " + FormatDWORD(*(Arg1->ObjectHandleArray + i)) + "\n";
	}
	else
	{
		logstr += "\t\tmember D3DKMT_HANDLE 0\n";
	}
	logstr += "\tend list ObjectHandleArray\n";


	logstr += "\tstart union\n";
	logstr += "\t\tmember UINT64 FenceValue " + FormatULONG64(Arg1->FenceValue) + "\n";
	logstr += "\t\tmember UINT64* MonitoredFenceValueArray " + FormatDWORD((DWORD)Arg1->MonitoredFenceValueArray) + "\n";
	logstr += FormatPVOID((char*)Arg1->Reserved, sizeof(UINT64) * 8, 1, "Reserved");
	logstr += "\tend union\n";
	logstr += "end struct D3DKMT_WAITFORSYNCHRONIZATIONOBJECTFROMGPU";
	log.info(logstr);
}

void SaveD3DKMT_WAITFORSYNCHRONIZATIONOBJECTFROMCPU(D3DKMT_WAITFORSYNCHRONIZATIONOBJECTFROMCPU *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_WAITFORSYNCHRONIZATIONOBJECTFROMCPU ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hDevice " + FormatDWORD(Arg1->hDevice) + "\n";
	logstr += "\tmember UINT ObjectCount " + FormatDWORD(Arg1->ObjectCount) + "\n";
	logstr += "\tstart list ObjectHandleArray\n";
	if (Arg1->ObjectCount > 0 && Arg1->ObjectHandleArray != NULL)
	{
		for (UINT i = 0; i < Arg1->ObjectCount; i++)
			logstr += "\t\tmember D3DKMT_HANDLE " + FormatDWORD(*(Arg1->ObjectHandleArray + i)) + "\n";
	}
	else
	{
		logstr += "\t\tmember D3DKMT_HANDLE 0\n";
	}
	logstr += "\tend list ObjectHandleArray\n";

	logstr += "\tstart list FenceValueArray\n";
	if (Arg1->ObjectCount > 0 && Arg1->FenceValueArray != NULL)
	{
		for (UINT i = 0; i < Arg1->ObjectCount; i++)
			logstr += "\t\tmember UINT64 " + FormatULONG64(*(Arg1->FenceValueArray + i)) + "\n";
	}
	else
	{
		logstr += "\t\tmember D3DKMT_HANDLE 0\n";
	}
	logstr += "\tend list FenceValueArray\n";


	logstr += "\tmember HANDLE hAsyncEvent " + FormatDWORD((DWORD)Arg1->hAsyncEvent) + "\n";
	logstr += "\tmember D3DDDI_WAITFORSYNCHRONIZATIONOBJECTFROMCPU_FLAGS Flags " + FormatDWORD((DWORD)Arg1->Flags.Value) + "\n";
	logstr += "end struct D3DKMT_WAITFORSYNCHRONIZATIONOBJECTFROMCPU";
	log.info(logstr);
}

void SaveD3DKMT_FREEGPUVIRTUALADDRESS(D3DKMT_FREEGPUVIRTUALADDRESS *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_FREEGPUVIRTUALADDRESS ";
	logstr += GetLogStr(inout) + "\n";


	logstr += "\tmember D3DKMT_HANDLE hAdapter " + FormatDWORD(Arg1->hAdapter) + "\n";
	logstr += "\tmember D3DGPU_VIRTUAL_ADDRESS BaseAddress " + FormatULONG64(Arg1->BaseAddress) + "\n";
	logstr += "\tmember D3DGPU_SIZE_T Size " + FormatULONG64(Arg1->Size) + "\n";
	logstr += "end struct D3DKMT_FREEGPUVIRTUALADDRESS";
	log.info(logstr);
}

void SaveD3DKMT_QUERYRESOURCEINFOFROMNTHANDLE(D3DKMT_QUERYRESOURCEINFOFROMNTHANDLE *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_QUERYRESOURCEINFOFROMNTHANDLE ";
	logstr += GetLogStr(inout) + "\n";


	logstr += "\tmember D3DKMT_HANDLE hDevice " + FormatDWORD(Arg1->hDevice) + "\n";
	logstr += "\tmember HANDLE hNtHandle " + FormatDWORD((DWORD)Arg1->hNtHandle) + "\n";
	logstr += FormatPVOID((char*)Arg1->pPrivateRuntimeData, Arg1->PrivateRuntimeDataSize, 1, "pPrivateRuntimeData");
	logstr += "\tmember UINT PrivateRuntimeDataSize " + FormatDWORD(Arg1->PrivateRuntimeDataSize) + "\n";
	logstr += "\tmember UINT TotalPrivateDriverDataSize " + FormatDWORD(Arg1->TotalPrivateDriverDataSize) + "\n";
	logstr += "\tmember UINT ResourcePrivateDriverDataSize " + FormatDWORD(Arg1->ResourcePrivateDriverDataSize) + "\n";
	logstr += "\tmember UINT NumAllocations " + FormatDWORD(Arg1->NumAllocations) + "\n";
	logstr += "end struct D3DKMT_QUERYRESOURCEINFOFROMNTHANDLE";
	log.info(logstr);
}


void SaveD3DKMT_OPENRESOURCEFROMNTHANDLE(D3DKMT_OPENRESOURCEFROMNTHANDLE *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_OPENRESOURCEFROMNTHANDLE ";
	logstr += GetLogStr(inout) + "\n";


	logstr += "\tmember D3DKMT_HANDLE hDevice " + FormatDWORD(Arg1->hDevice) + "\n";
	logstr += "\tmember HANDLE hNtHandle " + FormatDWORD((DWORD)Arg1->hNtHandle) + "\n";
	logstr += "\tmember UINT NumAllocations " + FormatDWORD(Arg1->NumAllocations) + "\n";
	if (Arg1->NumAllocations != 0 && Arg1->pOpenAllocationInfo2)
	{
		logstr += "\tstart list\n";
		for (UINT i = 0; i < Arg1->NumAllocations; i++)
		{
			/*D3DDDI_OPENALLOCATIONINFO2 *cur_ptr = Arg1->pOpenAllocationInfo2 + i;
			logstr += "\t\tstart struct D3DDDI_OPENALLOCATIONINFO2\n";
			logstr += "\t\t\tmember D3DKMT_HANDLE hAllocation " + FormatDWORD(cur_ptr->hAllocation) + "\n";

			logstr += FormatPVOID((char*)cur_ptr->pPrivateDriverData, cur_ptr->PrivateDriverDataSize, 3, "pPrivateDriverData");
			logstr += "\t\t\tmember UINT PrivateDriverDataSize " + FormatDWORD(cur_ptr->PrivateDriverDataSize) + "\n";

			logstr += "\t\t\t\tmember D3DGPU_VIRTUAL_ADDRESS GpuVirtualAddress " + FormatDWORD((DWORD)cur_ptr->GpuVirtualAddress) + "\n";

			logstr += FormatPVOID((char*)cur_ptr->Reserved, 6 * sizeof(ULONG_PTR), 3, "Reserved");
			logstr += "\t\tend struct D3DDDI_OPENALLOCATIONINFO2\n";*/
			logstr += "\t\tmember D3DDDI_OPENALLOCATIONINFO2* " + FormatDWORD((DWORD)(Arg1->pOpenAllocationInfo2 + i)) + "\n";
		}
		logstr += "\tend list\n";
	}

	logstr += FormatPVOID((char*)Arg1->pPrivateRuntimeData, Arg1->PrivateRuntimeDataSize, 1, "pPrivateRuntimeData");
	logstr += "\tmember UINT PrivateRuntimeDataSize " + FormatDWORD(Arg1->PrivateRuntimeDataSize) + "\n";
	logstr += FormatPVOID((char*)Arg1->pResourcePrivateDriverData, Arg1->ResourcePrivateDriverDataSize, 1, "pResourcePrivateDriverData");
	logstr += "\tmember UINT ResourcePrivateDriverDataSize " + FormatDWORD(Arg1->ResourcePrivateDriverDataSize) + "\n";
	logstr += FormatPVOID((char*)Arg1->pTotalPrivateDriverDataBuffer, Arg1->TotalPrivateDriverDataBufferSize, 1, "pTotalPrivateDriverDataBuffer");
	logstr += "\tmember UINT TotalPrivateDriverDataBufferSize " + FormatDWORD(Arg1->TotalPrivateDriverDataBufferSize) + "\n";

	logstr += "\tmember D3DKMT_HANDLE hResource " + FormatDWORD(Arg1->hResource) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hKeyedMutex " + FormatDWORD(Arg1->hKeyedMutex) + "\n";

	logstr += FormatPVOID((char*)Arg1->pKeyedMutexPrivateRuntimeData, Arg1->KeyedMutexPrivateRuntimeDataSize, 1, "pKeyedMutexPrivateRuntimeData");
	logstr += "\tmember UINT KeyedMutexPrivateRuntimeDataSize " + FormatDWORD(Arg1->KeyedMutexPrivateRuntimeDataSize) + "\n";

	logstr += "\tmember D3DKMT_HANDLE hSyncObject " + FormatDWORD(Arg1->hSyncObject) + "\n";
	logstr += "end struct D3DKMT_OPENRESOURCEFROMNTHANDLE";
	log.info(logstr);
}


void SaveD3DKMT_OPENSYNCOBJECTFROMNTHANDLE2(D3DKMT_OPENSYNCOBJECTFROMNTHANDLE2 *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_OPENSYNCOBJECTFROMNTHANDLE2 ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hDevice " + FormatDWORD(Arg1->hDevice) + "\n";
	logstr += "\tmember HANDLE hNtHandle " + FormatDWORD((DWORD)Arg1->hNtHandle) + "\n";
	logstr += "\tmember D3DDDI_SYNCHRONIZATIONOBJECT_FLAGS Flags " + FormatDWORD(Arg1->Flags.Value) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hSyncObject " + FormatDWORD(Arg1->hSyncObject) + "\n";

	logstr += "\tstart union\n";
	logstr += "\tmember VOID FenceValueCPUVirtualAddress " + FormatULONG64((ULONG64)Arg1->MonitoredFence.FenceValueCPUVirtualAddress) + "\n";
	logstr += "\tmember D3DGPU_VIRTUAL_ADDRESS FenceValueGPUVirtualAddress " + FormatULONG64(Arg1->MonitoredFence.FenceValueGPUVirtualAddress) + "\n";
	logstr += "\tmember UINT EngineAffinity " + FormatDWORD(Arg1->MonitoredFence.EngineAffinity) + "\n";
	logstr += FormatPVOID((char*)&Arg1->Reserved, 8 * sizeof(UINT64), 2, "Reserved");
	logstr += "\tend union\n";

	
	logstr += "end struct D3DKMT_OPENSYNCOBJECTFROMNTHANDLE2";

	log.info(logstr);
}

void SaveD3DKMT_LOCK(D3DKMT_LOCK *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_LOCK ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hDevice " + FormatDWORD(Arg1->hDevice) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hAllocation " + FormatDWORD(Arg1->hAllocation) + "\n";
	logstr += "\tmember UINT PrivateDriverData " + FormatDWORD(Arg1->PrivateDriverData) + "\n";
	logstr += "\tmember UINT NumPages " + FormatDWORD(Arg1->NumPages) + "\n";
	if (Arg1->NumPages != 0 && Arg1->pPages)
	{
		logstr += "\tstart list pPages\n";
		for (UINT i = 0; i < Arg1->NumPages; i++)
		{
			logstr += "\t\tmember UINT " + FormatDWORD(*(Arg1->pPages + i)) + "\n";
		}
		logstr += "\tend list pPages\n";
	}
	else
	{
		logstr += "member pPages " + FormatDWORD((DWORD)Arg1->pPages);
	}

	if (Arg1->NumPages != 0 && Arg1->pData)
	{
		logstr += "\tstart list pData\n";
		for (UINT i = 0; i < Arg1->NumPages; i++)
		{
			logstr += "\t\tmember UINT " + FormatDWORD((DWORD)*((PVOID*)Arg1->pData + i)) + "\n";
		}
		logstr += "\tend list pData\n";
	}
	else
	{
		logstr += "member pData " + FormatDWORD((DWORD)Arg1->pData);
	}

	logstr += "\tmember D3DDDI_SYNCHRONIZATIONOBJECT_FLAGS Flags " + FormatDWORD(Arg1->Flags.Value) + "\n";
	logstr += "\tmember D3DGPU_VIRTUAL_ADDRESS GpuVirtualAddress " + FormatULONG64(Arg1->GpuVirtualAddress) + "\n";

	logstr += "end struct D3DKMT_LOCK";
}

void SaveD3DKMT_UNLOCK(D3DKMT_UNLOCK *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_UNLOCK ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hDevice " + FormatDWORD(Arg1->hDevice) + "\n";
	logstr += "\tmember UINT NumAllocations " + FormatDWORD(Arg1->NumAllocations) + "\n";
	if (Arg1->NumAllocations != 0 && Arg1->phAllocations)
	{
		logstr += "\tstart list phAllocations\n";
		for (UINT i = 0; i < Arg1->NumAllocations; i++)
		{
			logstr += "\t\tmember D3DKMT_HANDLE " + FormatDWORD(*(Arg1->phAllocations + i)) + "\n";
		}
		logstr += "\tend list phAllocations\n";
	}
	else
	{
		logstr += "member phAllocations " + FormatDWORD((DWORD)Arg1->phAllocations);
	}

	logstr += "end struct D3DKMT_UNLOCK";
}


void SaveD3DKMT_SETCONTEXTINPROCESSSCHEDULINGPRIORITY(D3DKMT_SETCONTEXTINPROCESSSCHEDULINGPRIORITY *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_SETCONTEXTINPROCESSSCHEDULINGPRIORITY ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hContext " + FormatDWORD(Arg1->hContext) + "\n";
	logstr += "\tmember INT Priority " + FormatDWORD(Arg1->Priority) + "\n";
	logstr += "end struct D3DKMT_SETCONTEXTINPROCESSSCHEDULINGPRIORITY";
}

void SaveD3DKMT_SIGNALSYNCHRONIZATIONOBJECTFROMCPU(D3DKMT_SIGNALSYNCHRONIZATIONOBJECTFROMCPU *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_SIGNALSYNCHRONIZATIONOBJECTFROMCPU ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hDevice " + FormatDWORD(Arg1->hDevice) + "\n";
	logstr += "\tmember UINT ObjectCount " + FormatDWORD(Arg1->ObjectCount) + "\n";
	logstr += "\tstart list ObjectHandleArray\n";
	if (Arg1->ObjectCount > 0 && Arg1->ObjectHandleArray != NULL)
	{
		for (UINT i = 0; i < Arg1->ObjectCount; i++)
			logstr += "\t\tmember D3DKMT_HANDLE " + FormatDWORD(*(Arg1->ObjectHandleArray + i)) + "\n";
	}
	else
	{
		logstr += "\t\tmember D3DKMT_HANDLE 0\n";
	}
	logstr += "\tend list ObjectHandleArray\n";

	logstr += "\tstart list FenceValueArray\n";
	if (Arg1->ObjectCount > 0 && Arg1->FenceValueArray != NULL)
	{
		for (UINT i = 0; i < Arg1->ObjectCount; i++)
			logstr += "\t\tmember UINT64 " + FormatULONG64(*(Arg1->FenceValueArray + i)) + "\n";
	}
	else
	{
		logstr += "\t\tmember D3DKMT_HANDLE 0\n";
	}
	logstr += "\tend list FenceValueArray\n";


	logstr += "\tmember D3DKMT_SIGNALSYNCHRONIZATIONOBJECTFROMCPU Flags " + FormatDWORD((DWORD)Arg1->Flags.Value) + "\n";
	logstr += "end struct D3DKMT_SIGNALSYNCHRONIZATIONOBJECTFROMCPU";
	log.info(logstr);
}


void SaveD3DKMT_EVICT(D3DKMT_EVICT *Arg1, log4cpp::Category& log, LogType inout)
{
	std::string logstr = GetTID() + "\nstart struct D3DKMT_EVICT ";
	logstr += GetLogStr(inout) + "\n";
	logstr += "\tmember D3DKMT_HANDLE hDevice " + FormatDWORD(Arg1->hDevice) + "\n";
	logstr += "\tmember UINT ObjectCount " + FormatDWORD(Arg1->NumAllocations) + "\n";
	logstr += "\tstart list AllocationList\n";
	if (Arg1->NumAllocations > 0 && Arg1->AllocationList != NULL)
	{
		for (UINT i = 0; i < Arg1->NumAllocations; i++)
			logstr += "\t\tmember D3DKMT_HANDLE " + FormatDWORD(*(Arg1->AllocationList + i)) + "\n";
	}
	else
	{
		logstr += "\t\tmember D3DKMT_HANDLE 0\n";
	}
	logstr += "\tend list AllocationList\n";

	logstr += "\tmember D3DDDI_EVICT_FLAGS Flags " + FormatDWORD((DWORD)Arg1->Flags.Value) + "\n";
	logstr += "\tmember UINT64 NumBytesToTrim " + FormatULONG64(Arg1->NumBytesToTrim) + "\n";
	logstr += "end struct D3DKMT_EVICT";
	log.info(logstr);
}
//end
