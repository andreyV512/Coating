#include "ZoneViewer.h"
#include "templates/templates.hpp"

ZoneViewer::ZoneViewer()
	: tchart(backScreen)
	, tcursor(tchart)
{
	chart = &tchart;
	cursor = &tcursor;
	tcursor.horizontalLine = false;
	tchart.items.get<BarSeriesNoFixed>().SetColorBarHandler(this, &ZoneViewer::GetColorCell);
	tcursor.SetMouseMoveHandler(this, &ZoneViewer::Draw);
}

bool ZoneViewer::GetColorCell(int zone, double &data_, unsigned &color)
{
	//data_ = data[zone];
	color = zone % 2 ? 0xff00ff00 : 0xff0000ff;
	return zone < count;
}

bool ZoneViewer::Draw(TMouseMove &l, VGraphics &g)
{
	int x = currentX;

	bool drawZones = x < count;
	if (drawZones)
	{
		unsigned color = 0xff;
		wchar_t s[256] = L" test status";
		//char st = buffer.status[x];
		//StatusText()(st, color, s);

		Wchar_from<double, 1> Y(data[x]);

		wsprintf(label.buffer, L"<ff>зона %d Y %s <%6x>%s"
			, 1 + x
			, Y()
			, color
			, s
		);
		label.Draw(g());
	}
	return drawZones;
}
