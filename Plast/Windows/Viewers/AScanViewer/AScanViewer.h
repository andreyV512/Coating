#pragma once 
#include "Windows/Viewers/CommonSensorViewer.h"
#include "Graphics/FixedGridSeries.h"
#include "Graphics/Borders.h"
#include "Data/Data.h"

class AScanViewer : public CommonSensorViewer
{
public:
	class AlThr : public HOffsBorder { public: AlThr(Chart &c) : HOffsBorder(c) {} };
	class BtmRefThr : public HOffsBorder { public: BtmRefThr(Chart &c) : HOffsBorder(c) {} };
	class NAlThr : public HOffsBorder { public: NAlThr(Chart &c); void Draw(); };
	class NBtmRefThr : public HOffsBorder { public: NBtmRefThr(Chart &c); void Draw(); };
	typedef CommonSensorViewer Parent;
	typedef ChartDraw< Chart, Vlst<
		NoOffsetLeftAxes
		, BottomAxesInt
		, LineSeries
		, FixedGrid
		, AlThr
		, BtmRefThr
		, NAlThr
		, NBtmRefThr
	> > TChart;
	TChart tchart;
	Cursor tcursor;
	int numSensor;
	//Data::InputData &data;
	

	AScanViewer();

	//void operator()(TMouseWell &);

//	bool Draw(TMouseMove &, VGraphics &);
//	bool GetColorCell(int, double &, unsigned &, double &, unsigned &);
};