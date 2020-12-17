#include "CommonApp.h"
#include <windows.h>
#include "tools_debug/DebugMess.h"
#include "../LanProcess/LanDirect/EventNames.h"
namespace CommonApp
{
	const wchar_t *appName = L"aluminum bar 200409";
	bool IsAppRun()
	{
		CreateSemaphore(0, 0, 1, appName);
		return ERROR_ALREADY_EXISTS == GetLastError();
	}
	bool TestAppRun()
	{
		HANDLE h = OpenSemaphore(EVENT_ALL_ACCESS, TRUE, appName);
		int ret = GetLastError();
		bool b = ERROR_SUCCESS == ret || ERROR_ALREADY_EXISTS == ret;
		if (0 != h)CloseHandle(h);
		dprint("TestAppRun GetLastError %d\n", ret);
		return b;
	}
	bool TestProcessLan()
	{
		HANDLE h = OpenEvent(EVENT_ALL_ACCESS, TRUE, wLanParams);
		int ret = GetLastError();
		bool b = ERROR_SUCCESS == ret || ERROR_ALREADY_EXISTS == ret;
		if (0 != h)CloseHandle(h);
		dprint("TestProcessLan GetLastError %d\n", ret);
		return b;
	}
};

