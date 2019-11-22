#include "DefectViewer.h"
#include "window_tool/MenuAPI.h"

DefectViewer::DefectViewer()
	: backScreen(NULL)
	, tchart(backScreen)
	, hWnd()
{
	tchart.items.get<FixedGridSerieX>().SetColorCellHandler(this, &DefectViewer::GetColorCell);
}

DefectViewer::~DefectViewer()
{
	delete backScreen;
}

LRESULT DefectViewer::operator()(TCreate &l)
{
	tchart.minAxesX = 0;
	tchart.maxAxesX = 500;
	tchart.minAxesY = 0;
	tchart.maxAxesY = 20;
	tchart.rect.top = 17;
	label.fontHeight = 10;
	label.top = 0;
	return 0;
}

void DefectViewer::operator()(TSize &l)
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
	tchart.rect.right = l.Width;
	tchart.rect.bottom = l.Height;

	Graphics g(backScreen);
	SolidBrush solidBrush(Color((ARGB)BACK_GROUND));
	g.FillRectangle(&solidBrush, 0, 0, 10, l.Height); 
	g.FillRectangle(&solidBrush, 0, 0, l.Width, 29);

	tchart.maxAxesY = 20;
	tchart.Draw(g);
}

void DefectViewer::operator()(TPaint &l)
{
	if (NULL == backScreen)return;
	PAINTSTRUCT p;
	HDC hdc = BeginPaint(l.hwnd, &p);
	{
		Graphics g(hdc);
		g.DrawCachedBitmap(&CachedBitmap(backScreen, &g), 0, 0);
	}
	EndPaint(l.hwnd, &p);
}

void DefectViewer::operator()(TCommand &l)
{
	EventDo(l);
}

void DefectViewer::operator()(TDropFiles &l)
{
}

bool DefectViewer::GetColorCell(unsigned sens, int zone, double &data, unsigned &color, bool &b)
{
	data = 20;
	color = sens % 2 ? 0xff00ff00 : 0xff0000ff;
	b = true;
	return zone < 50;
}
