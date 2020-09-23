#pragma once
#include "templates/typelist.hpp"

struct Norm {};
struct noBottomReflection {};
struct defect {};
struct deadZone {};
struct SensorOff {};

typedef Vlst<Norm, noBottomReflection, defect, deadZone, SensorOff > zone_status_list;

typedef Vlst<
	Vlst<Norm, noBottomReflection, defect, deadZone, SensorOff>
	, Vlst<noBottomReflection, deadZone, SensorOff>
	, Vlst<defect, deadZone, SensorOff>
	, Vlst<deadZone, SensorOff>
> __skip_list__;

class Status
{	
public:
	Status();
	unsigned char operator()(unsigned char s0, unsigned char s1);
};