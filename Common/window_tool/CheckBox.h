#pragma once
#include "window_tool/TEvent.h"
class CheckBox: public TEvent
{
	HWND hWnd;
	class TObj{};
	TObj *obj;
	void (TObj:: *ptr)(bool);
	void Init(HWND, bool, wchar_t *);
public:
	template<class T>void Init(HWND hOwner, T *t, void (T::*ptr)(bool), bool b, wchar_t *txt)
	{
		obj = (TObj *)t;
		this->ptr = (void (TObj::*)(bool))ptr;
		Init(hOwner, b, txt);
	}
	bool operator()();
	void Do(TCommand &) override;
	void Size(int left, int top, int width, int height);
};

