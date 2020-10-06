#pragma once
#include "templates/typelist.hpp"
#include "App/AppBase.h"

typedef Vlst<Norm, NoBottomReflection, Defect, DeadZone, SensorOff > zone_status_list;

typedef Vlst<
	Vlst<SensorOff, Norm, NoBottomReflection, Defect, DeadZone>
	, Vlst<Norm, NoBottomReflection, Defect, DeadZone>
	, Vlst<NoBottomReflection, DeadZone>
	, Vlst<Defect, DeadZone>
> __skip_list__;

namespace StatusData
{	
	static int norm = VL::IndexOf<zone_status_list, Norm>::value;
	static int defect = VL::IndexOf<zone_status_list, Defect>::value;
	static int noBottomReflection = VL::IndexOf<zone_status_list, NoBottomReflection>::value;
	unsigned char Compute(unsigned char s0, unsigned char s1);
	void Text(unsigned char id, unsigned &color, const wchar_t *&txt);
	unsigned  Color(unsigned char id);

	void UpdateColor();
};