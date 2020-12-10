#include "Gain.h"
using namespace Gdiplus;

Gain::Gain(Chart &chart)
	: axes(chart)
	, line(chart)
	, chart(chart)
	, count(line.count)
	, data(line.data)
	, enable(false)
	, minAxeY(0)
	, maxAxeY(2)
{
	line.color = 0xffffff00;
	line.dashStyle = DashStyleDot;
	line.widthPen = 1;
}

void Gain::Draw() 
{
	if (enable)
	{
		auto _0 = chart.deltaTickY;
		auto _1 = chart.deltaDigitY;
		auto _2 = chart.dY;
		auto _3 = chart.offsetGridX;
		auto chart_offsetAxesLeft = chart.offsetAxesLeft;
		auto _5 = chart.minAxesY;
		auto _6 = chart.maxAxesY;

		axes.leftOffset = chart.rect.right - chart.offsetAxesRight + 15;
		chart.minAxesY = minAxeY;
		chart.maxAxesY = maxAxeY;

		axes.Draw();

		chart.offsetAxesLeft = chart_offsetAxesLeft;
		line.Draw();

		chart.deltaTickY	 = _0;
		chart.deltaDigitY	 = _1;
		chart.dY			 = _2;
		chart.offsetGridX	 = _3;
		chart.minAxesY       = _5;
		chart.maxAxesY       = _6;
	}
}