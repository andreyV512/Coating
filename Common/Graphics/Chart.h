#pragma once
#include "templates/typelist.hpp"
#include "window_tool/message.h"
#include <GdiPlus.h>

struct VGraphics
{
  virtual Gdiplus::Graphics &operator()() = 0;
  virtual ~VGraphics(){};
};
struct HDCGraphics:VGraphics
{
	HWND h;
	Gdiplus::Bitmap *bitmap;
	HDC hdc;
	Gdiplus::Graphics graphics;
	HDCGraphics(HWND h, Gdiplus::Bitmap *bitmap) 
		: h(h)
		, bitmap(bitmap)
		, hdc(GetDC(h))
		, graphics(hdc)
	{
		if(NULL != bitmap)
		    graphics.DrawCachedBitmap(&Gdiplus::CachedBitmap(bitmap, &graphics), 0, 0);
	} 
	~HDCGraphics(){ReleaseDC(h, hdc);}
	Gdiplus::Graphics &operator()(){return graphics;}
};
struct PaintGraphics:VGraphics
{
	Gdiplus::Graphics &graphics;
	explicit PaintGraphics(Gdiplus::Graphics &g) 
		:graphics(g)
	{}
	Gdiplus::Graphics &operator()(){return graphics;}
};

inline bool InRect(int x, int y, RECT &r)
{
	return r.top < y && r.bottom > y && r.left < x && r.right > x;
}

class Chart
{
public:
  int count = 1;
  RECT rect;
  int offsetAxesLeft, offsetAxesBottom, offsetAxesTop, offsetAxesRight;
  double minAxesX, maxAxesX, minAxesY, maxAxesY, minAxesYRight, maxAxesYRight;
  double minScaleX, maxScaleX, dScaleX, minScaleY, maxScaleY;
  int colorAxes;
  int colorFontAxes;
  int fontHeight;
  int colorGrid;
  double offsetGridX, offsetGridY;
  double deltaTickX, deltaTickY, deltaDigitX, deltaTickYRight, deltaDigitY, deltaDigitYRight;
  double dX = 1.0;
  double dY = 1.0;
  Gdiplus::Bitmap *&backScreen;
  Gdiplus::Graphics *g;
  Chart(Gdiplus::Bitmap *&backScreen);
  virtual ~Chart() = default;
  int GetCountDigit(double , double, double &, Gdiplus::Font &); 
  bool AxesValues(int, int, double &, double &);
  bool ValuesAxes(double, double, int &, int &);
  virtual void Draw(Gdiplus::Graphics &graph);
  void Restore(HWND);
  int BetweenLeftRight(int);
  void OffsetToPixelHorizontal(WORD &offsX, int delta);
  void OffsetToPixelVertical(WORD &offsY, int delta);
  void CoordCell(int mX, int mY, int &x, int &y);
  void CellCoord(WORD &mX, WORD &mY, int x, int y);
  void CoordLine(int mX, int &x);
  void LineCoord(WORD &mX, int x);
};
class LeftAxes
{
public:
  Chart &chart;
  LeftAxes(Chart &chart);
  void Draw();
};
class BottomAxes
{
	double minA, maxA;
	int offsMax;
	int offsMin;
	double deltaTickDigit;
public:
  Chart &chart;
  BottomAxes(Chart &chart);
  void Draw();
  void OffsetToPixel(WORD &, int);
};

class BottomAxesMeters
{
	int offsMax;
	int offsMin;
	double deltaTickDigit;
public:
  Chart &chart;
  double minBorder, maxBorder;
  BottomAxesMeters(Chart &chart);
  void Draw();
};

void CoordCell( Chart &chart, int mX, int &x, int delta);

class BottomAxesInt
{
	double minA, maxA;	
	double deltaTickDigit;
public:
    int offsMax;
    int offsMin;
  Chart &chart;
  BottomAxesInt(Chart &chart);
  void Draw();
  void OffsetToPixel(WORD &, int );
};

class Grid
{
public:
  Chart &chart;
  double *deltaY;
  Grid(Chart &chart) : chart(chart), deltaY(&chart.deltaTickY) {}
  void Draw();
  void DrawSample(Chart &, int);
};
class LineSeries
{
public:
	Chart &chart;
	int count;
	double *data;
	int color;
	int dashStyle;
	int widthPen;
	LineSeries(Chart &chart);
	void Draw();
	void SetData(double *data, int countSamples, double minValue, double maxValue);
	void SetData(double *d, int c);
};
class Cursor
{
	struct TObj{};
	typedef bool(TObj::*TPtrMove)(TMouseMove &, VGraphics &);	
	TObj *oMove;
	TPtrMove ptrMove;	
public:
   double x0, y0, x1, y1;
   Chart &chart;
   bool horizontalLine;
   Cursor(Chart &chart);
   void Draw();

   bool VerticalCursor(TMouseMove &, VGraphics &);
   bool CrossCursor(TMouseMove &, VGraphics &);
   void VerticalCursorNoTest(TMouseMove &, VGraphics &);
   template<class T>void SetMouseMoveHandler(T *t, bool(T::*to)(TMouseMove &, VGraphics &))
   {
	   oMove = (TObj *)t;
	   ptrMove = (TPtrMove)to;
   }   
};
//--------------------?????? ?????????????????????? ??????????????????????????
void OffsetAxes(int , int , double , double , double &, double &, double &, double &);




