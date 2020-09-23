#pragma once
#include "templates/typelist.hpp"

struct Norm {};
struct noBottomReflection {};
struct defect {};
struct deadZone {};
struct SensorOff {};

typedef Vlst<Norm, noBottomReflection, defect, SensorOff, deadZone > zone_status_list;

typedef Vlst<
	Vlst<SensorOff, Norm, noBottomReflection, defect, deadZone>
	, Vlst<Norm, noBottomReflection, defect, deadZone>
	, Vlst<noBottomReflection, deadZone>
	, Vlst<defect, deadZone>
> __skip_list__;

class Status
{	
public:
	Status();
	unsigned char operator()(unsigned char s0, unsigned char s1);
	void Test();
};