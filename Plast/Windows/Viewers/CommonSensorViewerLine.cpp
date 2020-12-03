#include "CommonSensorViewerLine.h"
#include "window_tool/MenuAPI.h"
#include "templates/templates.hpp"

CommonSensorViewerLine::CommonSensorViewerLine()
	: backScreen(NULL)
	, hWnd()
	, currentX(0)
	, mouseMove(true)
	, objMouseMove(NULL)
	, ptrMouseMove(NULL)
{}

CommonSensorViewerLine::~CommonSensorViewerLine()
{
	delete backScreen;
	zprint("\n");
}

LRESULT CommonSensorViewerLine::operator()(TCreate &l)
{
	chart->rect.top = 17;
	label.fontHeight = 10;
	label.top = 0;
	return 0;
}

void CommonSensorViewerLine::operator()(TSize &l)
{
	if (SIZE_MINIMIZED == l.resizing || 0 == l.Width || 0 == l.Height)return;
	if (NULL != backScreen)
	{
		if (backScreen->GetWidth() < l.Width || backScreen->GetHeight() < l.Height)
		{
			delete backScreen;
			backScreen = new Bitmap(l.Width, l.Height);
		}
	}
	else
	{
		backScreen = new Bitmap(l.Width, l.Height);
	}
	chart->rect.right = l.Width;
	chart->rect.bottom = l.Height;

	Graphics g(backScreen);
	SolidBrush solidBrush(Color((ARGB)BACK_GROUND));
	g.FillRectangle(&solidBrush, 0, 0, 10, l.Height);
	g.FillRectangle(&solidBrush, 0, 0, l.Width, 29);

	chart->Draw(g);
	label.Draw(g);
}

void CommonSensorViewerLine::operator()(TPaint &l)
{
	if (NULL == backScreen)return;
	PAINTSTRUCT p;
	HDC hdc = BeginPaint(l.hwnd, &p);
	{
		Graphics g(hdc);
		g.DrawCachedBitmap(&CachedBitmap(backScreen, &g), 0, 0);
		cursor->VerticalCursor(storedMouseMove, (VGraphics &)PaintGraphics(g));
	}
	EndPaint(l.hwnd, &p);
}

void CommonSensorViewerLine::operator()(TCommand &l)
{
	EventDo(l);
}

void CommonSensorViewerLine::operator()(TMouseMove &l)
{
	if (mouseMove)
	{
		if (cursor->CrossCursor(l, (VGraphics &)HDCGraphics(l.hwnd, backScreen)))
		{
			storedMouseMove = l;
			chart->CoordLine(l.x, currentX);
			if (objMouseMove && ptrMouseMove) (objMouseMove->*ptrMouseMove)(currentX);
		}
	}
}

void CommonSensorViewerLine::operator()(TMouseWell &l)
{
	mouseMove = false;

	int offs = l.delta / 120;

	currentX -= offs;

	if (currentX > chart->count - 1) 	currentX = chart->count - 1;
	else  if (currentX < 0) currentX = 0;

	chart->LineCoord(storedMouseMove.x, currentX);
	cursor->CrossCursor(storedMouseMove, (VGraphics &)HDCGraphics(l.hwnd, backScreen));
	if (objMouseMove && ptrMouseMove) (objMouseMove->*ptrMouseMove)(currentX);
}

void CommonSensorViewerLine::operator()(TLButtonDown &l)
{
	mouseMove = false;
	storedMouseMove.x = l.x;
	storedMouseMove.y = l.y;
	chart->CoordLine(storedMouseMove.x, currentX);
	chart->LineCoord(storedMouseMove.x, currentX);
	cursor->CrossCursor(storedMouseMove, (VGraphics &)HDCGraphics(l.hwnd, backScreen));
}

void CommonSensorViewerLine::operator()(TLButtonDbClk &)
{
	mouseMove = true;
}

