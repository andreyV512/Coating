#include "AScanViewer.h"
#include "templates/templates.hpp"
#include "Windows/MainWindow/MainWindow.h"
#include "App/AppBase.h"
#include "Data/StoreAllParam.h"
#include "tools_debug/DebugMess.h"

AScanViewer::AScanViewer()
	: tchart(backScreen)
	, tcursor(tchart)
	, data(tchart.items.get<LineSeries>().data)
{
	chart = &tchart;
	cursor = &tcursor;
	tcursor.horizontalLine = false;
	chart->minAxesY = 0;
	chart->maxAxesY = 100;
	tcursor.SetMouseMoveHandler(this, &AScanViewer::Draw);
}

bool AScanViewer::Draw(TMouseMove &l, VGraphics &g)
{
	int x = currentX;
	bool drawZones = x < tchart.count;

	double dx = tchart.maxAxesX / tchart.count;

	Wchar_from<double, 2> thick(dx * x);
	Wchar_from<double, 2> val(data[x]);

	label.buffer[0] = '\0';
	if (drawZones)
	{
		wsprintf(label.buffer, L"<ff>смещение %d  толщина %s мм уровень %s<%6x>xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
			, x
			, thick()
			, val()
			, BACK_GROUND
		);
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