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
		obj = (TObj *)o;
		Update = (void (TObj:: *)())ptr;
	}
};