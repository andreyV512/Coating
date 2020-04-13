#pragma once
#include <Windows.h>
#include "window_tool/message.h"
#include "MainWindowToolBar.h"
#include "../Viewers/DefectViewers/SensorViewer.h"
#include "../Viewers/DefectViewers/ResultViewer.h"
#include "Graphics/TopLabelViewer.h"

class MainWindow
{
public:
	template<int N>class Sens: public SensorViewer
	{
	public:
		typedef SensorViewer Parent;
		Sens() 
			: SensorViewer(Singleton<Data::Sensor<N>>::Instance().data)
		{ numSensor = N; }
	};
	typedef Vlst<Sens<1>, Sens<2>, Sens<3>, ResultViewer>viewers_list;
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




