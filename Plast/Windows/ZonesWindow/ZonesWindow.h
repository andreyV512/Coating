#pragma once
#include "window_tool/message.h"
#include "ZonesWindowToolBar.h"
#include "window_tool/TEvent.h"
#include "templates/typelist.hpp"
#include "Windows/Viewers/MainViewers/SensorViewer.h"
#include "Windows/Viewers/ZonesViewer/ZoneViewer.h"
#include "Windows/Viewers/ZonesViewer/AScanZoneViewer.h"

class ZonesWindow
{
public:
	template<int N>struct RadioBtn: TEvent
	{
		static const int ID = N;
		ZonesWindow *owner;
		void Do(TCommand &l)override
		{
			owner->ChangeSensor(ID);
		}
	};
	typedef Vlst<RadioBtn<0>, RadioBtn<1>, RadioBtn<2>> radio_btn_list;
	VL::Factory<radio_btn_list> radio_btns;
	HWND hWnd;
	HWND hStatuisBar;
	ZonesWindowToolBar toolBar;

	SensorViewer sensor;
	ZoneViewer zone;
	AScanZoneViewer aScan;

	ZonesWindow();
	LRESULT operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	void operator()(TClose &);

	void ChangeSensor(int);
};