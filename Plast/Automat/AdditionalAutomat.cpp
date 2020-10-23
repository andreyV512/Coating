#include "AdditionalAutomat.h"
#include "window_tool/Emptywindow.h"
#include "Windows/MainWindow/MainWindow.h"
#include "Windows/MainWindow/AppKeyHandler.h"
#include "Automat.hpp"
#include "Data/Store.h"
#include "Data/StoreBase.h"
#include "window_tool/Emptywindow.h"

void Automat::Done()
{
	//HWND h = FindWindow(WindowClass<MainWindow>()(), 0);
	//MainWindow *w = (MainWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
	auto w = GetWindowPtr<MainWindow>();

	bool saveResult = true;

	if (w && w->cbStopControl())
	{
		AppKeyHandler::Continue();
		Bits<Key<StartBtn>, Key<StopBtn>>();
		AppKeyHandler::Run();
		saveResult = IDYES == MessageBox(w->hWnd
			, L"Сохранить результат контроля?"
			, L"Cообщение"
			, MB_ICONQUESTION | MB_YESNO
		);
	}
	if (saveResult)
	{
		COleDateTime tme = COleDateTime::GetCurrentTime();
		Store::Archive(tme);
		Store::Result(tme);
	}
}
