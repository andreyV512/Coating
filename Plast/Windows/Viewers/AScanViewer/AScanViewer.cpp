#include "AScanViewer.h"
#include "templates/templates.hpp"
#include "Windows/MainWindow/MainWindow.h"
#include "App/AppBase.h"

AScanViewer::AScanViewer()
	: tchart(backScreen)
	, tcursor(tchart)
{
	chart = &tchart;
	cursor = &tcursor;
	tcursor.horizontalLine = false;
	//tchart.items.get<BarSeriesDouble>().SetColorBarHandler(this, &AScanViewer::GetColorCell);
	//tcursor.SetMouseMoveHandler(this, &AScanViewer::Draw);
	chart->minAxesY = -127;
	chart->maxAxesY = 127;
}



struct __mouse_well_data__
{
	AScanViewer *owner;
	TMouseWell mes;
};

template<class O, class P>struct __mouse_well__
{
	void operator()(O &o, P &p)
	{
		p.mes.hwnd = o.hWnd;
		o.currentX = p.owner->currentX;	
		SendMessage(MESSAGE(p.mes));
	}
};

//void AScanViewer::operator()(TMouseWell &l)
//{
//	__mouse_well_data__ data = {this, l};
//	VL::foreach<VL::EraseItem<MainWindow::viewers_list, AScanViewer>::Result, __mouse_well__>()(
//		Singleton<MainWindow>::Instance().viewers
//		, data);
//	(*(Parent *)this)(l);
//}

//unsigned SetMinColor(unsigned c)
//{
//	unsigned char(&x)[4] = (unsigned char(&)[4])c;
//	x[0] = unsigned char(0.75 * x[0]);
//	x[1] = unsigned char(0.75 * x[1]);
//	x[2] = unsigned char(0.75 * x[2]);
//	return c;
//}

//bool AScanViewer::GetColorCell(int zone, double &maxVal, unsigned &maxColor, double &minVal, unsigned &minColor)
//{
//	//maxVal = data.maxData[zone];
//	//minVal = data.minData[zone];
//	//minColor = zone % 2 ? 0xff00ff00 : 0xff0000ff;
//	//maxColor = SetMinColor(minColor);
//	return zone < data.count;
//}
//
//bool AScanViewer::Draw(TMouseMove &l, VGraphics &g)
//{
//	int x = currentX;
//
//	bool drawZones = x < data.count;
//	if (drawZones)
//	{
//		unsigned color = 0xff;
//		wchar_t s[256] = L" test status";
//		//char st = buffer.status[x];
//		//StatusText()(st, color, s);
//
//		Wchar_from<double, 1> Y(data.minData[x]);
//
//		wsprintf(label.buffer, L"<ff>зона %d Y %s <%6x>%s"
//			, 1 + x
//			, Y()
//			, color
//			, s
//		);
//		label.Draw(g());
//	}
//	return drawZones;
//}
AScanViewer::NBtmRefThr::NBtmRefThr(Chart &c) : HOffsBorder(c) {}
void AScanViewer::NBtmRefThr::Draw()
{
	if (value < 0) HOffsBorder::Draw();
}
AScanViewer::NAlThr::NAlThr(Chart &c) : HOffsBorder(c) {}
void AScanViewer::NAlThr::Draw()
{
	if (value < 0) HOffsBorder::Draw();
}
