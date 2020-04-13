#pragma once
#include <Windows.h>

class AScanWindowToolBar
{
public:
	HWND hWnd;
	HWND Init(HWND);
	void Size();
	static const int Width();
};