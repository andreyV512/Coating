#include "SensorViewer.h"
#include "templates/templates.hpp"

SensorViewer::SensorViewer(Data::SensorData *data)
	: tchart(backScreen)
	, tcursor(tchart)
	, data(data)
{
	chart = &tchart;
	cursor = &tcursor;
	tcursor.horizontalLine = false;
	tchart.items.get<BarSeriesNoFixed>().SetColorBarHandler(this, &SensorViewer::GetColorCell);
	tcursor.SetMouseMoveHandler(this, &SensorViewer::Draw);
}

bool SensorViewer::GetColorCell(int zone, double &data_, unsigned &color)
{
	data_ = data->outputData[zone];
	color = zone % 2 ? 0xff00ff00 : 0xff0000ff;
	return zone < data->count;
}

bool SensorViewer::Draw(TMouseMove &l, VGraphics &g)
{
	int x = currentX;

	bool drawZones = x < data->count;
	if (drawZones)
	{
		unsigned color = 0xff;
		wchar_t s[256] = L" test status";
		//char st = buffer.status[x];
		//StatusText()(st, color, s);

		Wchar_from<double, 1> Y(data->outputData[x]);

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
