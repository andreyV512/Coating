#pragma once
#include <Windows.h>
#include "window_tool/message.h"
#include "AScanWindowToolBar.h"
#include "Graphics/TopLabelViewer.h"

class AScanWindow
{
public: 
	HWND hWnd;
	HWND hStatuisBar;
	AScanWindowToolBar toolBar;
	TopLabelViewer topLabelViewer;

	LRESULT operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);

	static wchar_t *Title() { return (wchar_t *)L"AScan"; }
};