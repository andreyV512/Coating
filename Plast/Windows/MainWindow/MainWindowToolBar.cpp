#include "MainWindowToolBar.h"
#include "Resource.h"
#include "window_tool/InitToolBar.hpp"
#include "tools_debug/DebugMess.h"
#include "window_tool/OpenWindow.hpp"
#include "Windows/AScanWindow/AScanWindow.h"
#include "Windows/MainWindow/AppKeyHandler.h"
#include "Automat/Automat.h"

namespace
{
	template<int N>struct Key;

#define KEY(ID, text)template<>struct Key<ID>\
{\
	static void Click(HWND);\
	static wchar_t *Text(){return (wchar_t *)text;}\
};

	KEY(IDB_CycleBtn, L"F4 Цикл")
	KEY(IDB_Reset, L"Esc Стор")
//		KEY(IDB_QueryBtn, L"Тест")
#undef KEY

	template<int ID>using BTB = ButtonToolbar<ID, Key<ID> >;
	typedef Vlst<
		SeparatorToolbar<0>
		, BTB<IDB_CycleBtn>
		, BTB<IDB_Reset>
	//	, BTB<IDB_QueryBtn>
		, SeparatorToolbar<1>
	>tool_button_list;

	void Key<IDB_CycleBtn>::Click(HWND h) 
	{
		//dprint("Key<IDB_CycleBtn>::Click\n"); 
		//AppKeyHandler::Run();
		//App::IsRun() = true;
		Automat::Start();
	}
	void Key<IDB_Reset>::Click(HWND h) 
	{
		//dprint("Key<IDB_Reset>::Click\n"); 
		//AppKeyHandler::Stop();
		//App::IsRun() = false;
		Automat::Stop();
	}
	//void Key<IDB_QueryBtn>::Click(HWND h) 
	//{
	//	OpenWindow<AScanWindow>::Do(h);
	//	AppKeyHandler::DisableAll();
	//}
}

HWND MainWindowToolBar::Init(HWND h)
{
	hWnd = InitToolbar< tool_button_list>()(h);
	return hWnd;
}

void MainWindowToolBar::Size()
{
	MoveWindow(hWnd, 0, 0, 0, 0, false);
}

const int MainWindowToolBar::Width()
{
	return __tool_bar_width__<tool_button_list>::value;;
}
