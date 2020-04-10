#pragma once
#include "Graphics/Chart.hpp"
#include "Graphics/Chart.h"
#include "Graphics/ColorLabel.h"
#include "Graphics/EmptyLeftAxes.h"
#include "GridSeries.h"
#include "Graphics/BarSeries.h"

class CommonSensorViewer
{
public:
	int currentX;
	TMouseMove storedMouseMove;
protected:
	bool mouseMove;
public:
	HWND hWnd;
	Gdiplus::Bitmap *backScreen;
	ColorLabel label;
	Cursor *cursor;
	Chart *chart;

	CommonSensorViewer();
	virtual ~CommonSensorViewer();

	LRESULT operator()(TCreate &);
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TCommand &);
	void operator()(TMouseMove &);
	void operator()(TMouseWell &);
	void operator()(TLButtonDown &);
	void operator()(TLButtonDbClk &);
};