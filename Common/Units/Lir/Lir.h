#pragma once
class Lir
{
	void *cnt0_, *cnt1_;
	unsigned tmpTick, tick;
	double (Lir:: *ptrTick)();
	double &dx0, &dx1;
	double Tick0();
	double Tick1();
public:
	Lir();
	bool Init(int );
	void Clear();
	void Clear1();
	void Clear2();
	double Tick();
	void Change();
	unsigned Value1();
	unsigned Value2();
};