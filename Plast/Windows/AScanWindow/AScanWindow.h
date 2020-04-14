#pragma once
#include <Windows.h>
#include "window_tool/message.h"
#include "AScanWindowToolBar.h"
#include "Graphics/TopLabelViewer.h"
#include "Windows/Viewers/AScanViewer/AScanViewer.h"

class AScanWindow
{
public: 
	template<int N>class Sens : public AScanViewer
	{
	public:
		typedef AScanViewer Parent;
		Sens(){numSensor = N;}
	};
	typedef Vlst<Sens<1>, Sens<2>, Sens<3> >viewers_list;
	VL::Factory< viewers_list> viewers;
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