#pragma once
#include <windows.h>
#include <CommCtrl.h>
#include "window_tool/TEvent.h"
#include "Log/LogBuffer.h"

class GridMessages : public TEvent
{
	unsigned offset, offsetNext;
	bool isSelect;
	struct Log::TData *d;
public:
	HWND hWnd;
	void Create(HWND);
	void SetDataToGrid(LV_DISPINFO *);
	void SetColorToGrid(NMLVCUSTOMDRAW *);
	LRESULT Do(TNotify &) override;
};
