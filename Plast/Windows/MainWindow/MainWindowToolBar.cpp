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
#undef KEY

	template<int ID>using BTB = ButtonToolbar<ID, Key<ID> >;
	typedef Vlst<
		SeparatorToolbar<0>
		, BTB<IDB_CycleBtn>
		, BTB<IDB_Reset>
		, SeparatorToolbar<1>
	>tool_button_list;

	void Key<IDB_CycleBtn>::Click(HWND h) 
	{
		Automat::Start();
	}
	void Key<IDB_Reset>::Click(HWND h) 
	{
		Automat::Stop();
	}
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
