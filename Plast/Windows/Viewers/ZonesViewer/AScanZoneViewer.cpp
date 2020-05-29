#include "AScanZoneViewer.h"
#include "templates/templates.hpp"
#include "Windows/MainWindow/MainWindow.h"

AScanZoneViewer::AScanZoneViewer()
	: tchart(backScreen)
	, tcursor(tchart)
{
	chart = &tchart;
	cursor = &tcursor;
	tcursor.horizontalLine = false;
}

LRESULT AScanZoneViewer::operator()(TCreate &l)
{
	chart->minAxesX = 0;
	chart->maxAxesX = 255;
	chart->minAxesY = -5;
	chart->maxAxesY = 5;
	chart->rect.top = 17;
	label.fontHeight = 10;
	label.top = 0;
	return 0;
}


