#include "CommonSensorViewer.h"
#include "window_tool/MenuAPI.h"
#include "templates/templates.hpp"

CommonSensorViewer::CommonSensorViewer()
	: backScreen(NULL)
	, hWnd()
	, currentX(0)
	, mouseMove(true)
{}

CommonSensorViewer::~CommonSensorViewer()
{
	delete backScreen;
	zprint("\n");
}

LRESULT CommonSensorViewer::operator()(TCreate &l)
{
	chart->minAxesX = 0;
	chart->maxAxesX = 500;
	chart->minAxesY = 0;
	chart->maxAxesY = 20;
	chart->rect.top = 17;
	label.fontHeight = 10;
	label.top = 0;
	return 0;
}

void CommonSensorViewer::operator()(TSize &l)
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

	chart->maxAxesY = 20;
	chart->Draw(g);
}

void CommonSensorViewer::operator()(TPaint &l)
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

void CommonSensorViewer::operator()(TCommand &l)
{
	EventDo(l);
}

void CommonSensorViewer::operator()(TMouseMove &l)
{
	if (mouseMove)
	{
		if (cursor->CrossCursor(l, (VGraphics &)HDCGraphics(l.hwnd, backScreen)))
		{
			storedMouseMove = l;
		}
	}
}

void CommonSensorViewer::operator()(TMouseWell &l)
{
	mouseMove = false;

	int offs = l.delta / 120;

	currentX -= offs;

	if (currentX > chart->maxAxesX) 	currentX = (int)chart->maxAxesX - 1;
	else  if (currentX < 0) 	currentX = 0;
	int currentY = 0;
	chart->CellCoord(storedMouseMove.x, storedMouseMove.y, currentX, currentY);
	cursor->CrossCursor(storedMouseMove, (VGraphics &)HDCGraphics(l.hwnd, backScreen));
}

void CommonSensorViewer::operator()(TLButtonDown &l)
{
	mouseMove = false;
	storedMouseMove.x = l.x;
	storedMouseMove.y = l.y;
	//	 
	int currentY = 0;
	chart->CoordCell(storedMouseMove.x, storedMouseMove.y, currentX, currentY);
	chart->CellCoord(storedMouseMove.x, storedMouseMove.y, currentX, currentY);
	cursor->CrossCursor(storedMouseMove, (VGraphics &)HDCGraphics(l.hwnd, backScreen));
}

void CommonSensorViewer::operator()(TLButtonDbClk &)
{
	mouseMove = true;
}

