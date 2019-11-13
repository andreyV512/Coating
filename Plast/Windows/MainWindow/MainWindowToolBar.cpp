#include "MainWindowToolBar.h"
#include "Resource.h"
#include "window_tool/InitToolBar.hpp"

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
	KEY(IDB_QueryBtn, L"F4 Тест")
#undef KEY
#define BUTTON_KEY(ID)ButtonToolbar<ID, Key<ID> >				  
		typedef Vlst<
		SeparatorToolbar<0>
		, BUTTON_KEY(IDB_CycleBtn)
		, BUTTON_KEY(IDB_Reset)
		, BUTTON_KEY(IDB_QueryBtn)
		, SeparatorToolbar<1>
		>tool_button_list;
#undef BUTTON_KEY

	void Key<IDB_CycleBtn>::Click(HWND h) {}
	void Key<IDB_Reset>::Click(HWND h) {}
	void Key<IDB_QueryBtn>::Click(HWND h) {}
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
