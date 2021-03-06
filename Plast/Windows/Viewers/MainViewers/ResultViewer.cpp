#include "ResultViewer.h"
#include "templates/templates.hpp"
#include "Windows/MainWindow/MainWindow.h"
#include "MessageText/status.h"

ResultViewer::ResultViewer()
	: tchart(backScreen)
	, tcursor(tchart)
	, data(Singleton<Data::ResultData>::Instance())
{
	chart = &tchart;
	cursor = &tcursor;
	tcursor.horizontalLine = false;
	tchart.items.get<BarSeriesDouble>().SetColorBarHandler(this, &ResultViewer::GetColorCell);
	tcursor.SetMouseMoveHandler(this, &ResultViewer::Draw);
	chart->minAxesX = 0;
	chart->maxAxesX = 500;
	chart->minAxesY = 0;
	chart->maxAxesY = 100;
}

struct __mouse_well_data__
{
	ResultViewer *owner;
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

void ResultViewer::operator()(TMouseWell &l)
{
	__mouse_well_data__ data = {this, l};
	VL::foreach<VL::Erase<MainWindow::viewers_list, ResultViewer>::Result, __mouse_well__>()(
		Singleton<MainWindow>::Instance().viewers
		, data);
	(*(Parent *)this)(l);
}

unsigned SetMinColor(unsigned c)
{
	unsigned char(&x)[4] = (unsigned char(&)[4])c;
	x[0] = unsigned char(0.75 * x[0]);
	x[1] = unsigned char(0.75 * x[1]);
	x[2] = unsigned char(0.75 * x[2]);
	return c;
}

bool ResultViewer::GetColorCell(int zone, double &maxVal, unsigned &maxColor, double &minVal, unsigned &minColor)
{
	maxVal = data.maxData[zone];
	minVal = data.minData[zone];
	minColor = StatusData::Color(data.status[zone]);
	maxColor = SetMinColor(minColor);
	return zone < data.count;
}

bool ResultViewer::Draw(TMouseMove &l, VGraphics &g)
{
	int x = currentX;

	bool drawZones = x < data.count;
	label.buffer[0] = '\0';
	if (drawZones)
	{
		unsigned color = 0xff;
		const wchar_t *s = L"";

		char ds = data.status[x];
		StatusData::Text(ds, color, s);

		if (VL::IndexOf<zone_status_list, DeadZone>::value != ds)
		{
			Wchar_from<double, 1> Y(data.minData[x]);
			Wchar_from<double, 1> YY(data.maxData[x]);
			wsprintf(label.buffer, L"<ff>зона %d мин.%s мах.%s<%6x>%s<%6x>xxxxxxxxxxxxxxxxxxxx"
				, 1 + x
				, Y()
				, YY()
				, color
				, s
				, BACK_GROUND
			);
		}
		else
		{
			wsprintf(label.buffer, L"<ff>зона %d <%6x>%s<%6x>xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
				, 1 + x
				, color
				, s
				, BACK_GROUND
			);
		}
	}
	else
	{
		wsprintf(label.buffer, L"<%6x>xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
			, BACK_GROUND
		);
	}
	label.Draw(g());
	return drawZones;
}
