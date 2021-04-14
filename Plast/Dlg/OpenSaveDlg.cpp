#include "Dlg.h"
#include <shlwapi.h>
#include <memory>
#include "window_tool/DlgFiles.h"
#include "Data/Store.h"
#include "window_tool/Zip.h"
#include "Data/Store.h"
#include "Compute/Compute.h"
#include "Windows/MainWindow/MainWindow.h"
#include "Windows/MainWindow/AppKeyHandler.h"
#include "Log/LogBuffer.h"
#include "Log/LogMessages.h"
#include "tools_debug/DebugMess.h"

struct store_path
{
	wchar_t path[256];
	store_path(wchar_t *_path)
	{
		wcscpy_s(path, _path);
		zprint("\n");
	}
	~store_path()
	{
		zprint("\n");
	}
};

DWORD __stdcall OpenFileDlg_Do(LPVOID p)
{
	wchar_t *path = ((store_path *)p)->path;
	wchar_t *tail = path + wcslen(path) - 4;
	bool unzip = false;
	if (0 == wcscmp(tail, L".bz2"))
	{
		Log::Mess<LogMess::Unziping>();
		Zip::UnzipCurrentDir(path);
		*tail = L'\0';
		unzip = true;
	}
	if (Store::Load(path))
	{
		Singleton<Compute>::Instance().Recalculation();
		Zip::ZipCurentDir(path);
	}
	else
	{
		MainWindow::EnableMenu(true);
		AppKeyHandler::Stop();
	}
	delete (store_path *)p;
	return 0;
}

void OpenFileDlg::Do(HWND h)
{
	OpenData o(h, (wchar_t *)L"Открыть файл");
	if (o())
	{
		MainWindow::EnableMenu(false);
		AppKeyHandler::DisableAll();
		QueueUserWorkItem(OpenFileDlg_Do, new store_path(o.sFile), WT_EXECUTEDEFAULT);
	}
}

DWORD __stdcall SaveFileDlg_Do(LPVOID p)
{
	wchar_t *path = ((store_path *)p)->path;
	Store::Save(path);
	Zip::ZipCurentDir(path);
	delete (store_path *)p;
	return 0;
}

void SaveFileDlg::Do(HWND h)
{
	SaveData o(h, (wchar_t *)L"Сохранить файл");
	if (o())
	{
		QueueUserWorkItem(SaveFileDlg_Do, new store_path(o.sFile), WT_EXECUTEDEFAULT);
	}
}