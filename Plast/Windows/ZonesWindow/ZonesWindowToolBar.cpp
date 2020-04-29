#include "ZonesWindowToolBar.h"
#include "Resource.h"
#include "window_tool/InitToolBar.hpp"
#include "tools_debug/DebugMess.h"
#include "window_tool/OpenWindow.hpp"

namespace
{
	template<int N>struct Key;

#define KEY(ID, text)template<>struct Key<ID>\
{\
	static void Click(HWND);\
	static wchar_t *Text(){return (wchar_t *)text;}\
};

	KEY(IDB_LeftArrow, NULL)
	KEY(IDB_RightArrow, NULL)
#undef KEY

	template<int ID>using BTB = ButtonToolbar<ID, Key<ID> >;
	typedef Vlst<
		SeparatorToolbar<0>
		, BTB<IDB_LeftArrow>
		, BTB<IDB_RightArrow>
		, SeparatorToolbar<1>
	>tool_button_list;

	void Key<IDB_LeftArrow>::Click(HWND h)
	{
		dprint("Key<IDB_CycleBtn>::Click\n"); 
	}
	void Key<IDB_RightArrow>::Click(HWND h)
	{
		dprint("Key<IDB_Reset>::Click\n"); 
	}
}

HWND ZonesWindowToolBar::Init(HWND h)
{
	hWnd = InitToolbar< tool_button_list, 16>()(h);
	return hWnd;
}

void ZonesWindowToolBar::Size()
{
	MoveWindow(hWnd, 0, 0, 0, 0, false);
}

const int ZonesWindowToolBar::Width()
{
	return __tool_bar_width__<tool_button_list>::value;;
}
