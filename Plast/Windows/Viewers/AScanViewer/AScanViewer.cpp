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
