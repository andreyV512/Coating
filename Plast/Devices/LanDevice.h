#pragma once
/*
* INFO —cбор данных в цикле
*/
#include "App/Config.h"
#ifndef INNER_LAN
#include "../LanProcess/LanDirect/LanDirect.h"
class LanDevice : public LanRead {};
#endif
class CollectionData
{	
	class LanDevice &device;
public:	
	CollectionData();
	~CollectionData();
};