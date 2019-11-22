#pragma once
#include "Graphics/Chart.hpp"
#include "Graphics/Chart.h"
#include "Graphics/ColorLabel.h"
#include "Graphics/EmptyLeftAxes.h"
#include "GridSeries.h"
class DefectViewer
{
public:
	HWND hWnd;
	Gdiplus::Bitmap *backScreen;
	ColorLabel label;

	typedef ChartDraw< Chart, Vlst<
		BottomAxesMeters
		, EmptyLeftAxes
		, FixedGridSerieX
		, FixedGrid
	> > TChart;
	TChart tchart;

	DefectViewer();
	~DefectViewer();

	LRESULT operator()(TCreate &);
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TCommand &);
	void operator()(TDropFiles &);

	bool GetColorCell(unsigned, int, double &, unsigned &, bool &);
};