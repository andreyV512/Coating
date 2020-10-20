#include "Zip.h"
#include <Windows.h>
#include <stdio.h>
#include "window_tool/RunExecute.h"
#include "window_tool/ProgrammDir.h"
#include "tools_debug/DebugMess.h"

void Zip::Zip(wchar_t *nameFile)
{
	ProgrammDir pd;
	wchar_t buf[2048];
	wsprintf(buf, L"%s\\..\\bin\\bzip2 -zf %s\\..\\stored\\%s", pd.path, pd.path, nameFile);
	RunExecute(buf);
}

void Zip::Unzip(wchar_t *nameFile)
{
	ProgrammDir pd;
	wchar_t buf[2048];
	wsprintf(buf, L"%s\\..\\bin\\bzip2 -df %s\\..\\stored\\%s", pd.path, pd.path, nameFile);
	RunExecute(buf);
}

void Zip::ZipAll()
{
	ProgrammDir pd;
	wchar_t buf[2048];
	wsprintf(buf, L"%s\\..\\bin\\bzip2 -zf %s\\..\\stored\\", pd.path, pd.path);
	wchar_t *tail = buf + wcslen(buf);

	wsprintf(pd.tail, L"\\..\\stored\\*.dat");

	WIN32_FIND_DATA ffd;
	HANDLE hFind = FindFirstFile(pd.path, &ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		dprint("FindFirstFile failed (%d)\n", GetLastError());
		return;
	}
	bool b;
	do			 
	{
		dprint("%S\n", ffd.cFileName);
		wsprintf(tail, L"%s", ffd.cFileName);
		b = BlockingExecute(buf);

	} while (b && 0 != FindNextFile(hFind, &ffd));

	FindClose(hFind);
}

static DWORD __stdcall Zip_ZipAsyncAll(LPVOID)
{
	Zip::ZipAll();
	return 0;
}

void Zip::ZipAsyncAll()
{
	QueueUserWorkItem(Zip_ZipAsyncAll, NULL, WT_EXECUTEDEFAULT);
}
