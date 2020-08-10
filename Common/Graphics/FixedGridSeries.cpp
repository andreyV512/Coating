#include <math.h>
#include "Graphics/FixedGridSeries.h"
#include "Graphics/Chart.h"
#include "tools_debug\DebugMess.h"
#include <stdio.h>

using namespace Gdiplus;
//----------------------------------------------------------
FixedGridSeries::FixedGridSeries(Chart &chart) 
	: chart(chart)
{
	SetColorCellHandler(this, &FixedGridSeries::GetColorCellDefault);
}
//----------------------------------------------------------
void FixedGridSeries::Draw()
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
	double length = chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight;
	double left = chart.rect.left + chart.offsetAxesLeft;
	double bottom = chart.rect.bottom - chart.offsetAxesBottom;
	double x;
	double y = bottom - dY;
	unsigned color;
	double data;	
	for(int i = 0, len = (int)(chart.maxAxesY - chart.minAxesY); i <= len; ++i)
	{
		x = left;
		for(unsigned j  = 0; (obj->*SetColorBar)(i, j, data, color); ++j)
		{
			chart.g->FillRectangle(&SolidBrush(Color(color)), (REAL)x, (REAL)y, (REAL)dX, (REAL)dY);
			x += dX;
		}
		y -= dY;
	}
	chart.g->SetClip(&Region());
}
//----------------------------------------------------------------
bool FixedGridSeries::GetColorCellDefault(unsigned sensor, int zone, double &data, unsigned &color)
{
	data = 10.0 / 10 * sensor;
	color = 0xffff0000;
	if(data > 4.0) {color = 0xff00ff00; return zone < 33;}
	if(data > 2.0) color = 0xffffff00;
	return zone < 33;
}
//----------------------------------------------------------------------------------------------------------------
FixedGrid::FixedGrid(Chart &chart) : chart(chart), dX(chart.deltaTickX), dY(chart.deltaTickY) {}
//------------------------------------------------------------------------------------------------------------------
void FixedGrid::Draw()
{
	Color color(chart.colorGrid);
	Pen pen(color, 1);
	pen.SetDashStyle(DashStyleDash);
	double offs = chart.offsetGridX;
	int y_b = chart.rect.bottom - chart.offsetAxesBottom - 3;
	int y_t = chart.rect.top + chart.offsetAxesTop + 3;
	while(offs < chart.rect.right - chart.offsetAxesRight - dX * 0.2)
	{
		chart.g->DrawLine(&pen, (int)offs, y_b, (int)offs, y_t);
		offs += dX;
	}
	offs = chart.offsetGridY;
	y_b = chart.offsetAxesLeft + 3;
	y_t = chart.rect.right - chart.offsetAxesRight - 3;
	int bottom = chart.rect.bottom - chart.offsetAxesBottom;
	while(bottom <= offs) offs -= dY;
	while(offs > chart.rect.top + chart.offsetAxesTop + dY * 0.2)
	{
		chart.g->DrawLine(&pen, y_b, (int)offs, y_t, (int)offs);
		offs -= dY;
	}
}

 VerticalFixedGrid::VerticalFixedGrid(Chart &chart): chart(chart), dX(chart.deltaTickX), dY(chart.deltaTickY) {}

void VerticalFixedGrid::Draw()
{
	Color color(chart.colorGrid);
	Pen pen(color, 1);
	pen.SetDashStyle(DashStyleDash);
	double offs = chart.offsetAxesLeft + dX;
	int y_b = chart.rect.bottom - chart.offsetAxesBottom - 3;
	int y_t = chart.rect.top + chart.offsetAxesTop + 3;
	while(offs < chart.rect.right - chart.offsetAxesRight - dY * 0.2)
	{
		chart.g->DrawLine(&pen, (int)offs, y_b, (int)offs, y_t);
		offs += dX;
	}
}
//------------------------------------------------------------------------------
FixedLeftAxes::FixedLeftAxes(Chart &chart) 
	: chart(chart)
	, leftOffset(25)
{}
//-----------------------------------------------------------------------------------
#pragma warning(disable : 4996)
void FixedLeftAxes::Draw()
{
	Font font(L"Arial", (REAL)chart.fontHeight, FontStyleBold);
	Color color(chart.colorAxes);
	Pen pen(color, 2);
	SolidBrush fontColor(chart.colorFontAxes);
	StringFormat format;
	format.SetAlignment(StringAlignmentFar);
	double height;

	chart.offsetAxesLeft = leftOffset;

	chart.GetCountDigit(chart.minAxesY, chart.maxAxesY, height, font);
	int x = leftOffset;
	int bottom = chart.rect.bottom - chart.offsetAxesBottom;
	chart.g->DrawLine(&pen, x, chart.rect.top + chart.offsetAxesTop, x, bottom);
	wchar_t wbuf[32];
	PointF origin;
	RectF rect;	
	double deltaTick = 0;
	double deltaDigit = 0;
	double digit = 0;
	double minTick = 0;
	deltaDigit = 1;
	digit = chart.minAxesY;
	minTick = 0;
	double dy = (chart.maxAxesY > chart.minAxesY)? chart.maxAxesY - chart.minAxesY: 1;
	deltaTick = (chart.rect.bottom - chart.rect.top - chart.offsetAxesBottom - chart.offsetAxesTop) / dy;
	double offs = chart.offsetGridY = chart.rect.bottom - chart.offsetAxesBottom + minTick;
	chart.deltaTickY = deltaTick;
	chart.deltaDigitY = deltaDigit;
	chart.offsetGridX = leftOffset + deltaTick;
	chart.offsetAxesLeft = leftOffset;
	while(bottom < offs)
	{
		offs -= deltaTick;
		digit += deltaDigit;
	}
	origin.X = (REAL)chart.rect.left;
	int len;
	while(offs > chart.rect.top + chart.offsetAxesTop + height)
	{
		chart.g->DrawLine(&pen, (REAL)x - 5, (REAL)offs, (REAL)x, (REAL)offs);
		_itow((int)digit, wbuf, 10);
		len = (int)wcslen(wbuf) + 1;
		{
			chart.g->MeasureString(wbuf, len, &font, origin, &format, &rect);
			origin.Y = REAL(offs - deltaTick / 2 - chart.fontHeight);// + deltaTick);///2);
			origin.X = leftOffset - rect.Width - 5;
			chart.g->DrawString(wbuf, len, &font, origin, &fontColor);
		}
		offs -= deltaTick;
		digit += deltaDigit;
	}	
}
//----------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
NoOffsetLeftAxes::NoOffsetLeftAxes(Chart &chart) 
	: chart(chart)
	, leftOffset(25)
{}
//-----------------------------------------------------------------------------------
#pragma warning(disable : 4996)
void NoOffsetLeftAxes::Draw()
{
	Font font(L"Arial", (REAL)chart.fontHeight, FontStyleBold);
	Color color(chart.colorAxes);
	Pen pen(color, 2);
	SolidBrush fontColor(chart.colorFontAxes);
	StringFormat format;
	format.SetAlignment(StringAlignmentNear);
	double height;

	chart.offsetAxesLeft = leftOffset;

	chart.GetCountDigit(chart.minAxesY, chart.maxAxesY, height, font);
	int x = leftOffset;
	int bottom = chart.rect.bottom - chart.offsetAxesBottom;
	chart.g->DrawLine(&pen, x, chart.rect.top + chart.offsetAxesTop, x, bottom);
	char buf[32];
	wchar_t wbuf[32];
	PointF origin;
	RectF rect;	
	double deltaTick = 0;
	double deltaDigit = 0;
	double digit = 0;
	double minTick = 0;
	OffsetAxes(
		int(height * 1.5)
		, chart.rect.bottom - chart.rect.top - chart.offsetAxesBottom - chart.offsetAxesTop
		, chart.minAxesY
		, chart.maxAxesY
		, deltaDigit
		, deltaTick
		, digit
		, minTick
		);
	double offs = chart.offsetGridY = chart.rect.bottom - chart.offsetAxesBottom + minTick;
	chart.deltaTickY = deltaTick;
	chart.deltaDigitY = deltaDigit;
	chart.offsetGridX = leftOffset + deltaTick;
	chart.offsetAxesLeft = leftOffset;
	while(bottom < offs)
	{
		offs -= deltaTick;
		digit += deltaDigit;
	}
	origin.X = (REAL)chart.rect.left;
	int len;
	while(offs > chart.rect.top + chart.offsetAxesTop + 0.2 * deltaTick)
	{
		chart.g->DrawLine(&pen, (REAL)x - 5, (REAL)offs, (REAL)x, (REAL)offs);
		gcvt(digit, 5, buf);
		mbstowcs(wbuf, buf, 32);
		len = (int)wcslen(wbuf) + 1;
		{
			chart.g->MeasureString(wbuf, len, &font, origin, &format, &rect);
			origin.Y = REAL(offs - deltaTick + deltaTick/2);
			origin.X = leftOffset - rect.Width - 5;
			chart.g->DrawString(wbuf, len, &font, origin, &fontColor);
		}
		offs -= deltaTick;
		digit += deltaDigit;
	}	
}
//----------------------------------------------------------------------------------------------

void OffsetToPixel(Chart &chart, WORD &offsX, WORD &offsY, int delta, bool horisontalMove)
{
	if(0 == delta) return;
	if(horisontalMove)
	{
		double dX = (double)(chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight) / (chart.maxAxesX - chart.minAxesX);
		int offsMin = chart.rect.left + chart.offsetAxesLeft;
		double t = offsX - dX * delta;
		t = delta > 0 ? ceil(t) : floor(t);
		offsX = (WORD)t;
		if(offsMin >= offsX){offsX = offsMin + 3; return;}
		int offsMax = chart.rect.right - chart.offsetAxesRight;
		if(offsMax <= offsX)offsX = offsMax - 3;
	}
	else
	{
		double dY = (double)(chart.rect.bottom - chart.rect.top - chart.offsetAxesTop - chart.offsetAxesBottom) / (chart.maxAxesY - chart.minAxesY);
		int offsMin = chart.rect.top + chart.offsetAxesTop;
		double t = offsY + dY * delta;
		t = delta < 0 ? ceil(t) : floor(t);
		offsY = (WORD)t;
		if(offsMin >= offsY){offsY = offsMin + 3; return;}
		int offsMax = chart.rect.bottom - chart.offsetAxesBottom;
		if(offsMax <= offsY)offsY = WORD(offsMax - dY / 2);
	}
}

Scale::Scale(Chart &chart)
	: chart(chart)
	, rightOffset(300)
	, count(0)
	, data(NULL)
{
}

void Scale::Draw()
{
	chart.g->SetClip(&Region(RectF(
		REAL(chart.rect.left + chart.offsetAxesLeft)// + 3)
		, REAL(chart.rect.top + chart.offsetAxesTop)// + 3)
		, REAL((chart.rect.right - chart.offsetAxesRight) - (chart.rect.left + chart.offsetAxesLeft))// - 6)
		, REAL((chart.rect.bottom - chart.offsetAxesBottom) - (chart.rect.top + chart.offsetAxesTop) - 1)
		)),
	   CombineModeReplace
	 );	
	Font font(L"Arial", 12, FontStyleBold);
	Color color(chart.colorAxes);
	Pen pen(color, 2);
	SolidBrush fontColor(chart.colorFontAxes);
	StringFormat format;
	format.SetAlignment(StringAlignmentNear);
	//double height;

	int x = chart.rect.right - chart.offsetAxesRight - rightOffset;
	
	PointF origin;
	RectF rect;	
	double offsY = 20;
	char buf[128];
	wchar_t wbuf[128];

	double bottom = ((double)chart.rect.bottom - chart.offsetAxesBottom) - (chart.rect.top + chart.offsetAxesTop) - 20;
		

	for(int i = 0; i < count; ++i)
	{
		origin.Y = REAL(offsY);
		origin.X = (REAL)x - 30;

		_itow(ind[i], wbuf, 10);
		int len = (int)wcslen(wbuf) + 1;
		chart.g->DrawString(wbuf, len, &font, origin, &fontColor);

		double t = floor(data[i] * 1000 + 0.5) / 1000;
		if(t > 100) t = 0;
		else if(t < 0) t = 0;
		sprintf(buf, "%.3f", t);
		mbstowcs(wbuf, buf, 32);
		len = (int)wcslen(wbuf) + 1;
		chart.g->MeasureString(wbuf, len, &font, origin, &format, &rect);
		

		origin.X = (REAL)x;
		chart.g->DrawString(wbuf, len, &font, origin, &fontColor);
		//offsY += 1.2 * height;
		//if(offsY > bottom)
		//{
		//	offsY = 20;
		//	x += 120;
		//}
	}
	chart.g->SetClip(&Region());
}
