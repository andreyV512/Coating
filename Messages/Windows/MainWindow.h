#pragma once
#include "window_tool/message.h"
#include "Windows/GridMessages.h"

class MainWindow
{
	static const int IDT_TIMER1 = 999;
	unsigned lastIndex;
public:
	HWND hWnd;
	GridMessages grid;
	LRESULT operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TSize &);
	LRESULT operator()(TNotify &);
	void operator()(TCommand &);

	void operator()(TTimer &);
};
