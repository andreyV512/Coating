#include "CircleSeries.h"

using namespace Gdiplus;
//---------------------------------------------------------------------------------------------------------------
CircleSeries::CircleSeries(Chart &chart)
    : chart(chart) 
	, radius(5)
{
}
//----------------------------------------------------------------------------------------------------------
void CircleSeries::Draw()
{
	chart.g->SetClip(&Region(RectF(
		REAL(chart.rect.left + chart.offsetAxesLeft)
		, REAL(chart.rect.top + chart.offsetAxesTop)
		, REAL((chart.rect.right - chart.offsetAxesRight) - (chart.rect.left + chart.offsetAxesLeft))
		, REAL((chart.rect.bottom - chart.offsetAxesBottom) - (chart.rect.top + chart.offsetAxesTop))
		)),
       CombineModeReplace
     );	
	double dX = (double)(chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight) / (chart.maxAxesX - chart.minAxesX);
	double x = chart.rect.left + chart.offsetAxesLeft;
	double dY = (double)(chart.rect.bottom - chart.rect.top - chart.offsetAxesBottom - chart.offsetAxesTop) / (chart.maxAxesY - chart.minAxesY);
	double bottom = chart.rect.bottom - chart.offsetAxesBottom;
	REAL yy = 0;
	for(int i = 0; i < count; ++i)
	{
		if(*enabled[i])
		{
			yy = REAL(bottom - (yOffs[i] - chart.minAxesY) * dY);
			REAL xx = REAL(x + xOffs[i] * dX);
			chart.g->FillEllipse(&SolidBrush(color[i]), xx - radius, yy - radius, (REAL)2.0 * radius, (REAL)2.0 * radius);
		}
	}
	chart.g->SetClip(&Region());
}
//-------------------------------------------------------------------------------------------------------------------