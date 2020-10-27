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
	label.buffer[0] = '\0';
	if (drawZones)
	{
		unsigned color = 0xff;
		const wchar_t *s = L"";

		char ds = data.status[x];
		StatusData::Text(ds, color, s);

		if (VL::IndexOf<zone_status_list, DeadZone>::value != ds)
		{
			Wchar_from<double, 1> Y(data.data[x]);
			wsprintf(label.buffer, L"<ff>зона %d Y %s <%6x>%s                  <%6x>."
				, 1 + x
				, Y()
				, color
				, s
				, BACK_GROUND
			);
		}
		else
		{
			wsprintf(label.buffer, L"<ff>зона %d <%6x>%s                      <%6x>."
				, 1 + x
				, color
				, s
				, BACK_GROUND
			);
		}
	}
	label.Draw(g());
	return drawZones;
}
