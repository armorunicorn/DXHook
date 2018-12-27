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
	root.info(#OrgFunName);\
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

void SaveD3DKMT_CREATEALLOCATION(D3DKMT_CREATEALLOCATION *Arg1, UINT tpye, LogType inout, bool isD3D);
void SaveD3DKMT_CREATEDEVICE(D3DKMT_CREATEDEVICE  *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_CREATECONTEXT(D3DKMT_CREATECONTEXT  *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_RENDER(D3DKMT_RENDER  *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_CREATECONTEXTVIRTUAL(D3DKMT_CREATECONTEXTVIRTUAL   *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_ESCAPE(D3DKMT_ESCAPE    *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_SUBMITCOMMAND(D3DKMT_SUBMITCOMMAND     *Arg1, log4cpp::Category& log, LogType inout);
void SaveD3DKMT_ENUMADAPTERS(D3DKMT_ENUMADAPTERS     *Arg1, log4cpp::Category& log, LogType inout);

//NtGdiDdDDI
/*HOOK_FUN_DEF(D3DKMTCreateDevice, D3DKMT_CREATEDEVICE)
DDI_HOOK_FUN_DEF(NtGdiDdDDICreateDevice, D3DKMT_CREATEDEVICE)
//HOOK_FUN_DEF(D3DKMTCreateAllocation, D3DKMT_CREATEALLOCATION)
//DDI_HOOK_FUN_DEF(NtGdiDdDDICreateAllocation, D3DKMT_CREATEALLOCATION)
#define SET_DWORD(p, i, v) *((DWORD*)p + i) = v
BOOL WINAPI D3DKMTCreateAllocationFake(D3DKMT_CREATEALLOCATION *Arg1)
{
	NTSTATUS ret = 0;

	int type = 2;
	__try
	{
		SaveD3DKMT_CREATEALLOCATION(Arg1, 2, in, true);
		//Arg1->pAllocationInfo2->Priority = 0;
		//Arg1->pAllocationInfo2->Reserved[0] = 0;
		//Arg1->pAllocationInfo2->Reserved[1] = 0;
		//Arg1->pAllocationInfo2->Reserved[2] = 0;
		//Arg1->pAllocationInfo2->Reserved[3] = 0;
		//Arg1->pAllocationInfo2->Reserved[4] = 0;
		//Arg1->pAllocationInfo2->GpuVirtualAddress = 0;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		type = 1;
		SaveD3DKMT_CREATEALLOCATION(Arg1, 1, in, true);
	}
	ret = D3DKMTCreateAllocation(Arg1);
	SaveD3DKMT_CREATEALLOCATION(Arg1, type, out, true);
	return ret;
}

DDIFakeFunType NtGdiDdDDICreateAllocationOrg = (DDIFakeFunType)GetProcAddress(GetModuleHandle(TEXT("win32u")), "NtGdiDdDDICreateAllocation");
BOOL WINAPI NtGdiDdDDICreateAllocationFake(D3DKMT_CREATEALLOCATION *Arg1)
{
	//log4cpp::Category& root = log4cpp::Category::getRoot();
	int type = 2;
	__try
	{
		SaveD3DKMT_CREATEALLOCATION(Arg1, 2, in, false);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		SaveD3DKMT_CREATEALLOCATION(Arg1, 1, in, false);
	}
	BOOL ret = NtGdiDdDDICreateAllocationOrg((void*)Arg1);
	SaveD3DKMT_CREATEALLOCATION(Arg1, type, out, false);
	return ret;
}*/

//HOOK_FUN_DEF(D3DKMTCreateContext, D3DKMT_CREATECONTEXT)
//DDI_HOOK_FUN_DEF(NtGdiDdDDICreateContext, D3DKMT_CREATECONTEXT)

//HOOK_FUN_DEF(D3DKMTRender, D3DKMT_RENDER)
//DDI_HOOK_FUN_DEF(NtGdiDdDDIRender, D3DKMT_RENDER)

//HOOK_FUN_DEF(D3DKMTCreateContextVirtual, D3DKMT_CREATECONTEXTVIRTUAL)
//DDI_HOOK_FUN_DEF(NtGdiDdDDICreateContextVirtual, D3DKMT_CREATECONTEXTVIRTUAL)

//HOOK_FUN_DEF(D3DKMTEscape, D3DKMT_ESCAPE)
//DDI_HOOK_FUN_DEF(NtGdiDdDDIEscape, D3DKMT_ESCAPE)

//HOOK_FUN_DEF(D3DKMTSubmitCommand, D3DKMT_SUBMITCOMMAND)
//DDI_HOOK_FUN_DEF(NtGdiDdDDISubmitCommand, D3DKMT_SUBMITCOMMAND)

//HOOK_FUN_DEF(D3DKMTEnumAdapters, D3DKMT_ENUMADAPTERS)
//DDI_HOOK_FUN_DEF(NtGdiDdDDIEnumAdapters, D3DKMT_ENUMADAPTERS)


TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIAbandonSwapChain)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIAcquireKeyedMutex)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIAcquireKeyedMutex2)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIAcquireSwapChain)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIAddSurfaceToSwapChain)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIAdjustFullscreenGamma)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICacheHybridQueryValue)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIChangeVideoMemoryReservation)
TEST_NOARG_DDI_HOOK_FUN_DEF(NtGdiDdDDICheckExclusiveOwnership)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICheckMonitorPowerState)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICheckMultiPlaneOverlaySupport)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICheckMultiPlaneOverlaySupport2)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICheckMultiPlaneOverlaySupport3)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICheckOcclusion)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICheckSharedResourceAccess)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICheckVidPnExclusiveOwnership)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICloseAdapter)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIConfigureSharedResource)
//111111111111
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICreateAllocation)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICreateBundleObject)
//111111111111
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICreateContext)
//111111111111
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICreateContextVirtual)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICreateDCFromMemory)
//111111111111
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICreateDevice)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICreateHwContext)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICreateHwQueue)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICreateKeyedMutex)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICreateKeyedMutex2)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICreateOutputDupl)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICreateOverlay)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICreatePagingQueue)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICreateProtectedSession)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICreateSwapChain)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDICreateSynchronizationObject)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIDDisplayEnum)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIDestroyAllocation)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIDestroyAllocation2)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIDestroyContext)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIDestroyDevice)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIDestroyHwContext)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIDestroyHwQueue)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIDestroyKeyedMutex)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIDestroyOutputDupl)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIDestroyOverlay)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIDestroyPagingQueue)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIDestroyProtectedSession)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIDestroySynchronizationObject)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIDispMgrCreate)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIDispMgrSourceOperation)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIDispMgrTargetOperation)
//111111111111
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIEnumAdapters)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIEnumAdapters2)
//111111111111
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIEscape)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIEvict)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIExtractBundleObject)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIFlipOverlay)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIFlushHeapTransitions)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIFreeGpuVirtualAddress)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetAllocationPriority)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetCachedHybridQueryValue)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetContextInProcessSchedulingPriority)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetContextSchedulingPriority)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetDWMVerticalBlankEvent)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetDeviceState)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetDisplayModeList)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetMemoryBudgetTarget)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetMultiPlaneOverlayCaps)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetMultisampleMethodList)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetOverlayState)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIGetPostCompositionCaps)
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
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDILock)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDILock2)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIMakeResident)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIMapGpuVirtualAddress)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIMarkDeviceAsError)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDINetDispGetNextChunkInfo)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDINetDispQueryMiracastDisplayDeviceStatus)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDINetDispQueryMiracastDisplayDeviceSupport)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDINetDispStartMiracastDisplayDevice)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDINetDispStopMiracastDisplayDevice)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDINetDispStopSessions)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOfferAllocations)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenAdapterFromDeviceName)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenAdapterFromHdc)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenAdapterFromLuid)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenBundleObjectNtHandleFromName)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenKeyedMutex)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenKeyedMutex2)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenKeyedMutexFromNtHandle)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenNtHandleFromName)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenProtectedSessionFromNtHandle)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenResource)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenResourceFromNtHandle)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenSwapChain)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenSyncObjectFromNtHandle)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenSyncObjectFromNtHandle2)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenSyncObjectNtHandleFromName)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOpenSynchronizationObject)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOutputDuplGetFrameInfo)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOutputDuplGetMetaData)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOutputDuplGetPointerShapeData)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOutputDuplPresent)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIOutputDuplReleaseFrame)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIPinDirectFlipResources)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIPollDisplayChildren)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIPresent)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIPresentMultiPlaneOverlay)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIPresentMultiPlaneOverlay2)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIPresentMultiPlaneOverlay3)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIPresentRedirected)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIQueryAdapterInfo)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIQueryAllocationResidency)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIQueryClockCalibration)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIQueryFSEBlock)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIQueryProcessOfferInfo)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIQueryProtectedSessionInfoFromNtHandle)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIQueryProtectedSessionStatus)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIQueryRemoteVidPnSourceFromGdiDisplayName)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIQueryResourceInfo)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIQueryResourceInfoFromNtHandle)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIQueryStatistics)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIQueryVidPnExclusiveOwnership)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIQueryVideoMemoryInfo)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIReclaimAllocations)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIReclaimAllocations2)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIReleaseKeyedMutex)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIReleaseKeyedMutex2)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIReleaseProcessVidPnSourceOwners)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIReleaseSwapChain)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIRemoveSurfaceFromSwapChain)
//111111111111
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIRender)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIReserveGpuVirtualAddress)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISetAllocationPriority)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISetContextInProcessSchedulingPriority)
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
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISetQueuedLimit)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISetStablePowerState)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISetStereoEnabled)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISetSyncRefreshCountWaitTarget)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISetVidPnSourceHwProtection)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISetVidPnSourceOwner)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISetYieldPercentage)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIShareObjects)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISharedPrimaryLockNotification)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISharedPrimaryUnLockNotification)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISignalSynchronizationObject)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISignalSynchronizationObjectFromCpu)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISignalSynchronizationObjectFromGpu)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISignalSynchronizationObjectFromGpu2)
//111111111111
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISubmitCommand)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISubmitCommandToHwQueue)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISubmitSignalSyncObjectsToHwQueue)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDISubmitWaitForSyncObjectsToHwQueue)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDITrimProcessCommitment)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIUnOrderedPresentSwapChain)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIUnlock)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIUnlock2)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIUnpinDirectFlipResources)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIUpdateAllocationProperty)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIUpdateGpuVirtualAddress)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIUpdateOverlay)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIWaitForIdle)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIWaitForSynchronizationObject)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIWaitForSynchronizationObjectFromCpu)
TEST_DDI_HOOK_FUN_DEF(NtGdiDdDDIWaitForSynchronizationObjectFromGpu)
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
	//111111111111
	HOOK_FUN(NtGdiDdDDICreateAllocation, NtGdiDdDDICreateAllocationFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDICreateBundleObject, NtGdiDdDDICreateBundleObjectFake, win32u, root, result);
	//111111111111
	HOOK_FUN(NtGdiDdDDICreateContext, NtGdiDdDDICreateContextFake, win32u, root, result);
	//111111111111
	HOOK_FUN(NtGdiDdDDICreateContextVirtual, NtGdiDdDDICreateContextVirtualFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDICreateDCFromMemory, NtGdiDdDDICreateDCFromMemoryFake, win32u, root, result);
	//111111111111
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
	//111111111111
	HOOK_FUN(NtGdiDdDDIEnumAdapters, NtGdiDdDDIEnumAdaptersFake, win32u, root, result);
	HOOK_FUN(NtGdiDdDDIEnumAdapters2, NtGdiDdDDIEnumAdapters2Fake, win32u, root, result);
	//111111111111
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
	//111111111111
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
	//111111111111
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

void SaveD3DKMT_CREATEALLOCATION(D3DKMT_CREATEALLOCATION *Arg1, UINT type, LogType inout, bool isD3D)
{
	log4cpp::Category& root = log4cpp::Category::getRoot();
	char tmpBuf[512] = { 0 };
	std::string logstr = GetTID() + "\nstart struct D3DKMT_CREATEALLOCATION ";
	if (isD3D)
		logstr += "D3DKMT";
	else
		logstr += "NTDDI";
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