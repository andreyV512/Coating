#pragma once
#include <Windows.h>
#include "window_tool\message.h"
#include "MainWindowToolBar.h"
#include "../Viewers/DefectViewers/DefectViewer.h"
#include "Graphics/TopLabelViewer.h"

class MainWindow
{
public:
	typedef Vlst<DefectViewer>viewers_list;
	VL::Factory< viewers_list> viewers;
	HWND hWnd;
	HWND hStatuisBar;
	MainWindowToolBar toolBar;
	TopLabelViewer topLabelViewer;

	LRESULT operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
};
