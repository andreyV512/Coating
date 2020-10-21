#include "RunExecute.h"
#include <Windows.h>

bool RunExecute(wchar_t *path, bool hide)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si = {};
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = hide ? SW_HIDE: SW_SHOWNORMAL;
	si.cb = sizeof(STARTUPINFO);
	
	if(CreateProcess(NULL, path, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
	{
		CloseHandle(pi.hThread); 
		CloseHandle(pi.hProcess);
		return true;
	}
	return false;
}

bool BlockingExecute(wchar_t *path)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si = {};
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;// SW_SHOWNORMAL;
	si.cb = sizeof(STARTUPINFO);

	if (CreateProcess(NULL, path, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		return true;
	}
	return false;
}
