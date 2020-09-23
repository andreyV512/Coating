#pragma once
#include "templates/typelist.hpp"
#include "App/AppBase.h"

//struct Norm {};
//struct noBottomReflection {};
//struct defect {};
//struct deadZone {};
//struct SensorOff {};

typedef Vlst<Norm, noBottomReflection, defect, deadZone, SensorOff > zone_status_list;

typedef Vlst<
	Vlst<SensorOff, Norm, noBottomReflection, defect, deadZone>
	, Vlst<Norm, noBottomReflection, defect, deadZone>
	, Vlst<noBottomReflection, deadZone>
	, Vlst<defect, deadZone>
> __skip_list__;

namespace StatusData
{	
	unsigned char Compute(unsigned char s0, unsigned char s1);
	void Text(unsigned char id, unsigned &color, const wchar_t *&txt);
	unsigned  Color(unsigned char id);

	void UpdateColor();
	void Test();
};