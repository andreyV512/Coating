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
	class Lir &lir;
	class Lan &lan;
public:
	LanDevice(int);
	int Buff(char *&);
	void Confirm(int);
};

class CollectionData
{	
	LanDevice device;
public:	
	CollectionData(int);
	~CollectionData();

	void ChangeLir();
};