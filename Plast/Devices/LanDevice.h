#pragma once
/*
* INFO —бор данных в цикле
void CollectionData::ChangeLir(); - мен€ет лир 
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
//	class Lir &lir;
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

	//void ChangeLir();
};