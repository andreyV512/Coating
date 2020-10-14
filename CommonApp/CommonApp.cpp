#include "CommonApp.h"
#include <windows.h>
namespace CommonApp
{
	bool IsAppRun()
	{
		const wchar_t *typeWindow = L"aluminum bar 200409";
		CreateSemaphore(0, 0, 1, typeWindow);
		return GetLastError() == ERROR_ALREADY_EXISTS;
	}
};

