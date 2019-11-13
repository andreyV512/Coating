#pragma once
#include <Windows.h>
#include "window_tool\message.h"
#include "MainWindowToolBar.h"

class MainWindow
{
public:
	HWND hWnd;
	HWND hStatuisBar;
	MainWindowToolBar toolBar;

	LRESULT operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
};
