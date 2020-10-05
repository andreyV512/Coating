#pragma once
#include <Windows.h>
#include "window_tool/message.h"
#include "AScanWindowToolBar.h"
#include "Graphics/TopLabelViewer.h"
#include "Windows/Viewers/AScanViewer/AScanViewer.h"
#include "App/AppBase.h"
#include "Compute/ComputeFrame.h"
#include "Devices/LanDevice.h"

class AScanWindow
{
	int idTimer;
public: 
	template<int N>class Sens : public AScanViewer
	{
	public:
		typedef AScanViewer Parent;
		LineSeries &line;
		double data[8154];
		Sens():line(tchart.items.get<LineSeries>())
		{
			numSensor = N;
			line.data = data;
		}
	};
	TresholdsTable::TItems treshItems;
	typedef Vlst<Sens<0>, Sens<1>, Sens<2> >viewers_list;
	VL::Factory< viewers_list> viewers;
	HWND hWnd;
	HWND hStatuisBar;
	AScanWindowToolBar toolBar;
	TopLabelViewer topLabelViewer;
	ComputeFrame computeFrame;
	CollectionData data;
	LRESULT operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	void operator()(TTimer &);

	void operator()(TClose &);

	void SetThresh();

	static wchar_t *Title() { return (wchar_t *)L"AScan"; }

	void SwitchBipolar(bool);
	void Start();
	void Stop();
};