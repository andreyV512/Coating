#pragma once
#include "templates/impl.hpp"


class AScanAuto
{
public:
	struct TObj {};
	TObj *obj;
	void (TObj::*Update)();
public:
	class IScan { public: virtual ~IScan() {} };
private:
	Impl<IScan, 32> impl;
public:
	AScanAuto();
	void Start();
	void Stop();
	template<class T>void SetHandle(T *o, void (T::*ptr)())
	{
		o = (TObj *)obj;
		Update = (void (TObj:: *)())ptr;
	}
};