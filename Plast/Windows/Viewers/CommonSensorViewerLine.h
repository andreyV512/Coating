#pragma once
#include "Graphics/Chart.hpp"
#include "Graphics/Chart.h"
#include "Graphics/ColorLabel.h"
#include "Graphics/EmptyLeftAxes.h"
#include "Graphics/BarSeries.h"

class CommonSensorViewerLine
{
public:
	int currentX;
	TMouseMove storedMouseMove;
protected:
	bool mouseMove;
	struct XMouseMove {};
	XMouseMove *objMouseMove;
	void(XMouseMove::*ptrMouseMove)(int);
public:
	HWND hWnd;
	Gdiplus::Bitmap *backScreen;
	ColorLabel label;
	Cursor *cursor;
	Chart *chart;

	CommonSensorViewerLine();
	virtual ~CommonSensorViewerLine();

	LRESULT operator()(TCreate &);
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TCommand &);
	void operator()(TMouseMove &);
	void operator()(TMouseWell &);
	void operator()(TLButtonDown &);
	void operator()(TLButtonDbClk &);

	template<class T>void SetMouseMove(T *t, void(T:: *ptr)(int))
	{
		objMouseMove = (XMouseMove *)t;
		ptrMouseMove = (void(XMouseMove::*)(int))ptr;
	}
};