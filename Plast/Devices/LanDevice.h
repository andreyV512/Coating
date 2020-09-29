#pragma once
/*
* INFO —бор данных в цикле
*/
namespace Data
{
	struct InputData;
}

class LanDevice
{
	friend class CollectionData;
	int numberPackets;
	int packetSize;
	Data::InputData &data;
	class Lan &lan;
public:
	LanDevice();
	int Buff(char *&);
	void Confirm(unsigned);
};

class CollectionData
{	
	LanDevice device;
public:	
	CollectionData();
	~CollectionData();
};