#include "SensorViewer.h"
#include "templates/templates.hpp"
#include "MessageText/status.h"

SensorViewer::SensorViewer(Data::SensorData &data)
	: tchart(backScreen)
	, tcursor(tchart)
	, data(data)
{
	chart = &tchart;
	cursor = &tcursor;
	tcursor.horizontalLine = false;
	tchart.items.get<BarSeriesNoFixed>().SetColorBarHandler(this, &SensorViewer::GetColorCell);
	tcursor.SetMouseMoveHandler(this, &SensorViewer::Draw);
	chart->minAxesX = 0;
	chart->maxAxesX = 500;
	chart->minAxesY = 0;
	chart->maxAxesY = 100;
	chart->rect.top = 17;
	label.fontHeight = 10;
	label.top = 0;
}

bool SensorViewer::GetColorCell(int zone, double &data_, unsigned &color)
{
	data_ = data.data[zone];
	color = StatusData::Color(data.status[zone]);
	return zone < data.count;
}

bool SensorViewer::Draw(TMouseMove &l, VGraphics &g)
{
	int x = currentX;

	bool drawZones = x < data.count;
	if (drawZones)
	{
		unsigned color = 0xff;
		const wchar_t *s = NULL;

		StatusData::Text(data.status[x], color, s);

		Wchar_from<double, 1> Y(data.data[x]);

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
