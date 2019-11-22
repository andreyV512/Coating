#include "GridSeries.h"
#include "Graphics/Chart.h"
#include "App/App.h"

using namespace Gdiplus;
//----------------------------------------------------------
FixedGridSerieX::FixedGridSerieX(Chart &chart) : FixedGridSeries(chart){}
//----------------------------------------------------------
void FixedGridSerieX::Draw()
{
	chart.g->SetClip(&Region(RectF(
		REAL(chart.rect.left + chart.offsetAxesLeft + 0)
		, REAL(chart.rect.top + chart.offsetAxesTop + 0)
		, REAL((chart.rect.right - chart.offsetAxesRight) - (chart.rect.left + chart.offsetAxesLeft) - 0)
		, REAL((chart.rect.bottom - chart.offsetAxesBottom) - (chart.rect.top + chart.offsetAxesTop) - 1)
		)),
		CombineModeReplace
		);	
	double dX = chart.deltaTickX / chart.deltaDigitX;
	double dY = chart.deltaTickY / chart.deltaDigitY;
	double length = (double)chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight;
	double left = (double)chart.rect.left + chart.offsetAxesLeft;
	double bottom = (double)chart.rect.bottom - chart.offsetAxesBottom;
	double x;
	double y = bottom - dY;
	unsigned color;
	double data;
	bool b = false;
	for(int i = 0; i < App::count_sensors; ++i)
	{
		x = left;
		for(unsigned j  = 0; (obj->*SetColorBar)(i, j, data, color, b); ++j)
		{
			chart.g->FillRectangle(&SolidBrush(Color(color)), (REAL)x, (REAL)y, (REAL)dX, (REAL)dY);
			x += dX;
		}
		if(b)y -= dY;
	}
	chart.g->SetClip(&Region());
}