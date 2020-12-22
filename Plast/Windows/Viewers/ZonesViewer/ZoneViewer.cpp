#include "ZoneViewer.h"
#include "templates/templates.hpp"
#include "MessageText/status.h"
#include "Data/StoreAllParam.h"
#include "tools_debug/DebugMess.h"

ZoneViewer::ZoneViewer()
	: tchart(backScreen)
	, tcursor(tchart)
{
	dprint("7 start offset %f %f %f\n"
		, Singleton<ALLPatrams>::Instance().items.get<VL::Factory<TresholdsTable::items_list>>().get< Num<AlarmThreshStart, 0>>().value
		, Singleton<ALLPatrams>::Instance().items.get<VL::Factory<TresholdsTable::items_list>>().get< Num<AlarmThreshStart, 1>>().value
		, Singleton<ALLPatrams>::Instance().items.get<VL::Factory<TresholdsTable::items_list>>().get< Num<AlarmThreshStart, 2>>().value
	);
	chart = &tchart;
	cursor = &tcursor;
	tcursor.horizontalLine = false;
	tchart.items.get<BarSeriesNoFixed>().SetColorBarHandler(this, &ZoneViewer::GetColorCell);
	tcursor.SetMouseMoveHandler(this, &ZoneViewer::Draw);
	dprint("8 start offset %f %f %f\n"
		, Singleton<ALLPatrams>::Instance().items.get<VL::Factory<TresholdsTable::items_list>>().get< Num<AlarmThreshStart, 0>>().value
		, Singleton<ALLPatrams>::Instance().items.get<VL::Factory<TresholdsTable::items_list>>().get< Num<AlarmThreshStart, 1>>().value
		, Singleton<ALLPatrams>::Instance().items.get<VL::Factory<TresholdsTable::items_list>>().get< Num<AlarmThreshStart, 2>>().value
	);
}

bool ZoneViewer::GetColorCell(int zone, double &data_, unsigned &color)
{
	if (zone < count)
	{
		data_ = data[zone];
		color = StatusData::Color(status[zone]);
		return true;
	}
	return false;
}

bool ZoneViewer::Draw(TMouseMove &l, VGraphics &g)
{
	int x = currentX;

	bool drawZones = x < count;
	if (drawZones)
	{
		unsigned color = 0xff;
		const wchar_t *s = L"";
		StatusData::Text(status[x], color, s);
		Wchar_from<double, 1> Y(data[x]);

		wsprintf(label.buffer, L"<ff>датчик %d зона <ff00>%d <ff>смещение %d Y %s <%6x>%s                                            <%6x>."
			, sensor
			, zone
			, 1 + x
			, Y()
			, color
			, s
			, BACK_GROUND
		);
		label.Draw(g());
	}
	return drawZones;
}
