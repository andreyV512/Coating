#pragma once
/*
* INFO —cбор данных в цикле
*/
#include "App/Config.h"
#ifndef INNER_LAN
//получение данных из другого процесса
#include "../LanProcess/LanDirect/LanDirect.h"
class LanDevice : public LanRead {};
#endif
class CollectionData
{	
//	class LanDevice &device;
	const unsigned generatorBit;
public:	
	CollectionData();
	~CollectionData();
};