#pragma once
#include "Graphics/FixedGridSeries.h"

class FixedGridSerieX: public FixedGridSeries
{
	class TObj{};
	typedef bool(TObj::*Tptr)(unsigned, unsigned, double &, unsigned &, bool &);
	TObj *obj;
	Tptr SetColorBar;
public:
	FixedGridSerieX(Chart &chart);
	void Draw();
	template<class T>void SetColorCellHandler(T *t, bool(T::*ptr)(unsigned, int, double &, unsigned &, bool &))
	{
		obj = (TObj *)t;
		SetColorBar = (Tptr)ptr;
	}
};