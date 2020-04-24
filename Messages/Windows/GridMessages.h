#pragma once
#include <windows.h>
#include <CommCtrl.h>
#include "window_tool/TEvent.h"

class GridMessages : public TEvent
{
public:
	HWND hWnd;
	void Create(HWND);
	void SetDataToGrid(LV_DISPINFO *);
	void SetColorToGrid(NMLVCUSTOMDRAW *);
	LRESULT Do(TNotify &) override;
};
