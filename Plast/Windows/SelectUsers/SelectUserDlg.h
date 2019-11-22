#pragma once
#include <Windows.h>
struct UserTable;
struct SelectUserDlg
{
	static bool Do(HWND h, UserTable &);
};