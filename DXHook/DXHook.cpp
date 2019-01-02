// DXHook.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <easyhook.h>
#include <d3dkmthk.h>
//#include "include/log4cpp/Category.hh"
//#include "include/log4cpp/PropertyConfigurator.hh"


//#pragma comment(lib, "log4cppD.lib")
//#pragma comment(lib, "Ws2_32.lib")
int main()
{
	//UINT a = sizeof(D3DDDI_ALLOCATIONINFO);
	//UINT b = sizeof(D3DDDI_ALLOCATIONINFO2);

	//log4cpp::Category::shutdown();
	ULONG pid = 0;
	NTSTATUS nt = RhCreateAndInject(
		(wchar_t*)L"F:\\Shovel_Knight\\ShovelKnight.exe",   // The process to inject into
		//(wchar_t*)L"C:\\Users\\Saber\\Desktop\\Shovel Knight\\ShovelKnight.exe",
		(wchar_t*)L"",           // ThreadId to wake up upon injection
		0,
		EASYHOOK_INJECT_DEFAULT,
		(wchar_t*)L"C:\\Users\\Saber\\Documents\\project\\DXHook\\Debug\\HookDll.dll", // 32-bit
		NULL,		 // 64-bit not provided
		NULL, // data to send to injected DLL entry point
		0,// size of data to send
		&pid
	);

	if (nt != 0)
	{
		printf("RhCreateAndInject failed with error code = %d\n", nt);
		PWCHAR err = RtlGetLastErrorString();
		std::wcout << err << "\n";
	}
	else
	{
		std::wcout << L"Library injected successfully.\n";
	}
	return 0;
}
