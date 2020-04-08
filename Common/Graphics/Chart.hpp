#pragma once
#include <Windows.h>
#include <Gdiplus.h>
#include "templates/typelist.hpp"

using namespace Gdiplus;
#pragma warning(disable : 4355)
template<class Chart, class List>class ChartDraw : public Chart
{
	template<class O, class P>struct __draw__{void operator()(O &o){
		o.Draw();
	}};
public:
	typedef List items_list;
	typedef VL::Factory<List> TItems;
	TItems items;
public:
	ChartDraw(Gdiplus::Bitmap *&backScreen_): Chart(backScreen_), items((Chart &)*this){}
	void Draw(Gdiplus::Graphics &graph)
	{
		Chart::Draw(graph);
		VL::foreach<List, __draw__>()(items);
	}
	template<class lst>void DrawItems(Gdiplus::Graphics &graph)
	{
		Chart::Draw(graph);
		VL::foreach<lst, __draw__>()(items);
	}
};