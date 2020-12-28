#pragma once
#include <windows.h>
#include <CommCtrl.h>
#include "window_tool/TEvent.h"
#include "Log/LogBuffer.h"
#include <vector>
#include <algorithm>

class GridMessages : public TEvent
{
	Log::TData buf[1024];
	unsigned head, tail;
	std::vector<unsigned>skip;
public:
	HWND hWnd;
	void Create(HWND);
	void SetDataToGrid(LV_DISPINFO *);
	void SetColorToGrid(NMLVCUSTOMDRAW *);
	LRESULT Do(TNotify &) override;

	void Update();
	void Clean();
	void SetSkip(unsigned);
};
