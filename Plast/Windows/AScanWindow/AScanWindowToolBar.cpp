#include "AScanWindowToolBar.h"
#include "Resource.h"
#include "window_tool/InitToolBar.hpp"
#include "AScanWindow.h"
#include "tools_debug/DebugMess.h"


namespace
{
	template<int N>struct Key;

#define KEY(ID, text)template<>struct Key<ID>\
{\
	static void Click(HWND);\
	static wchar_t *Text(){return (wchar_t *)text;}\
};

	KEY(IDB_CycleBtn, L"Цикл")
		KEY(IDB_Reset, L"Стор")
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
		AScanWindow *w = (AScanWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
		w->aScanAuto.Start();

	}
	void Key<IDB_Reset>::Click(HWND h) 
	{
		AScanWindow *w = (AScanWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
		w->aScanAuto.Stop();
	}
}

HWND AScanWindowToolBar::Init(HWND h)
{
	hWnd = InitToolbar< tool_button_list>()(h);
	return hWnd;
}

void AScanWindowToolBar::Size()
{
	MoveWindow(hWnd, 0, 0, 0, 0, false);
}

const int AScanWindowToolBar::Width()
{
	return __tool_bar_width__<tool_button_list>::value;;
}
