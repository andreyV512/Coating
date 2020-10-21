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

void Zip::ZipCurentDir(wchar_t *nameFile)
{
	ProgrammDir pd;
	wchar_t buf[2048];
	wsprintf(buf, L"%s\\..\\bin\\bzip2 -zf %s", pd.path, nameFile);
	BlockingExecute(buf);
}

void Zip::Unzip(wchar_t *nameFile)
{
	ProgrammDir pd;
	wchar_t buf[2048];
	wsprintf(buf, L"%s\\..\\bin\\bzip2 -df %s\\..\\stored\\%s", pd.path, pd.path, nameFile);
	BlockingExecute(buf);
}

void Zip::UnzipCurrentDir(wchar_t *nameFile)
{
	ProgrammDir pd;
	wchar_t buf[2048];
	wsprintf(buf, L"%s\\..\\bin\\bzip2 -df %s", pd.path, nameFile);
	BlockingExecute(buf);
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
		dprint("Zip::ZipAll() FindFirstFile failed (%d)\n", GetLastError());
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

DWORD __stdcall Zip_ZipAsyncAll(LPVOID)
{
	Zip::ZipAll();
	return 0;
}

void Zip::ZipAsyncAll()
{
	QueueUserWorkItem(Zip_ZipAsyncAll, NULL, WT_EXECUTEDEFAULT);
}

bool TestNameFile(wchar_t *txt)
{
	static const int count_char_in_name_file = 12;
	int countChars = 0;
	wchar_t c;
	while (c = *txt)
	{
		if (c >= '0' && c <= '9') ++countChars;
		else  break;
		++txt;
	}
	return '.' == c && count_char_in_name_file == countChars;
}

int Zip::CountZipFilesInDirectory()
{
	
	int counter = 0;
	ProgrammDir pd;
	wchar_t buf[2048];
	wsprintf(buf, L"%s\\..\\bin\\bzip2 -zf %s\\..\\stored\\", pd.path, pd.path);
	wchar_t *tail = buf + wcslen(buf);

	wsprintf(pd.tail, L"\\..\\stored\\*.bz2");

	WIN32_FIND_DATA ffd;
	HANDLE hFind = FindFirstFile(pd.path, &ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		dprint("Zip::CountZipFilesInDirectory() FindFirstFile failed (%d)\n", GetLastError());
		return 0;
	}
	do
	{
		if (TestNameFile(ffd.cFileName))
		{
			++counter;
		}
		else
		{
			wsprintf(pd.tail, L"\\..\\stored\\%s", ffd.cFileName);
			DeleteFile(pd.path);
			dprint("Deleted file %S\n", ffd.cFileName);
		}

	} while (0 != FindNextFile(hFind, &ffd));

	FindClose(hFind);
	return counter;
}

void Zip::RemoveZipFilesInDirectory(int maxCountFiles)
{
	int countRemoveFiles =  Zip::CountZipFilesInDirectory() - maxCountFiles;
	if (countRemoveFiles > 0)
	{
		ProgrammDir pd;
		wsprintf(pd.tail, L"\\..\\stored\\*.bz2");
		WIN32_FIND_DATA ffd;
		HANDLE hFind = FindFirstFile(pd.path, &ffd);
		if (INVALID_HANDLE_VALUE == hFind)
		{
			dprint("Zip::RemoveZipFilesInDirectory() FindFirstFile failed (%d)\n", GetLastError());
			return;
		}
		do
		{
			wsprintf(pd.tail, L"\\..\\stored\\%s", ffd.cFileName);
			DeleteFile(pd.path);
			dprint("Deleted file %S\n", ffd.cFileName);

		} while (--countRemoveFiles > 0 && 0 != FindNextFile(hFind, &ffd));

		FindClose(hFind);
	}
}
