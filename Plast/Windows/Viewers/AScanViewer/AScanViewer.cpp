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
	chart->minAxesY = 0;
	chart->maxAxesY = 100;
	tcursor.SetMouseMoveHandler(this, &AScanViewer::Draw);
}

bool AScanViewer::Draw(TMouseMove &l, VGraphics &g)
{
	int x = currentX;
	bool drawZones = x < tchart.count;
	label.buffer[0] = '\0';
	if (drawZones)
	{
		wsprintf(label.buffer, L"<ff>смещение %d                       <%6x>."
			, x
			, BACK_GROUND
		);
	}
	label.Draw(g());
	return drawZones;
}