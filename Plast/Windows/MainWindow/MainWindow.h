#pragma once
#include <Windows.h>
#include "window_tool/message.h"
#include "MainWindowToolBar.h"
#include "Windows/Viewers/MainViewers/SensorViewer.h"
#include "Windows/Viewers/MainViewers/ResultViewer.h"
#include "Graphics/TopLabelViewer.h"
#include "window_tool/SelectTypeSizeList.h"
#include "window_tool/CheckBox.h"

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
	typedef Vlst<Sens<0>, Sens<1>, Sens<2>, ResultViewer>viewers_list;
	VL::Factory< viewers_list> viewers;
	HWND hWnd;
	HWND hStatuisBar;
	MainWindowToolBar toolBar;
	TopLabelViewer topLabelViewer;
	SelectTypeSizeList select;

	CheckBox cbStopControl;

	LRESULT operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	void operator()(TClose &);
	void operator()(TCopyData &);

	static void EnableMenu(bool);

	void StoreStopControl(bool);
	bool LoadStopControl();
};




