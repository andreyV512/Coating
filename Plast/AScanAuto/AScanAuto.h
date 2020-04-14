#pragma once
#include <Windows.h>

class AScanAuto
{
	static const int frame_size = 512;
	double data[frame_size * 3];
	struct Obj {};
	Obj *obj;
	void(Obj::*ptr)(double(&)[frame_size * 3]);
	HANDLE hTresh;
	bool terminate;
public:
	void Run();
public:
	
	AScanAuto();
	~AScanAuto();
	void Init();
	void Start();
	void Stop();

	template<class T> void SetHandler(T *t, void(T::*p)(double(&)[frame_size * 3]))
	{
		obj = (Obj *)t;
		ptr = (void(T:: *)(double(&)[frame_size * 3]))p;
	}
};