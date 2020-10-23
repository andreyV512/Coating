#include "CheckBox.h"

void CheckBox::Init(HWND hOwner, bool b, wchar_t *txt)
{
	hWnd = CreateWindow(L"button", txt
		, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_TABSTOP
		, 0, 0, 0, 0, hOwner, NULL, GetModuleHandle(NULL), NULL
	);
	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);
	Button_SetCheck(hWnd, b ? BST_CHECKED : BST_UNCHECKED);
}

bool CheckBox::operator()()
{
    return BST_CHECKED == Button_GetCheck(hWnd);
}

void CheckBox::Do(TCommand &l)
{
	bool b = BST_CHECKED == Button_GetCheck(l.hControl);
	(obj->*ptr)(b);
}

void CheckBox::Size(int left, int top, int width, int height)
{
	MoveWindow(hWnd, left, top, width, height, true);
}
