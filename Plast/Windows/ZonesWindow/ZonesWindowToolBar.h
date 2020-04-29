#pragma once
#include <Windows.h>

class ZonesWindowToolBar
{
public:
	HWND hWnd;
	HWND Init(HWND);
	void Size();
	static const int Width();
};
